#include "ajustelineal.h"
#include "ui_ajustelineal.h"
#include "imagenes.h"

Ajustelineal::Ajustelineal(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ajustelineal)
{
    ui->setupUi(this);
    nfoto = numfoto;
    pmin = 5;
    pmax = 5;
    ver_ajuste_lineal(nfoto, pmin, pmax);
    set_callback_foto(nfoto, false);

}

Ajustelineal::~Ajustelineal()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void Ajustelineal::on_horizontalSlider_valueChanged(int value)
{
    pmin = value;
    if(pmin+pmax>100){
        pmax=100-pmin;
        on_horizontalSlider_2_valueChanged(pmax);
    }
    ui->spinBox->setValue(value);
    if(ui->checkBox->isChecked())
        ver_ajuste_lineal(nfoto, pmin, pmax);
}

void Ajustelineal::on_spinBox_valueChanged(int arg1)
{
    pmin = arg1;
    ui->horizontalSlider->setValue(arg1);
    if(ui->checkBox->isChecked())
        ver_ajuste_lineal(nfoto, pmin, pmax);
}

void Ajustelineal::on_horizontalSlider_2_valueChanged(int value)
{
    pmax = value;
    if(pmin+pmax>100){
        pmin = 100-pmax;
        on_horizontalSlider_valueChanged(pmin);
    }
    ui->spinBox_2->setValue(value);
    if(ui->checkBox->isChecked())
        ver_ajuste_lineal(nfoto, pmin, pmax);
}

void Ajustelineal::on_spinBox_2_valueChanged(int arg1)
{
    pmax = arg1;
    ui->horizontalSlider_2->setValue(arg1);
    if(ui->checkBox->isChecked())
        ver_ajuste_lineal(nfoto, pmin, pmax);
}

void Ajustelineal::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
        ver_ajuste_lineal(nfoto, pmin, pmax);
    else {
        mostrar(nfoto);
    }
}

void Ajustelineal::on_Ajustelineal_accepted()
{
    ver_ajuste_lineal(nfoto, pmin, pmax, true);
}

void Ajustelineal::on_Ajustelineal_rejected()
{
    mostrar(nfoto);
}
