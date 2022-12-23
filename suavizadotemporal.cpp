#include "suavizadotemporal.h"
#include "ui_suavizadotemporal.h"
#include "mainwindow.h"

#include "video.h"
#include "QFileDialog"

suavizadoTemporal::suavizadoTemporal(QString nombreVideo,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::suavizadoTemporal)
{
    ui->setupUi(this);
    nombreVideoOriginal = nombreVideo;
}

suavizadoTemporal::~suavizadoTemporal()
{
    delete ui;
}

void suavizadoTemporal::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
    n = value;
}

void suavizadoTemporal::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void suavizadoTemporal::on_suavizadoTemporal_accepted()
{
    QString nombreVideoSalida= QFileDialog::getSaveFileName(w, "Guardar vídeo", ".", QString::fromLatin1(FiltroVideo.c_str()));
    if(!nombreVideoOriginal.isEmpty() && !nombreVideoSalida.isEmpty())
        suavizadoTemporalVideo(nombreVideoOriginal.toLatin1().data(), nombreVideoSalida.toLatin1().data(), n);
}

void suavizadoTemporal::on_suavizadoTemporal_rejected()
{
    close();
}
