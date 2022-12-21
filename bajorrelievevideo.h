#ifndef BAJORRELIEVEVIDEO_H
#define BAJORRELIEVEVIDEO_H

#include <QDialog>

namespace Ui {
class bajorrelieveVideo;
}

class bajorrelieveVideo : public QDialog
{
    Q_OBJECT

public:
    explicit bajorrelieveVideo(QWidget *parent = nullptr);
    ~bajorrelieveVideo();

private slots:
    void on_bajorrelieveVideo_accepted();

private:
    Ui::bajorrelieveVideo *ui;
    double angulo;
    double grado;
    int fondo;
};

#endif // BAJORRELIEVEVIDEO_H
