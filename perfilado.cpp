#include "perfilado.h"
#include "ui_perfilado.h"

#include "imagenes.h"

Perfilado::Perfilado(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Perfilado)
{
    ui->setupUi(this);
    nfoto = numfoto;
    radio = 1;
    grado = 1.0;

    ver_perfilado(nfoto, grado, radio);

    set_callback_foto(nfoto, false);
}

Perfilado::~Perfilado()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void Perfilado::on_horizontalSlider_valueChanged(int value)
{
    radio = value*2+1;
    ui->spinBox->setValue(value);
    ver_perfilado(nfoto, grado, radio);
}

void Perfilado::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void Perfilado::on_horizontalSlider_2_valueChanged(int value)
{
    grado = value/100.0;
    ui->spinBox_2->setValue(value);
    ver_perfilado(nfoto, grado, radio);
}

void Perfilado::on_spinBox_2_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void Perfilado::on_Perfilado_accepted()
{
    ver_perfilado(nfoto, grado, radio, true);
}

void Perfilado::on_Perfilado_rejected()
{
    mostrar(nfoto);
}
