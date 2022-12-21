#include "bajorrelievevideo.h"
#include "ui_bajorrelievevideo.h"

#include "video.h"
#include "QFileDialog"
#include "mainwindow.h"

bajorrelieveVideo::bajorrelieveVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bajorrelieveVideo)
{
    ui->setupUi(this);
    angulo = 0;
    grado = 1.0;
    fondo = 0;
}

bajorrelieveVideo::~bajorrelieveVideo()
{
    delete ui;
}

void bajorrelieveVideo::on_bajorrelieveVideo_accepted()
{
    grado = ui->horizontalSlider->value();
    angulo = ui->dial->value();
    fondo = ui->radioButton->isChecked() ? 0 :
            (ui->radioButton_2->isChecked() ? 1 :
            (ui->radioButton_3->isChecked() ? 2 : 3));

    QString videoOriginal = "";
    if(ui->radioButton_video->isChecked())
        videoOriginal = QFileDialog::getOpenFileName();

    QString nombre= QFileDialog::getSaveFileName(w, "Guardar vídeo", ".", QString::fromLatin1(FiltroVideo.c_str()));

    if(ui->radioButton_video->isChecked())
        bajorrelieve_video(videoOriginal.toLatin1().data(), nombre.toLatin1().data(), angulo, grado, fondo);
    else
        bajorrelieve_camara(nombre.toLatin1().data(), angulo, grado, fondo);
}
