#include "informacion.h"
#include "ui_informacion.h"

Informacion::Informacion(Mat img,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Informacion)
{
    ui->setupUi(this);

    int ancho = img.cols;
    int alto = img.rows;

    int canales = img.channels()+1;

    int profundidad = 0;
    switch (img.depth())
        {
            case 0:
            case 1:
                profundidad = 8;
                break;
            case 2:
            case 3:
                profundidad = 16;
                break;
            case 4:
            case 5:
                profundidad = 32;
                break;
            case 6:
                profundidad = 64;
                break;
            default:
                break;
        }
    profundidad *= canales;

    int memoria = img.elemSize()*ancho*alto;

    Scalar color_medio = mean(img);
    QString color_medio_hex="0x";
    for(int i=0; i<=canales; i++){
        color_medio_hex.append(QString("%1").arg((int) color_medio[canales-i], 0, 16));
    }

    // Poner la informacion en el cuadro de texto
    ui->dimensiones->setText(QString(QString::number(ancho)+"x"+QString::number(alto)));
    ui->canales->setText(QString::number(canales));
    ui->profundidad->setText(QString::number(profundidad));
    ui->memoria->setText(QString::number(memoria));
    ui->color_medio->setText(color_medio_hex);
}

Informacion::~Informacion()
{
    delete ui;
}
