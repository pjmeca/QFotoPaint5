#ifndef TRANSICIONIMAGENES_H
#define TRANSICIONIMAGENES_H

#include <QDialog>

#include "imagenes.h"

namespace Ui {
class TransicionImagenes;
}

class TransicionImagenes : public QDialog
{
    Q_OBJECT

public:
    explicit TransicionImagenes(QWidget *parent = nullptr);
    ~TransicionImagenes();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_buttonBox_accepted();

private:
    Ui::TransicionImagenes *ui;
    int nfoto1, nfoto2;
    Mat imgUno, imgDos, imgRes;
    int corresp[MAX_VENTANAS];
};

#endif // TRANSICIONIMAGENES_H
