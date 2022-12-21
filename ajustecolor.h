#ifndef AJUSTECOLOR_H
#define AJUSTECOLOR_H

#include <QDialog>

namespace Ui {
class AjusteColor;
}

class AjusteColor : public QDialog
{
    Q_OBJECT

public:
    explicit AjusteColor(int numfoto,
                         QWidget *parent = nullptr);
    ~AjusteColor();
    void aplicarImagen(bool guardar=false);

private slots:
    void on_radioButton_toggled(bool checked);

    void on_radioButton_3_toggled(bool checked);

    void on_radioButton_6_toggled(bool checked);

    void on_horizontalSlider_r_valueChanged(int value);

    void on_horizontalSlider_g_valueChanged(int value);

    void on_horizontalSlider_b_valueChanged(int value);

    void on_spinBox_r_valueChanged(int arg1);

    void on_spinBox_g_valueChanged(int arg1);

    void on_spinBox_b_valueChanged(int arg1);

    void on_AjusteColor_accepted();

    void on_checkBox_toggled(bool checked);

    void on_AjusteColor_rejected();

private:
    Ui::AjusteColor *ui;
    int nfoto;
    bool previsualizar = true;
};

#endif // AJUSTECOLOR_H
