#-------------------------------------------------
#
# Project created by QtCreator 2022-09-01T12:00:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFotoPaint5

TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    ajustecolor.cpp \
    ajustelineal.cpp \
    bajorrelievevideo.cpp \
    capturardevideo.cpp \
    colorfalso.cpp \
    dbajorrelieve.cpp \
    informacion.cpp \
    mainwindow.cpp \
    imagenes.cpp \
    dialognueva.cpp \
    brillocontraste.cpp \
    matsatlum.cpp \
    minmaxvideo.cpp \
    movimiento.cpp \
    perfilado.cpp \
    perspectiva.cpp \
    pincharestirar.cpp \
    suavizados.cpp \
    suavizadotemporal.cpp \
    transicionimagenes.cpp \
    video.cpp \
    rotaravideo.cpp \
    mediaponderada.cpp \
    acercade.cpp \
    mediadevideo.cpp

HEADERS  += mainwindow.h \
    ajustecolor.h \
    ajustelineal.h \
    bajorrelievevideo.h \
    capturardevideo.h \
    colorfalso.h \
    dbajorrelieve.h \
    imagenes.h \
    dialognueva.h \
    brillocontraste.h \
    informacion.h \
    matsatlum.h \
    minmaxvideo.h \
    movimiento.h \
    perfilado.h \
    perspectiva.h \
    pincharestirar.h \
    suavizados.h \
    suavizadotemporal.h \
    transicionimagenes.h \
    video.h \
    rotaravideo.h \
    mediaponderada.h \
    acercade.h \
    mediadevideo.h

FORMS    += mainwindow.ui \
    ajustecolor.ui \
    ajustelineal.ui \
    bajorrelievevideo.ui \
    capturardevideo.ui \
    colorfalso.ui \
    dbajorrelieve.ui \
    dialognueva.ui \
    brillocontraste.ui \
    informacion.ui \
    matsatlum.ui \
    minmaxvideo.ui \
    movimiento.ui \
    perfilado.ui \
    perspectiva.ui \
    pincharestirar.ui \
    suavizados.ui \
    rotaravideo.ui \
    mediaponderada.ui \
    acercade.ui \
    mediadevideo.ui \
    suavizadotemporal.ui \
    transicionimagenes.ui

INCLUDEPATH += "C:\OpenCV\OpenCV4.6.0G\include"

LIBS += -L"C:\OpenCV\OpenCV4.6.0G\lib"\
        -llibopencv_world460

RESOURCES += recursos.qrc

RC_ICONS = imagenes/icono.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
