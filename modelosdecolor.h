#ifndef MODELOSDECOLOR_H
#define MODELOSDECOLOR_H

#include <QDialog>

namespace Ui {
class modelosDeColor;
}

class modelosDeColor : public QDialog
{
    Q_OBJECT

public:
    explicit modelosDeColor(int numfoto,
                            QWidget *parent = nullptr);
    ~modelosDeColor();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_modelosDeColor_accepted();

    void on_modelosDeColor_rejected();

private:
    Ui::modelosDeColor *ui;
    int nfoto;
    int pos;
};

#endif // MODELOSDECOLOR_H
