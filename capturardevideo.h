#ifndef CAPTURARDEVIDEO_H
#define CAPTURARDEVIDEO_H

#include <QDialog>
#include "imagenes.h"

namespace Ui {
class Capturardevideo;
}

class Capturardevideo : public QDialog
{
    Q_OBJECT

public:
    explicit Capturardevideo(QString nombre,
                             QWidget *parent = nullptr);
    ~Capturardevideo();
    bool isOpened();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

    void on_Capturardevideo_rejected();

    void on_Capturardevideo_accepted();

private:
    Ui::Capturardevideo *ui;
    VideoCapture cap;
};

#endif // CAPTURARDEVIDEO_H
