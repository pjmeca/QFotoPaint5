//---------------------------------------------------------------------------

#include "imagenes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <assert.h>
#include <QClipboard>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES                        //////////////
///////////////////////////////////////////////////////////////////

ventana foto[MAX_VENTANAS];

tipo_herramienta herr_actual= HER_PUNTO;

int radio_pincel= 10;

Scalar color_pincel= CV_RGB(255, 255, 255);

int difum_pincel= 10;

bool preguntar_guardar= true;

static int numpos= 0; // Número actual en el orden de posición de las ventanas

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void callback (int event, int x, int y, int flags, void *_nfoto);

//---------------------------------------------------------------------------

void inic_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        foto[i].usada= false;
}

//---------------------------------------------------------------------------

void fin_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            destroyWindow(foto[i].nombre);
            foto[i].usada= false;
        }
    }
}

//---------------------------------------------------------------------------

int primera_libre (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (!foto[i].usada)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

void escribir_barra_estado (void)
{
    int usadas, modificadas;
    num_fotos(usadas, modificadas);
    w->setStatusBarText(QString::number(usadas)+" fotos abiertas, "+
                        QString::number(modificadas)+" modificadas.");
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, int ancho, int alto, Scalar color)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img.create(alto, ancho, CV_8UC3);
    foto[nfoto].img=  color;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, ancho, alto);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, Mat img)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada && !img.empty());
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img= img;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, img.cols, img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= nombre;
    foto[nfoto].nombref= Lt1(nombre);
    foto[nfoto].img= imread(foto[nfoto].nombref);
    if (foto[nfoto].img.empty())
        return;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= false;
    foto[nfoto].roi= Rect(0, 0, foto[nfoto].img.cols, foto[nfoto].img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void guardar_foto (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    foto[nfoto].modificada= false;
    // 1. Guardar la imagen con el mismo nombre que tiene
    if (nombre=="")
        imwrite(foto[nfoto].nombref, foto[nfoto].img);
    // 2. Guardar la imagen con un nombre distinto al actual
    else {
        imwrite(Lt1(nombre), foto[nfoto].img);
        setWindowTitle(foto[nfoto].nombre, nombre);
    }
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void cerrar_foto (int nfoto, bool destruir_ventana)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (destruir_ventana)
        destroyWindow(foto[nfoto].nombre);
    foto[nfoto].usada= false;
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void mostrar (int nfoto)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

int nombre_a_numero (string nombre)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (foto[i].usada && foto[i].nombre==nombre)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

int foto_activa (void)
{
    int maxorden= -1, maxpos= -1;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada && foto[i].orden>maxorden) {
            maxorden= foto[i].orden;
            maxpos= i;
        }
    }
    return maxpos;
}

//---------------------------------------------------------------------------

int num_fotos (int &usadas, int &modificadas)
{
    usadas= 0;
    modificadas= 0;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            usadas++;
            if (foto[i].modificada)
                modificadas++;
        }
    }
    return usadas;
}

//---------------------------------------------------------------------------

void set_callback_foto (int nfoto, bool activo)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (!activo)
        setMouseCallback(foto[nfoto].nombre, nullptr);
    else
        setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DEL CALLBACK PRINCIPAL          //////////////
///////////////////////////////////////////////////////////////////

static int downx, downy;
// Posición inicial del ratón al pinchar sobre la imagen actual

//---------------------------------------------------------------------------

void ninguna_accion (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    circle(res, Point(x, y), radio_pincel, CV_RGB(255,255,255), 2, LINE_AA);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_close (int factual)
{
    if (foto[factual].usada && !foto[factual].img.empty()) {
        if (foto[factual].modificada && preguntar_guardar) {
            QString cadena= "El archivo " + QString::fromStdString(foto[factual].nombre) +
                    " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(w, "Archivo modificado", cadena,
                                            QMessageBox::Yes | QMessageBox::No );
            if (resp==QMessageBox::Yes)
                guardar_foto(factual);
        }
        cerrar_foto(factual, false);
    }
}

//---------------------------------------------------------------------------

void cb_punto (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color_pincel, -1, LINE_AA);
    else {

        int t = radio_pincel+difum_pincel;
        int posx = t, posy=t;
        Rect roi(x-t, y-t, 2*t+1, 2*t+1);

        // Si el ROI se sale de la imagen
        if (roi.x < 0){
            roi.width += roi.x;
            posx += roi.x;
            roi.x = 0;
        }
        if (roi.y < 0){
            roi.height += roi.y;
            posy += roi.y;
            roi.y = 0;
        }
        if (roi.x+roi.width > im.cols){
            roi.width = im.cols-roi.x;
        }
        if (roi.y+roi.height > im.rows){
            roi.height = im.rows-roi.y;
        }

        Mat frag = im(roi);
        Mat res(frag.size(), im.type(), color_pincel);
        Mat cop(frag.size(), im.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(frag, cop, frag, 1.0/255.0);
        frag = res + frag;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_linea (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        line(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        line(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel*2+1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_ver_linea (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    line(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_rectangulo (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    if (difum_pincel==0)
        rectangle(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        rectangle(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_ver_rectangulo(int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    rectangle(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1); // negativo para que rellene el rectangulo si el radio del pincel es 0 (si rectangle recibe un número negativo, lo rellena)
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_elipse (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    if (difum_pincel==0)
        ellipse(im, Point(downx, downy), Size(abs(x-downx), abs(y-downy)), 0, 0, 360, color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        ellipse(cop, Point(downx, downy), Size(abs(x-downx), abs(y-downy)), 0, 0, 360, CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_ver_elipse (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    ellipse(res, Point(downx, downy), Size(abs(x-downx), abs(y-downy)), 0, 0, 360, color_pincel, radio_pincel*2-1); // negativo para que rellene el rectangulo si el radio del pincel es 0 (si rectangle recibe un número negativo, lo rellena)
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

Scalar ColorArcoiris()
{
       static Scalar colorActual = CV_RGB(255,0,0);
       static int estado = 0;
       int incremento = 8;

       switch(estado) {
       case 0 :
           colorActual.val[1] += incremento;
           if(colorActual.val[1] >= 255) estado = 1;
           break;
       case 1 :
           colorActual.val[2] -= incremento;
           if (colorActual.val[2] <= 0) estado = 2;
           break;
       case 2 :
           colorActual.val[0] += incremento;
           if (colorActual.val[0] >= 255) estado = 3;
           break;
       case 3 :
           colorActual.val[1] -= incremento;
           if (colorActual.val[1] <= 0) estado = 4;
           break;
       case 4 :
           colorActual.val[2] += incremento;
           if (colorActual.val[2] >= 255) estado = 5;
           break;
       case 5 :
           colorActual.val[0] -= incremento;
           if (colorActual.val[0] <= 0) estado = 0;
           break;
       }

       return colorActual;
}

void cb_arcoiris (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, ColorArcoiris(), -1, LINE_AA);
    else {

        int t = radio_pincel+difum_pincel;
        int posx = t, posy=t;
        Rect roi(x-t, y-t, 2*t+1, 2*t+1);

        // Si el ROI se sale de la imagen
        if (roi.x < 0){
            roi.width += roi.x;
            posx += roi.x;
            roi.x = 0;
        }
        if (roi.y < 0){
            roi.height += roi.y;
            posy += roi.y;
            roi.y = 0;
        }
        if (roi.x+roi.width > im.cols){
            roi.width = im.cols-roi.x;
        }
        if (roi.y+roi.height > im.rows){
            roi.height = im.rows-roi.y;
        }

        Mat frag = im(roi);
        Mat res(frag.size(), im.type(), ColorArcoiris());
        Mat cop(frag.size(), im.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(frag, cop, frag, 1.0/255.0);
        frag = res + frag;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_seleccionar (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    Rect nuevo= Rect(min(downx, x), min(downy, y),
                     max(downx, x)-min(downx, x)+1, max(downy, y)-min(downy, y)+1);
    if (nuevo.x<0)
        nuevo.x= 0;
    if (nuevo.y<0)
        nuevo.y= 0;
    if (nuevo.x+nuevo.width>im.size().width)
        nuevo.width= im.size().width-nuevo.x;
    if (nuevo.y+nuevo.height>im.size().height)
        nuevo.height= im.size().height-nuevo.y;
    foto[factual].roi= nuevo;
}

//---------------------------------------------------------------------------

void cb_ver_seleccion (int factual, int x, int y, bool foto_roi)
{
    Mat res= foto[factual].img.clone();
    Point p1, p2;
    if (foto_roi) {
        p1.x= foto[factual].roi.x;
        p1.y= foto[factual].roi.y;
        p2.x= foto[factual].roi.x+foto[factual].roi.width-1;
        p2.y= foto[factual].roi.y+foto[factual].roi.height-1;
    }
    else {
        p1= Point(downx, downy);
        p2= Point(x, y);
    }
    rectangle(res, p1, p2, CV_RGB(255,foto_roi?0:255,0),2);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void callback (int event, int x, int y, int flags, void *_nfoto)
{
    int factual= reinterpret_cast<int>(_nfoto);

    // 1. Eventos y casos especiales
    // 1.1. Evento cerrar ventana
    if (event==EVENT_CLOSE) {
        cb_close(factual);
        return;
    }
    // 1.2. Evento obtiene el foco
    if (event==EVENT_FOCUS) {
        foto[factual].orden= numpos++;
    }
    // 1.3. El ratón se sale de la ventana
    if (x<0 || x>=foto[factual].img.size().width || y<0 || y>=foto[factual].img.size().height)
        return;
    // 1.4. Se inicia la pulsación del ratón
    if (event==EVENT_LBUTTONDOWN) {
        downx= x;
        downy= y;
    }

    // 2. Según la herramienta actual
    switch (herr_actual) {

    // 2.1. Herramienta PUNTO
    case HER_PUNTO:
        if (flags==EVENT_FLAG_LBUTTON)
            cb_punto(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.2. Herramienta LINEA
    case HER_LINEA:
        if (event==EVENT_LBUTTONUP)
            cb_linea(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_linea(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.3. Herramienta RECTANGULO
    case HER_RECTANGULO:
        if (event==EVENT_LBUTTONUP)
            cb_rectangulo(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_rectangulo(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;


        // 2.4. Herramienta ELIPSE
    case HER_ELIPSE:
        if (event==EVENT_LBUTTONUP)
            cb_elipse(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_elipse(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.5. Herramienta PUNTO ARCOÍRIS
    case HER_ARCOIRIS:
        if (flags==EVENT_FLAG_LBUTTON)
            cb_arcoiris(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.6. Herramienta SELECCION
    case HER_SELECCION:
        if (event==EVENT_LBUTTONUP)
            cb_seleccionar(factual, x, y);
        else if (event==EVENT_MOUSEMOVE)
            cb_ver_seleccion(factual, x, y, flags!=EVENT_FLAG_LBUTTON);
        break;

        // 2.7. Herramienta TRAZO
    case HER_TRAZO:
        if (flags==EVENT_FLAG_LBUTTON){
            cb_punto(factual, x, y);
            cb_linea(factual, x, y);
            downx = x;
            downy = y;
        }
        else
            ninguna_accion(factual, x, y);
        break;
    }
    escribir_barra_estado();
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres)
{
    Mat img(foto[nfoto].img.size(), foto[nfoto].img.type());
    bitwise_not(foto[nfoto].img, img);
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void rotar_angulo (Mat imagen, Mat &imgRes, double angulo, double escala, int modo)
{
    double w= imagen.size().width, h= imagen.size().height;
    Size sres;
    if (modo==0) {     // Reescalar con proporción al original
        sres.width= int(w*escala);
        sres.height= int(h*escala);
    }
    else if (modo==1)  // Reescalar y ampliar para caber entera
        sres.width= sres.height= int(sqrt(w*w + h*h)*escala);
    else               // Reescalar y recortar para no mostrar borde
        sres.width= sres.height= int(min(w, h)*escala/sqrt(2.0));
    imgRes.create(sres, imagen.type());
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= sres.width/2+cx*escala;
    M.at<double>(1,2)= sres.height/2+cy*escala;
    imgRes= color_pincel;
    warpAffine(imagen, imgRes, M, sres, INTER_CUBIC);
}

//---------------------------------------------------------------------------

void rotar_exacto (int nfoto, int nres, int grado)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(nres>=0 && nres<MAX_VENTANAS && !foto[nres].usada);
    Mat entrada= foto[nfoto].img;
    Mat salida;
    if (grado == 0)
        salida= entrada.clone();
    else if (grado == 1) {
        transpose(entrada, salida);
        flip(salida, salida, 1);
    }
    else if (grado == 2)
        flip(entrada, salida, -1);
    else if (grado == 3) {
        transpose(entrada, salida);
        flip(salida, salida, 0);
    }
    crear_nueva(nres, salida);
}

//---------------------------------------------------------------------------

void ver_brillo_contraste (int nfoto, double suma, double prod, double gamma, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img;
    foto[nfoto].img.convertTo(img, CV_8UC3, prod, suma);
    Mat img32f;
    img.convertTo(img32f, CV_32F, 1.0/255);
    pow(img32f, gamma, img32f);
    img32f.convertTo(img, CV_8U, 255);
    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void ver_bajorrelieve (int nfoto, double angulo, double grado, int nfondo, bool guardar)
{
    QString nombres[4]={":/imagenes/arena.jpg",
                       ":/imagenes/cielo.jpg",
                       ":/imagenes/gris.png",
                       ":/imagenes/madera.jpg"};
    QImage imq= QImage(nombres[nfondo]);
    Mat imgfondo(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imgfondo, imgfondo, COLOR_RGBA2RGB);
    resize(imgfondo, imgfondo, foto[nfoto].img.size());

    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    Mat rotada;
    rotar_angulo(gris, rotada, angulo, 1.0, 1);
    Mat sobel;
    Sobel(rotada, sobel, CV_8U, 1, 0, 3, grado, 128, BORDER_REFLECT);
    rotar_angulo(sobel, rotada, -angulo, 1.0, 0);
    Mat res;
    res= rotada(Rect((rotada.cols-gris.cols)/2,
                     (rotada.rows-gris.rows)/2,
                     gris.cols, gris.rows));
    cvtColor(res, res, COLOR_GRAY2BGR); // lo devolvemos a 3 canales

    addWeighted(res, 1.0, imgfondo, 1.0, -128, res); // le sumamos el fondo

    if(guardar)
        crear_nueva(primera_libre(), res);
    else
        imshow("Bajorelieve", res);
}

//---------------------------------------------------------------------------

void escala_color(int nfoto, int nres)
{
    Mat grises;
    cvtColor(foto[nfoto].img, grises, COLOR_BGR2GRAY);
    cvtColor(grises, grises, COLOR_GRAY2BGR);
    Mat lut(1, 256, CV_8UC3);
    int vr = color_pincel.val[2];
    int vg = color_pincel.val[1];
    int vb = color_pincel.val[0];
    for (int A=0; A<256; A++)
        if(A<128)
            lut.at<Vec3b>(A) = Vec3b(vb*A/128, vg*A/128, vr*A/128);
        else
            lut.at<Vec3b>(A) = Vec3b(vb+(255-vb)*(A-128)/128,
                                     vg+(255-vg)*(A-128)/128,
                                     vr+(255-vr)*(A-128)/128);
    Mat imgres;
    LUT(grises, lut, imgres);
    crear_nueva(nres, imgres);
}

//---------------------------------------------------------------------------

void ver_pinchar_estirar(int nfoto, int cx, int cy,
                         double radio, double grado,
                         bool guardar)
{
    Mat S(foto[nfoto].img.rows, foto[nfoto].img.cols, CV_32FC1);
    for(int y=0; y<S.rows; y++)
        for(int x=0; x<S.cols; x++)
            S.at<float>(y,x)=exp(-((x-cx)*(x-cx)+(y-cy)*(y-cy))/(radio*radio));
            //S.at<float>(y,x)=sin(0.01*sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)) + radio/100.0); // para el efecto de agua
    Mat Gx, Gy;
    Sobel(S, Gx, CV_32F, 1, 0, 3, grado, 0, BORDER_REFLECT);
    Sobel(S, Gy, CV_32F, 0, 1, 3, grado, 0 , BORDER_REFLECT);
    multiply(S, Gx, Gx);
    multiply(S, Gy, Gy);
    for(int y=0; y<S.rows; y++)
        for(int x=0; x<S.cols; x++) {
            Gx.at<float>(y, x) += x;
            Gy.at<float>(y, x) += y;
        }
    Mat imgres;
    remap(foto[nfoto].img, imgres, Gx, Gy, INTER_LINEAR, BORDER_REFLECT);
    imshow("Pinchar/estirar", imgres);

    if(guardar){
        imgres.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
        mostrar(nfoto);
    }
}

//---------------------------------------------------------------------------

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(tamx>0 && tamx&1 && tamy>0 && tamy&1);
    Mat img= foto[nfoto].img.clone();
    Mat fragmento = img(foto[nfoto].roi); // para que se apliquen los suavizados sobre el ROI

    if (ntipo == 1)
        GaussianBlur(fragmento, fragmento, Size(tamx, tamy), 0);
    else if (ntipo == 2)
        blur(fragmento, fragmento, Size(tamx, tamy));
    else if (ntipo == 3)
        medianBlur(fragmento, fragmento, tamx);
    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void ver_ajuste_lineal(int nfoto, double pmin,
                       double pmax, bool guardar)
{
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    int canales[1] = {0};
    int bins[1] = {256};
    float rango[2] = {0, 256};
    const float *rangos[] = {rango};
    Mat hist;
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    normalize(hist, hist, 100, 0, NORM_L1);
    int vmin=0;
    for(double acum=0; vmin<256 && acum<pmin; vmin++)
        acum += hist.at<float>(vmin);
    int vmax = 255;
    for(double acum = 0; vmax>=0 && acum<pmax; vmax--)
        acum += hist.at<float>(vmax);
    if(vmin>=vmax)
        vmax=vmin+1;
    double a = 255.0/(vmax-vmin);
    double b= -vmin*a;
    Mat res;
    foto[nfoto].img.convertTo(res, CV_8U, a, b);
    imshow(foto[nfoto].nombre, res);
    if(guardar) {
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }

}

//---------------------------------------------------------------------------

void ver_matsatlum (int nfoto, int matiz, double sat,
                    double lum, bool guardar)
{
    Mat hls;
    cvtColor(foto[nfoto].img, hls, COLOR_BGR2HLS_FULL);
    Mat canales[3];
    split(hls, canales);
    canales[0].convertTo(canales[0], CV_16S, 1, matiz);
    bitwise_and(canales[0], 255, canales[0]);
    canales[0].convertTo(canales[0], CV_8U);
    canales[1] *= lum;
    canales[2] *= sat;
    merge(canales, 3, hls);
    Mat imgres;
    cvtColor(hls, imgres, COLOR_HLS2BGR_FULL);

    imshow(foto[nfoto].nombre, imgres);
    if(guardar){
        imgres.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void ver_perfilado (int nfoto, double grado,
                    int radio, bool guardar)
{
    Mat laplace;
    Laplacian(foto[nfoto].img, laplace, CV_16S, radio, -grado, 0, BORDER_REFLECT);
    Mat img16;
    foto[nfoto].img.convertTo(img16, CV_16S);
    img16 += laplace;
    Mat imgres;
    img16.convertTo(imgres, CV_8U);

    imshow(foto[nfoto].nombre, imgres);
    if(guardar){
        imgres.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void ver_perspectiva(int nfoto1, int nfoto2, Point2f pt1[], Point2f pt2[], bool guardar)
{
    Mat m=getPerspectiveTransform(pt1, pt2);
    Mat imres = foto[nfoto2].img.clone();
    warpPerspective(foto[nfoto1].img, imres, m, imres.size(), INTER_LINEAR, BORDER_TRANSPARENT);
    if (guardar) {
        imres.copyTo(foto[nfoto2].img);
        foto[nfoto2].modificada = true;
        mostrar(nfoto2);
    } else{
        for(int i=0; i<4; i++)
            line(imres, pt2[i], pt2[(i+1)%4], CV_RGB(0,0,0), 2);
        for(int i=0; i<4; i++)
            circle(imres, pt2[i], 10, CV_RGB(255, 0, 0), -1);
        imshow("Perspectiva", imres);
    }
}

//---------------------------------------------------------------------------

void media_ponderada (int nf1, int nf2, int nueva, double peso)
{
    assert(nf1>=0 && nf1<MAX_VENTANAS && foto[nf1].usada);
    assert(nf2>=0 && nf2<MAX_VENTANAS && foto[nf2].usada);
    assert(nueva>=0 && nueva<MAX_VENTANAS && !foto[nueva].usada);
    Mat img= foto[nf1].img.clone();
    Mat cop;
    resize(foto[nf2].img, cop, img.size());
    addWeighted(img, peso, cop, 1.0-peso, 0, img);
    crear_nueva(nueva, img);
}

//---------------------------------------------------------------------------

string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------

void ver_histograma(int nfoto, int nres, int canal)
{
    QImage imq= QImage(":/imagenes/histbase.png");
    Mat imghist(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imghist, imghist, COLOR_RGBA2RGB); // quitamos el canal alfa

    Mat gris;
    Mat hist;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    int canales[1] = {0};
    int bins[1] = {256};
    float rango[2] = {0, 256};
    const float *rangos[]= {rango};
    if(canal == 3)
        calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    else
        calcHist(&(foto[nfoto].img), 1, &canal, noArray(), hist, 1, bins, rangos);
    double vmin, vmax;
    minMaxLoc(hist, &vmin, &vmax);
    for (int i= 0; i<256; i++){
         float valor = hist.at<float>(i);
         rectangle(imghist, Point(3+i*391/256,185), Point(3+(i+1)+391/256, 185-valor*182/vmax), CV_RGB(canal==2?255:0, canal==1?255:0, canal==0?255:0), -1);
    }

    crear_nueva(nres, imghist);
}

//---------------------------------------------------------------------------

void nueva_portapapeles()
{
    QClipboard* clip = QApplication::clipboard();
    QImage imagen = clip->image(QClipboard::Clipboard);
    Mat mat = Mat(imagen.height(), imagen.width(), CV_8UC4, imagen.scanLine(0));

    int pl= primera_libre();
    if (pl != -1) {
        crear_nueva(pl, mat.clone());
    }
}

//---------------------------------------------------------------------------

void copiar_en_portapapeles(Mat imagen)
{
    QClipboard* clip = QApplication::clipboard();

    QImage img((unsigned char*) imagen.data, imagen.cols, imagen.rows, imagen.step, QImage::Format_RGBA8888);

    clip->setImage(img, QClipboard::Clipboard);
}

//---------------------------------------------------------------------------

void color_falso(int nfoto, int ncolor, bool guardar)
{
    Mat gris, res;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    applyColorMap(gris, res, ncolor);

    imshow(foto[nfoto].nombre, res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void ajustar_rojo_verde_azul(int nfoto,int Rop, int Rval, int Gop, int Gval, int Bop, int Bval, bool previsualizar, bool guardar){
    Mat img= foto[nfoto].img.clone();
    Mat res;
    Mat ch1, ch2, ch3;
    Mat channels[3];
    split(img, channels);
    // get the channels (dont forget they follow BGR order in OpenCV)
    ch1 = channels[0];
    ch2 = channels[1];
    ch3 = channels[2];

    //Rojo
    if(Rop==0){
        //Sum
        ch3+=Rval;
    }else{
        //Mult
        ch3*=Rval;
    }

    //Verde
    if(Gop==0){
        //Sum
        ch2+=Gval;
    }else{
        //Mult
        ch2*=Gval;
    }

    //Azul
    if(Bop==0){
        //Sum
        ch1+=Bval;
    }else{
        //Mult
        ch1*=Bval;
    }

    merge(channels, 3, res);

    if(previsualizar)
        imshow(foto[nfoto].nombre,res);
    if(guardar){
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

void modelos_de_color(int nfoto,int pos, bool guardar){
    Mat img = foto[nfoto].img;
    Mat res;
    switch (pos) {
    case 0:
        cvtColor(img,res,COLOR_BGR2RGB);//4
        break;
    case 1:
        cvtColor(img,res,COLOR_BGR2HLS);//52
        break;
    case 2:
        cvtColor(img,res,COLOR_BGR2HSV);//40
        break;
    case 3:
        cvtColor(img,res,COLOR_BGR2XYZ);//32
        break;
    case 4:
        cvtColor(img,res,COLOR_BGR2YUV);//82
        break;
    case 5:
        cvtColor(img,res,COLOR_BGR2Lab);//44
        break;
    case 6:
        cvtColor(img,res,COLOR_BGR2Luv);//50
        break;
    case 7:
        cvtColor(img,res,COLOR_BGR2GRAY);//6
        break;
    }
    imshow(foto[nfoto].nombre, res);
    if (guardar) {
            res.copyTo(foto[nfoto].img);
            foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------
