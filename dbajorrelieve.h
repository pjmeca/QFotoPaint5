#ifndef DBAJORRELIEVE_H
#define DBAJORRELIEVE_H

#include <QDialog>

namespace Ui {
class Dbajorrelieve;
}

class Dbajorrelieve : public QDialog
{
    Q_OBJECT

public:
    explicit Dbajorrelieve(int numfoto, QWidget *parent = nullptr);
    ~Dbajorrelieve();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_dial_valueChanged(int value);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_Dbajorrelieve_accepted();

    void on_Dbajorrelieve_rejected();

private:
    int nfoto;
    double angulo;
    double grado;
    int fondo;
    Ui::Dbajorrelieve *ui;
};

#endif // DBAJORRELIEVE_H
