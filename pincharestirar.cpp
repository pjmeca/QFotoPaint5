#include "pincharestirar.h"
#include "ui_pincharestirar.h"

#include "imagenes.h"

int nfoto;
int cx, cy;
double radio, grado;

void callback_pinchar(int evento, int x, int y, int flags, void *p)
{
    if (flags == EVENT_FLAG_LBUTTON) {
        cx = x;
        cy = y;

        ver_pinchar_estirar(nfoto, cx, cy, radio, grado);
    }
}

PincharEstirar::PincharEstirar(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PincharEstirar)
{
    ui->setupUi(this);
    nfoto = numfoto;
    cx = cy = 200;
    radio = 200;
    grado = radio*radio*30/1000.0;
    ver_pinchar_estirar(nfoto, cx, cy, radio, grado);
    setMouseCallback("Pinchar/estirar", callback_pinchar);
}

PincharEstirar::~PincharEstirar()
{
    delete ui;
}

void PincharEstirar::on_horizontalSlider_valueChanged(int value)
{
    grado = radio*radio*value/1000.0;
    ui->spinBox->setValue(value);
    ver_pinchar_estirar(nfoto, cx, cy, radio, grado);
}

void PincharEstirar::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void PincharEstirar::on_horizontalSlider_2_valueChanged(int value)
{
    radio = value;
    grado = radio*radio*ui->horizontalSlider->value()/1000.0;
    ui->spinBox_2->setValue(value);
    ver_pinchar_estirar(nfoto, cx, cy, radio, grado);
}

void PincharEstirar::on_spinBox_2_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void PincharEstirar::on_PincharEstirar_accepted()
{
    ver_pinchar_estirar(nfoto, cx, cy, radio, grado, true);
    destroyWindow("Pinchar/estirar");
}

void PincharEstirar::on_PincharEstirar_rejected()
{
    destroyWindow("Pinchar/estirar");
}
