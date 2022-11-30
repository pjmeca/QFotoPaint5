#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include <QDialog>
#include "video.h"

namespace Ui {
class Movimiento;
}

class Movimiento : public QDialog
{
    Q_OBJECT

public:
    explicit Movimiento(QString nombref,
                        int numres,
                        QWidget *parent = nullptr);
    ~Movimiento();
    bool isOpened();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_Movimiento_accepted();

    void on_Movimiento_rejected();

private:
    Ui::Movimiento *ui;
    VideoCapture cap;
    QString nombre;
    int framei;
    int framef;
    int nres;
};

#endif // MOVIMIENTO_H
