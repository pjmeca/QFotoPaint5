#ifndef PINCHARESTIRAR_H
#define PINCHARESTIRAR_H

#include <QDialog>

namespace Ui {
class PincharEstirar;
}

class PincharEstirar : public QDialog
{
    Q_OBJECT

public:
    explicit PincharEstirar(int numfoto, QWidget *parent = nullptr);
    ~PincharEstirar();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_spinBox_2_valueChanged(int arg1);

    void on_PincharEstirar_accepted();

    void on_PincharEstirar_rejected();

private:
    Ui::PincharEstirar *ui;
};

#endif // PINCHARESTIRAR_H
