#include "modelosdecolor.h"
#include "ui_modelosdecolor.h"

#include "imagenes.h"

modelosDeColor::modelosDeColor(int numfoto,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modelosDeColor)
{
    ui->setupUi(this);
    nfoto = numfoto;
    pos = 0;

    set_callback_foto(nfoto, false);
}

modelosDeColor::~modelosDeColor()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void modelosDeColor::on_comboBox_currentIndexChanged(int index)
{
    pos = index;
    modelos_de_color(nfoto, pos);
}

void modelosDeColor::on_modelosDeColor_accepted()
{
    modelos_de_color(nfoto, pos, true);
}

void modelosDeColor::on_modelosDeColor_rejected()
{
    imshow(foto[nfoto].nombre, foto[nfoto].img);
}
