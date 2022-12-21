#include "colorfalso.h"
#include "ui_colorfalso.h"

#include "imagenes.h"

colorFalso::colorFalso(int numfoto,
                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colorFalso)
{
    ui->setupUi(this);
    nfoto = numfoto;
    ncolor = 0;

    color_falso(nfoto, ncolor, false);

    set_callback_foto(nfoto, false);
}

colorFalso::~colorFalso()
{
    delete ui;
    set_callback_foto(nfoto, true);
}

void colorFalso::on_comboBox_currentIndexChanged(int index)
{
    ncolor = index;
    color_falso(nfoto, index, false);
}

void colorFalso::on_colorFalso_accepted()
{
    color_falso(nfoto, ncolor, true);
}

void colorFalso::on_colorFalso_rejected()
{
    imshow(foto[nfoto].nombre, foto[nfoto].img);
}
