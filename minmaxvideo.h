#ifndef MINMAXVIDEO_H
#define MINMAXVIDEO_H

#include <QDialog>
#include "video.h"

namespace Ui {
class minMaxVideo;
}

class minMaxVideo : public QDialog
{
    Q_OBJECT

public:
    explicit minMaxVideo(QString nombref,
                         int numres1,
                         int numres2,
                         QWidget *parent = nullptr);
    ~minMaxVideo();
    bool isOpened();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_minMaxVideo_accepted();

    void on_minMaxVideo_rejected();

    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

private:
    Ui::minMaxVideo *ui;
    VideoCapture cap;
    QString nombre;
    int framei;
    int framef;
    int nres1, nres2;
};

#endif // MINMAXVIDEO_H
