#include "capturardevideo.h"
#include "ui_capturardevideo.h"

Capturardevideo::Capturardevideo(QString nombre,
                                 QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Capturardevideo)
{
    ui->setupUi(this);
    cap.open(nombre.toLatin1().data());
    if(cap.isOpened()){
        int nframes = cap.get(CAP_PROP_FRAME_COUNT);
        ui->horizontalSlider->setMaximum(nframes-1);
        ui->spinBox->setMaximum(nframes-1);
        Mat img;
        cap.read(img);
        imshow("Frame actual", img);
    }
}

Capturardevideo::~Capturardevideo()
{
    delete ui;
}

bool Capturardevideo::isOpened()
{
    return cap.isOpened();
}

void Capturardevideo::on_horizontalSlider_valueChanged(int value)
{
    cap.set(CAP_PROP_POS_FRAMES, value);
    Mat img;
    cap.read(img);
    if (!img.empty())
        imshow("Frame actual", img);
    ui->spinBox->setValue(value);
}

void Capturardevideo::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void Capturardevideo::on_Capturardevideo_accepted()
{
    cap.set(CAP_PROP_POS_FRAMES, ui->spinBox->value());
    Mat img;
    cap.read(img);
    if (!img.empty())
        crear_nueva(primera_libre(), img);
    destroyWindow("Frame actual");
}

void Capturardevideo::on_Capturardevideo_rejected()
{
    destroyWindow("Frame actual");
}
