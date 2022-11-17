#include "perspectiva.h"
#include "ui_perspectiva.h"

#include "imagenes.h"

int corresp[MAX_VENTANAS];
int nfoto1, nfoto2;
Point2f pt[2][4];

void pintar_esquinas()
{
    Mat cop = foto[nfoto1].img.clone();
    for(int i=0; i<4; i++)
        line(cop, pt[0][i], pt[0][(i+1)%4], CV_RGB(0,0,0), 2);
    for(int i=0; i<4; i++)
        circle(cop, pt[0][i], 10, CV_RGB(0, 255, 0), -1);
    imshow("Origen", cop);
}

void cb_perspectiva(int evento, int x, int y, int flags, void *p)
{
    if(flags == EVENT_FLAG_LBUTTON){
        int n = (int) p;
        int dmin = 200;
        int pmin = 0;
        for (int i=0; i<4; i++){
            int dact = abs(x - pt[n][i].x) + abs(y - pt[n][i].y);
            if (dact < dmin) {
                dmin = dact;
                pmin = i;
            }
        }
        if (dmin < 200) {
            pt[n][pmin].x = x;
            pt[n][pmin].y = y;
            if(n == 0)
                pintar_esquinas();
            ver_perspectiva(nfoto1, nfoto2, pt[0], pt[1]);
        }
    }
}

Perspectiva::Perspectiva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Perspectiva)
{
    ui->setupUi(this);
    int pos= 0;
    for(int i=0; i<MAX_VENTANAS; i++)
        if (foto[i].usada){
            ui->listWidget->addItem(QString::fromStdString(foto[i].nombre));
            ui->listWidget_2->addItem(QString::fromStdString(foto[i].nombre));
            corresp[pos++]= i;
        }
    nfoto1 = corresp[0];
    nfoto2 = foto_activa();
    int W = foto[nfoto1].img.cols;
    int H = foto[nfoto1].img.rows;
    pt[0][0] = Point2f(0,0);
    pt[0][1] = Point2f(W, 0);
    pt[0][2] = Point2f(W,H);
    pt[0][3] = Point2f(0,H);
    W = foto[nfoto2].img.cols;
    H = foto[nfoto2].img.rows;
    pt[1][0] = Point2f(0,0);
    pt[1][1] = Point2f(W, 0);
    pt[1][2] = Point2f(W,H);
    pt[1][3] = Point2f(0,H);
    pintar_esquinas();
    ver_perspectiva(nfoto1, nfoto2, pt[0], pt[1]);
    setMouseCallback("Origen", cb_perspectiva, (void*) 0);
    setMouseCallback("Perspectiva", cb_perspectiva, (void*) 1);
}

Perspectiva::~Perspectiva()
{
    delete ui;
}

void Perspectiva::on_Perspectiva_accepted()
{
    ver_perspectiva(nfoto1, nfoto2, pt[0], pt[1], true);
    destroyWindow("Origen");
    destroyWindow("Perspectiva");
}

void Perspectiva::on_Perspectiva_rejected()
{
    destroyWindow("Origen");
    destroyWindow("Perspectiva");
}

void Perspectiva::on_listWidget_currentRowChanged(int currentRow)
{
    nfoto1 = corresp[currentRow];
    pintar_esquinas();
    ver_perspectiva(nfoto1, nfoto2, pt[0], pt[1]);
}

void Perspectiva::on_listWidget_2_currentRowChanged(int currentRow)
{
    nfoto2 = corresp[currentRow];
    ver_perspectiva(nfoto1, nfoto2, pt[0], pt[1]);
}
