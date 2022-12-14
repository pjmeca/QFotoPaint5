#include "minmaxvideo.h"
#include "ui_minmaxvideo.h"

minMaxVideo::minMaxVideo(QString nombref,
                         int numres1,
                         int numres2,
                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::minMaxVideo)
{
  ui->setupUi(this);
  nombre = nombref;
  nres1 = numres1;
  nres2 = numres2;
  framei = 0;
  cap.open(nombre.toLatin1().data());
  if(cap.isOpened()) {
      int ntotal = cap.get(CAP_PROP_FRAME_COUNT);
      ui->horizontalSlider->setMaximum(ntotal-2);
      ui->horizontalSlider_2->setMaximum(ntotal-1);
      ui->horizontalSlider->setValue(0);
      ui->horizontalSlider_2->setValue(ntotal-1);
      framef = ntotal-1;

  }
}

minMaxVideo::~minMaxVideo()
{
    delete ui;
}

bool minMaxVideo::isOpened()
{
    return cap.isOpened();
}

void minMaxVideo::on_horizontalSlider_valueChanged(int value)
{
    framei = value;
    if(framei >= framef)
        ui->horizontalSlider_2->setValue(framei+1);
    Mat frame;
    if (cap.read(frame))
        imshow("Vídeo", frame);
}

void minMaxVideo::on_horizontalSlider_2_valueChanged(int value)
{
    framef = value;
    if(framef <= framei)
        ui->horizontalSlider->setValue(framef-1);
    Mat frame;
    if (cap.read(frame))
        imshow("Vídeo", frame);
}

void minMaxVideo::on_minMaxVideo_accepted()
{
    minMax(nombre.toLatin1().data(), framei, framef, nres1, nres2);
}

void minMaxVideo::on_minMaxVideo_rejected()
{
    destroyWindow("Vídeo");
}

void minMaxVideo::on_buttonBox_2_accepted()
{
    on_minMaxVideo_accepted();
}

void minMaxVideo::on_buttonBox_2_rejected()
{
    close();
}
