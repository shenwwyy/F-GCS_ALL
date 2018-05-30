#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T14:40:45
#
#-------------------------------------------------

DESTDIR = ../build

QT += core gui
QT += network
QT += sql
QT += opengl
QT += multimedia
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(./qextserialport-1.2beta2/src/qextserialport.pri)


TARGET = F-GCS001
TEMPLATE = app




LIBS += -L../build \
    -lopmapwidget \
    -lglut32 \
    -lopengl32 \
    -lglu32 \
    -lgdi32 \
    -lwinmm


SOURCES += main.cpp\
        mainwindow.cpp \
    datalinkdialog.cpp \
    missiondialog.cpp \
    airplanedialog.cpp \
    ruledialog.cpp \
    settingdialog.cpp \
    mapsettingdialog.cpp \
    savedialog.cpp \
    decode.cpp \
    attitudepanel.cpp \
    datasave.cpp \
    missionsettingdialog.cpp \
    inifile.cpp \
    waypointcsv.cpp \
    glwidget.cpp \
    Calibration.cpp

HEADERS  += mainwindow.h \
    datalinkdialog.h \
    missiondialog.h \
    airplanedialog.h \
    ruledialog.h \
    settingdialog.h \
    mapsettingdialog.h \
    savedialog.h \
    decode.h \
    attitudepanel.h \
    datasave.h \
    missionsettingdialog.h \
    inifile.h \
    waypointcsv.h \
    glwidget.h \
    calibration.h

FORMS    += mainwindow.ui \
    datalinkdialog.ui \
    missiondialog.ui \
    airplanedialog.ui \
    ruledialog.ui \
    settingdialog.ui \
    mapsettingdialog.ui \
    savedialog.ui \
    missionsettingdialog.ui

INCLUDEPATH += ./opmap/core \
               ./opmap/internals \
               ./opmap/mapwidget



RESOURCES += \
    image.qrc

RC_FILE = ico.rc

