#ifndef SUAVIZADOTEMPORAL_H
#define SUAVIZADOTEMPORAL_H

#include <QDialog>

namespace Ui {
class suavizadoTemporal;
}

class suavizadoTemporal : public QDialog
{
    Q_OBJECT

public:
    explicit suavizadoTemporal(QString nombVideo,
                               QWidget *parent = nullptr);
    ~suavizadoTemporal();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_suavizadoTemporal_accepted();

    void on_suavizadoTemporal_rejected();

private:
    Ui::suavizadoTemporal *ui;
    QString nombreVideoOriginal;
    int n;
};

#endif // SUAVIZADOTEMPORAL_H
