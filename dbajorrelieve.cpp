#include "dbajorrelieve.h"
#include "ui_dbajorrelieve.h"

#include "imagenes.h"

Dbajorrelieve::Dbajorrelieve(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dbajorrelieve)
{
    ui->setupUi(this);
       nfoto = numfoto;
       angulo = 0;
       grado = 1.0;
       fondo = 0;
       ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

Dbajorrelieve::~Dbajorrelieve()
{
    delete ui;
}

void Dbajorrelieve::on_horizontalSlider_valueChanged(int value)
{
    grado = value/100.0;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_dial_valueChanged(int value)
{
    angulo = value;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_radioButton_clicked()
{
    fondo = 0;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_radioButton_2_clicked()
{
    fondo = 1;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_radioButton_3_clicked()
{
    fondo = 2;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_radioButton_4_clicked()
{
    fondo = 3;
    ver_bajorrelieve(nfoto, angulo, grado, fondo);
}

void Dbajorrelieve::on_Dbajorrelieve_accepted()
{
    ver_bajorrelieve(nfoto, angulo, grado, fondo, true);
    destroyWindow("Bajorrelieve");
}

void Dbajorrelieve::on_Dbajorrelieve_rejected()
{
    destroyWindow("Bajorrelieve");
}
