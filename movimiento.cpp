#include "movimiento.h"
#include "ui_movimiento.h"


Movimiento::Movimiento(QString nombref,
            int numres,
            QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Movimiento)
{
    ui->setupUi(this);
    nombre = nombref;
    nres = numres;
    framei = 0;
    cap.open(nombre.toLatin1().data());
    if(cap.isOpened()) {
        int ntotal = cap.get(CAP_PROP_FRAME_COUNT);
        ui->horizontalSlider->setMaximum(ntotal-2);
        ui->horizontalSlider_2->setMaximum(ntotal-1);
        ui->horizontalSlider->setValue(0);
        ui->horizontalSlider_2->setValue(ntotal-1);
        framef = ntotal-1;

    }
}

Movimiento::~Movimiento()
{
    delete ui;
}

void Movimiento::on_horizontalSlider_valueChanged(int value)
{
    framei = value;
    if(framei >= framef)
        ui->horizontalSlider_2->setValue(framei+1);
    Mat frame;
    if (cap.read(frame))
        imshow("Vídeo", frame);
}

void Movimiento::on_horizontalSlider_2_valueChanged(int value)
{
    framef = value;
    if(framef <= framei)
        ui->horizontalSlider->setValue(framef-1);
    Mat frame;
    if (cap.read(frame))
        imshow("Vídeo", frame);
}

bool Movimiento::isOpened()
{
    return cap.isOpened();
}

void Movimiento::on_Movimiento_accepted()
{
    movimiento(nombre.toLatin1().data(), framei, framef, nres);
}

void Movimiento::on_Movimiento_rejected()
{
    destroyWindow("Vídeo");
}
