#include "ajustecolor.h"
#include "ui_ajustecolor.h"

#include "imagenes.h"

AjusteColor::AjusteColor(int numfoto,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AjusteColor)
{
    ui->setupUi(this);
    nfoto = numfoto;

    set_callback_foto(nfoto, false);
}

AjusteColor::~AjusteColor()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void AjusteColor::aplicarImagen(bool guardar)
{
    int Rop = ui->radioButton->isChecked() ? 0 : 1,
        Rval = ui->spinBox_r->value(),
        Gop = ui->radioButton_3->isChecked() ? 0 : 1,
        Gval = ui->spinBox_g->value(),
        Bop = ui->radioButton_6->isChecked() ? 0 : 1,
        Bval = ui->spinBox_b->value();

    ajustar_rojo_verde_azul(nfoto,
                            Rop, Rval, Gop, Gval, Bop, Bval, previsualizar, guardar);
}

void AjusteColor::on_radioButton_toggled(bool checked)
{
    // Sumar
    if(checked){
        ui->horizontalSlider_r->setMinimum(-256);
        ui->horizontalSlider_r->setMaximum(256);
        ui->horizontalSlider_r->setValue(0);

        ui->spinBox_r->setMinimum(-256);
        ui->spinBox_r->setMaximum(256);
        ui->spinBox_r->setValue(0);
    }
    // Multiplicar
    else {
        ui->horizontalSlider_r->setMinimum(0);
        ui->horizontalSlider_r->setMaximum(255);
        ui->horizontalSlider_r->setValue(1);

        ui->spinBox_r->setMinimum(0);
        ui->spinBox_r->setMaximum(255);
        ui->spinBox_r->setValue(1);
    }
    aplicarImagen();
}

void AjusteColor::on_radioButton_3_toggled(bool checked)
{
    // Sumar
    if(checked){
        ui->horizontalSlider_g->setMinimum(-256);
        ui->horizontalSlider_g->setMaximum(256);
        ui->horizontalSlider_g->setValue(0);

        ui->spinBox_g->setMinimum(-256);
        ui->spinBox_g->setMaximum(256);
        ui->spinBox_g->setValue(0);
    }
    // Multiplicar
    else {
        ui->horizontalSlider_g->setMinimum(0);
        ui->horizontalSlider_g->setMaximum(255);
        ui->horizontalSlider_g->setValue(1);

        ui->spinBox_g->setMinimum(0);
        ui->spinBox_g->setMaximum(255);
        ui->spinBox_g->setValue(1);
    }
    aplicarImagen();
}

void AjusteColor::on_radioButton_6_toggled(bool checked)
{
    // Sumar
    if(checked){
        ui->horizontalSlider_b->setMinimum(-256);
        ui->horizontalSlider_b->setMaximum(256);
        ui->horizontalSlider_b->setValue(0);

        ui->spinBox_b->setMinimum(-256);
        ui->spinBox_b->setMaximum(256);
        ui->spinBox_b->setValue(0);
    }
    // Multiplicar
    else {
        ui->horizontalSlider_b->setMinimum(0);
        ui->horizontalSlider_b->setMaximum(255);
        ui->horizontalSlider_b->setValue(1);

        ui->spinBox_b->setMinimum(0);
        ui->spinBox_b->setMaximum(255);
        ui->spinBox_b->setValue(1);
    }
    aplicarImagen();
}

void AjusteColor::on_horizontalSlider_r_valueChanged(int value)
{
    ui->spinBox_r->setValue(value);
    aplicarImagen();
}

void AjusteColor::on_horizontalSlider_g_valueChanged(int value)
{
    ui->spinBox_g->setValue(value);
    aplicarImagen();
}

void AjusteColor::on_horizontalSlider_b_valueChanged(int value)
{
    ui->spinBox_b->setValue(value);
    aplicarImagen();
}

void AjusteColor::on_spinBox_r_valueChanged(int arg1)
{
    ui->horizontalSlider_r->setValue(arg1);
}

void AjusteColor::on_spinBox_g_valueChanged(int arg1)
{
    ui->horizontalSlider_g->setValue(arg1);
}

void AjusteColor::on_spinBox_b_valueChanged(int arg1)
{
    ui->horizontalSlider_b->setValue(arg1);
}

void AjusteColor::on_AjusteColor_accepted()
{
    aplicarImagen(true);
}

void AjusteColor::on_checkBox_toggled(bool checked)
{
    previsualizar=checked;

    if(previsualizar)
        aplicarImagen();
    else
        imshow(foto[nfoto].nombre, foto[nfoto].img);
}

void AjusteColor::on_AjusteColor_rejected()
{
    imshow(foto[nfoto].nombre, foto[nfoto].img);
}
