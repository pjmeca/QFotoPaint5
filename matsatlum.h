#ifndef MATSATLUM_H
#define MATSATLUM_H

#include <QDialog>

namespace Ui {
class MatSatLum;
}

class MatSatLum : public QDialog
{
    Q_OBJECT

public:
    explicit MatSatLum(int numfoto, QWidget *parent = nullptr);
    ~MatSatLum();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_spinBox_2_valueChanged(int arg1);

    void on_dial_valueChanged(int value);

    void on_MatSatLum_accepted();

    void on_MatSatLum_rejected();

private:
    Ui::MatSatLum *ui;
    int nfoto;
    int matiz;
    double sat, lum;
};

#endif // MATSATLUM_H
