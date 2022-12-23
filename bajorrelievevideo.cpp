#include "bajorrelievevideo.h"
#include "ui_bajorrelievevideo.h"

#include "video.h"
#include "QFileDialog"
#include "mainwindow.h"
#include "QMessageBox"

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

void bajorrelieveVideo::reabrirDialogo() {
    bajorrelieveVideo b;
    b.ui->horizontalSlider->setValue(grado);
    b.ui->dial->setValue(angulo);
    fondo == 0 ?    b.ui->radioButton->setChecked(true) :
      (fondo == 1 ? b.ui->radioButton_2->setChecked(true) :
      (fondo == 2 ? b.ui->radioButton_3->setChecked(true) :
                    b.ui->radioButton_4->setChecked(true)));
    if(ui->radioButton_camara->isChecked())
        b.ui->radioButton_camara->setChecked(true);
    else
        b.ui->radioButton_video->setChecked(true);
    b.exec();
}

void bajorrelieveVideo::on_bajorrelieveVideo_accepted()
{
    grado = ui->horizontalSlider->value();
    angulo = ui->dial->value();
    fondo = ui->radioButton->isChecked() ? 0 :
            (ui->radioButton_2->isChecked() ? 1 :
            (ui->radioButton_3->isChecked() ? 2 : 3));

    QString videoOriginal = "";
    if(ui->radioButton_video->isChecked()){
        videoOriginal = QFileDialog::getOpenFileName();
        if(videoOriginal.isEmpty()){
            QMessageBox::warning(this, "Error",
                                 "Por favor, especifica un vídeo al que aplicar el efecto.");
            reabrirDialogo();
            return;
        }
    }

    QString nombre= QFileDialog::getSaveFileName(w, "Guardar vídeo", ".", QString::fromLatin1(FiltroVideo.c_str()));
    if(nombre.isEmpty()){
        QMessageBox::warning(this, "Error",
                             "Por favor, especifica una ubicación donde guardar el vídeo.");
        reabrirDialogo();
        return;
    }

    if(ui->radioButton_video->isChecked())
        bajorrelieve_video(videoOriginal.toLatin1().data(), nombre.toLatin1().data(), angulo, grado, fondo);
    else
        bajorrelieve_video("", nombre.toLatin1().data(), angulo, grado, fondo);
}
