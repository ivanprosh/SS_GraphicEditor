#-------------------------------------------------
#
# Project created by QtCreator 2016-08-13T16:20:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EditU
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editmnm.cpp \
    PObject.cpp \
    pobjectprorety.cpp \
    itemdelegate.cpp \
    qlistmodel.cpp \
    prop2model.cpp

HEADERS  += mainwindow.h \
    editmnm.h \
    PObject.h \
    pobjectproperty.h \
    itemdelegate.h \
    qlistmodel.h \
    prop2model.h

FORMS    += mainwindow.ui \
    editmnm.ui

RESOURCES += \
    icon.qrc


