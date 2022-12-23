//---------------------------------------------------------------------------

#include "video.h"
#include <math.h>

#include <QDebug>
#include <QImage>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES PRIVADAS               //////////////
///////////////////////////////////////////////////////////////////

static VideoCapture camara;
// Capturador de cámara usado actualmente

static Mat img_media;
// Imagen media acumulada de la cámara, con profundidad 32F

static int frames_img_media;
// Número de frames que se han acumulado en la media img_media

string FiltroVideo= "Todos los formatos (*.avi *.mpg *.wmv *.mov);;Archivos AVI (*.avi);;Archivos MPG (*.mpg *.mpeg);;Archivos WMV (*.wmv);;Archivos MOV (*.mov);;Otros (*.*)";

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE VIDEO       //////////////
///////////////////////////////////////////////////////////////////

void rotar_video (int nfoto, string nombre, int modo, int nframes, int codec, double fps)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada && !nombre.empty() && nframes>0);
    Mat rotada;
    rotar_angulo(foto[nfoto].img, rotada, 0, 1, modo);
    VideoWriter writer(nombre, codec, fps, rotada.size());
    if (writer.isOpened()) {
        for (int i= 0; i<=nframes; i++) {
            rotar_angulo(foto[nfoto].img, rotada, i*360.0/nframes, 1, modo);
            namedWindow("Imagen a vídeo - Rotación", WINDOW_NORMAL);
            imshow("Imagen a vídeo - Rotación", rotada);
            waitKey(1);
            writer << rotada;
        }
        writer.release();
        destroyWindow("Imagen a vídeo - Rotación");
    }
}

//---------------------------------------------------------------------------

int inic_camara (int numero)
{
    if (camara.isOpened())
        camara.release();
    camara.open(numero);
    if (camara.isOpened()) {
        Mat img;
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        camara >> img;
        resizeWindow("Imagen de cámara", img.size().width, img.size().height);
        imshow("Imagen de cámara", img);
        return int(1000.0/30);
    }
    else
        return 0;
}

//---------------------------------------------------------------------------

void fin_camara (void)
{
    if (camara.isOpened()) {
        camara.release();
        namedWindow("Imagen de cámara", WINDOW_NORMAL);
        destroyWindow("Imagen de cámara");
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    destroyWindow("Imagen media");
}

//---------------------------------------------------------------------------

void acumular_media (bool primera)
{
    assert(camara.isOpened());
    Mat frame;
    camara >> frame;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", frame);
    if (primera) {
        frames_img_media= 1;
        frame.convertTo(img_media, CV_32FC3);
    }
    else {
        frames_img_media++;
        Mat frame32F;
        frame.convertTo(frame32F, CV_32FC3);
        img_media= frame32F + img_media;
        img_media.convertTo(frame, CV_8UC3, 1.0/frames_img_media);
    }
    namedWindow("Imagen media", WINDOW_NORMAL);
    imshow("Imagen media", frame);
}

//---------------------------------------------------------------------------

void movimiento(string nombre, int framei, int framef,
                int nres)
{
    VideoCapture cap(nombre);
    if (cap.isOpened()) {
        cap.set(CAP_PROP_POS_FRAMES, framei);
        Mat frame;
        if (cap.read(frame)) {
            Mat acum(frame.size(), CV_32SC3, Scalar(0,0,0));
            Mat frame_ant = frame.clone();
            Mat dif, dif32;
            Mat acum8u;
            framei++;
            while (framei <= framef && cap.read(frame) && waitKey(1)==-1) {
                absdiff(frame, frame_ant, dif);
                dif.convertTo(dif32, CV_32S);
                acum += dif32;
                frame.copyTo(frame_ant);
                imshow("Vídeo", frame);
                normalize(acum, acum8u, 0, 255, NORM_MINMAX, CV_8U);
                imshow("Acumulado", acum8u);
                framei++;
            }
            crear_nueva(nres, acum8u);
            destroyWindow("Vídeo");
            destroyWindow("Acumulado");
        }
    }
}

//---------------------------------------------------------------------------

void minMax(string nombre, int framei, int framef,
                int nres1, int nres2)
{
    VideoCapture cap(nombre);
    if (cap.isOpened()) {
        cap.set(CAP_PROP_POS_FRAMES, framei);
        Mat frame;
        if (cap.read(frame)) {

            Mat maxFrame = frame.clone();
            Mat minFrame = frame.clone();
            Mat dif;
            Mat maxFrame8u, minFrame8u;
            Mat aux;
            framei++;

            while (framei <= framef && cap.read(frame) && waitKey(1)==-1) {

                maxFrame.convertTo(aux, frame.type());
                max(frame, aux, dif);
                dif.convertTo(maxFrame, CV_32S);
                maxFrame.convertTo(maxFrame8u, CV_8U);

                minFrame.convertTo(aux, frame.type());
                min(frame, aux, dif);
                dif.convertTo(minFrame, CV_32S);
                minFrame.convertTo(minFrame8u, CV_8U);

                imshow("Valor máximo", maxFrame8u);
                imshow("Valor mínimo", minFrame8u);

                framei++;
            }
            crear_nueva(nres1, maxFrame8u);
            crear_nueva(nres2, minFrame8u);

            destroyWindow("Valor máximo");
            destroyWindow("Valor mínimo");
        }
    }
}

//----------------------------------------------------------------------------

void suavizadoTemporalVideo(string nombreVideoOriginal, string nombreVideoSalida, int n)
{
    VideoCapture cap(nombreVideoOriginal);
    if (cap.isOpened()) {
        int framei=0;
        cap.set(CAP_PROP_POS_FRAMES, 1);
        Mat frame;
        Mat *nAnteriores = new Mat[n];
        if (cap.read(frame)) {
           VideoWriter writer(nombreVideoSalida, cap.get(CAP_PROP_FOURCC), cap.get(CAP_PROP_FPS), frame.size());
           if(writer.isOpened()){
               int ntotal=0;
               nAnteriores[0]=frame.clone();
               ntotal++;
               framei++;
               while (cap.read(frame) && waitKey(1)==-1) {
                   Mat media=nAnteriores[0].clone()/n;
                   for(int i=1;i<ntotal;i++){
                       media+=nAnteriores[i]/n;
                   }
                   media.convertTo(media,CV_8U);
                   imshow("Suavizado media", media);
                   writer << media;
                   if(ntotal<n)
                       ntotal++;
                   nAnteriores[framei%n]=frame.clone();
                   framei++;
               }
               writer.release();
               destroyWindow("Suavizado media");
            }
        }
        delete[] nAnteriores;
    }
}

//---------------------------------------------------------------------------

void media_a_nueva (int nfoto)
{
    Mat res(img_media.size(), CV_8UC3);
    img_media.convertTo(res, CV_8UC3, 1.0/frames_img_media);
    crear_nueva (nfoto, res);
}

//---------------------------------------------------------------------------

void mostrar_camara (void)
{
    Mat img;
    camara >> img;
    namedWindow("Imagen de cámara", WINDOW_NORMAL);
    imshow("Imagen de cámara", img);
}

//---------------------------------------------------------------------------

void capturar_camara(int nres)
{
    VideoCapture cap(0);
    if (cap.isOpened()) {
        Mat imagen;
        int tecla = -1;
        cap.read(imagen);
        while((tecla=waitKey(1))==-1 && !imagen.empty()) {
            namedWindow("Pulse una tecla para capturar", 0);
            imshow("Pulse una tecla para capturar",imagen);
            cap.read(imagen);
        }
        if (tecla != 27 && !imagen.empty())
            crear_nueva(nres, imagen);
        destroyWindow("Pulse una tecla para capturar");
    } else {
        qDebug("No se ha podido abrir la cámara.");
    }
}

//---------------------------------------------------------------------------

void guardarFrame(VideoWriter writer, Mat frame, String nombre){
    imshow(nombre, frame);
    writer << frame;
    waitKey(1);
}

void transicion(int nf1, int nf2, String nombre, int framesImagen, int codec, double fps,int trans)
{
    assert(nf1>=0 && nf1<MAX_VENTANAS && foto[nf1].usada);
    assert(nf2>=0 && nf2<MAX_VENTANAS && foto[nf2].usada);
    Mat frame;
    Mat img1= foto[nf1].img.clone();
    Mat img2;
    resize(foto[nf2].img, img2, img1.size());
    VideoWriter writer(nombre, codec, fps, img1.size());

    int duracion = 3;

    if(writer.isOpened()){

        frame=img1.clone();
        for(int i=0;i<framesImagen;i++){
            guardarFrame(writer, frame, nombre);
        }

        switch(trans) {
        case 0 ://Media ponderada
        {
            int salto = 1000/(fps*duracion);
            if (salto == 0)
                salto = 1;
            for(int i =1;i<1000;i+=salto){
                frame=img2.clone();
                addWeighted(img1, (1000-i)/1000.0, frame, i/1000.0, 0, frame);
                guardarFrame(writer, frame, nombre);
            }
            break;
        }
        case 1 : //Barrido columnas
        {
            int salto = img1.cols/(fps*duracion);
            if (salto == 0)
                salto = 1;
            for(int i=0;i<img1.cols;i++){
                img2.col(i).copyTo(frame.col(i));
                if(i%salto == 0)
                    guardarFrame(writer, frame, nombre);
            }
            break;
        }
        case 2 : //Barrido filas
        {
            int salto = img1.rows/(fps*duracion);
            if (salto == 0)
                salto = 1;
            for(int i=0;i<img1.rows;i++){
                img2.row(i).copyTo(frame.row(i));
                if(i%salto == 0)
                    guardarFrame(writer, frame, nombre);
            }
            break;
        }
        }

        frame=img2.clone();
        for(int i=0;i<framesImagen;i++){
            guardarFrame(writer, frame, nombre);
        }

        destroyWindow(nombre);

        writer.release();
    }
}

//---------------------------------------------------------------------------

// CAP_PROP_FPS no funciona con cámaras, así que debemos calcular los fps manualmente
// implementación basada en: https://learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/
double getFpsCamara(VideoCapture video)
{
    // Number of frames to capture
    int num_frames = 60;

    // Start and end times
    time_t start, end;

    // Variable for storing video frames
    Mat frame;

    // Start time
    time(&start);

    // Grab a few frames
    for(int i = 0; i < num_frames; i++)
        video >> frame;

    // End Time
    time(&end);

    // Time elapsed
    double seconds = difftime (end, start);

    // Calculate frames per second
    return num_frames / seconds;
}

void bajorrelieve_video (String videoOriginal, String videoSalida, double angulo, double grado, int nfondo)
{
    QString nombres[4]={":/imagenes/arena.jpg",
                       ":/imagenes/cielo.jpg",
                       ":/imagenes/gris.png",
                       ":/imagenes/madera.jpg"};
    QImage imq= QImage(nombres[nfondo]);
    Mat imgfondo(imq.height(),imq.width(),CV_8UC4,imq.scanLine(0));
    cvtColor(imgfondo, imgfondo, COLOR_RGBA2RGB);

    VideoCapture cap; double fps_camara = 0;
    if(videoOriginal.empty()) {
        cap = VideoCapture(0); // cámara
        fps_camara = getFpsCamara(cap);
    } else
        cap = VideoCapture(videoOriginal); // vídeo

    if (cap.isOpened()) {
        if(!videoOriginal.empty())
            cap.set(CAP_PROP_POS_FRAMES, 1);
        Mat frame;
        if (cap.read(frame)) {
            VideoWriter writer(videoSalida, cap.get(CAP_PROP_FOURCC), videoOriginal.empty() ? fps_camara : cap.get(CAP_PROP_FPS), frame.size());
            resize(imgfondo, imgfondo, frame.size());

            Mat gris;
            cvtColor(frame, gris, COLOR_BGR2GRAY);
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

            imshow(videoSalida, res);
            writer << res;

            while(cap.read(frame) && !frame.empty() && waitKey(1) == -1) {
                Mat gris;
                cvtColor(frame, gris, COLOR_BGR2GRAY);
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

                imshow(videoSalida, res);
                writer << res;
            }

            writer.release();
        }
    } else {
        printf("No se pudo abrir el vídeo de origen.\n");
    }
}
