#include "matsatlum.h"
#include "ui_matsatlum.h"

#include "imagenes.h"

MatSatLum::MatSatLum(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatSatLum)
{
    ui->setupUi(this);
    nfoto = numfoto;
    matiz = 0;
    lum = sat = 1.0;
    set_callback_foto(nfoto, false);
}

MatSatLum::~MatSatLum()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void MatSatLum::on_horizontalSlider_valueChanged(int value)
{
    sat = value/100.0;
    ui->spinBox->setValue(value);
    ver_matsatlum(nfoto, matiz, sat, lum);
}

void MatSatLum::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void MatSatLum::on_horizontalSlider_2_valueChanged(int value)
{
    lum = value/100.0;
    ui->spinBox_2->setValue(value);
    ver_matsatlum(nfoto, matiz, sat, lum);
}

void MatSatLum::on_spinBox_2_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void MatSatLum::on_dial_valueChanged(int value)
{
    matiz = value;
    ver_matsatlum(nfoto, matiz, sat, lum);
}

void MatSatLum::on_MatSatLum_accepted()
{
    ver_matsatlum(nfoto, matiz, sat, lum, true);
}

void MatSatLum::on_MatSatLum_rejected()
{
    mostrar(nfoto);
}
