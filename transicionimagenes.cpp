#include "transicionimagenes.h"
#include "ui_transicionimagenes.h"

#include "QFileDialog"
#include "mainwindow.h"
#include "video.h"

TransicionImagenes::TransicionImagenes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransicionImagenes)
{
    ui->setupUi(this);
    int cur_row= 0;
    nfoto1= foto_activa();
    nfoto2= nfoto1;
    for (int i= 0, pos= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            ui->listWidget->addItem(QString::fromStdString(foto[i].nombre));
            corresp[pos++]= i;
            if (i!=nfoto1 && (nfoto1==nfoto2 || foto[i].orden>foto[nfoto2].orden)) {
                nfoto2= i;
                cur_row= pos-1;
            }
        }
    }
    imgUno= foto[nfoto1].img;
    imgDos.create(imgUno.size(), imgUno.type());
    imgRes.create(imgUno.size(), imgUno.type());
    ui->listWidget->setCurrentRow(cur_row);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
}

TransicionImagenes::~TransicionImagenes()
{
    delete ui;
}

void TransicionImagenes::on_listWidget_currentRowChanged(int currentRow)
{
    nfoto2= corresp[currentRow];
    cv::resize(foto[nfoto2].img, imgDos, imgUno.size(), 0, 0, INTER_CUBIC);
}

void TransicionImagenes::on_buttonBox_accepted()
{
    QString nombre= QFileDialog::getSaveFileName(w, "Guardar vídeo", ".", QString::fromLatin1(FiltroVideo.c_str()));
    if (!nombre.isEmpty()) {

        // Número de transición
        int trans = ui->radioButton_disolucion->isChecked() ? 0 :
                         (ui->radioButton_cubrir->isChecked() ? 1 : 2);

        // Número de frames de las imágenes
        int framesImg = ui->spinBox->value();

        // Número de fps
        double fps = ui->doubleSpinBox->value();

        // Códec
        int codigocc;
        QString cad= ui->comboBox->currentText();
        if (cad=="DEFAULT")
            codigocc= VideoWriter::fourcc('M','P','E','G');
        else {
            string cadena= cad.toLatin1().data();
            codigocc= VideoWriter::fourcc(cadena[0], cadena[1], cadena[2], cadena[3]);
        }

        // Transición entre imgUno e ImgDos y guardar en ImgRes
        // Meter ImgRes en el vídeo
        // Repetir
        transicion(nfoto1, nfoto2, nombre.toLatin1().data(), framesImg, codigocc, fps, trans);
    }
}
