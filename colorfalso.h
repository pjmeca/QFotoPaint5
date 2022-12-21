#ifndef COLORFALSO_H
#define COLORFALSO_H

#include <QDialog>

namespace Ui {
class colorFalso;
}

class colorFalso : public QDialog
{
    Q_OBJECT

public:
    explicit colorFalso(int numfoto,
                        QWidget *parent = nullptr);
    ~colorFalso();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_colorFalso_accepted();

    void on_colorFalso_rejected();

private:
    Ui::colorFalso *ui;
    int nfoto;
    int ncolor;
};

#endif // COLORFALSO_H
