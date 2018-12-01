#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T11:37:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets charts network

TARGET = expression_v3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mycamerawidget.cpp \
    myareachartwidget.cpp

HEADERS += \
        mainwindow.h \
    mycamerawidget.h \
    myareachartwidget.h

FORMS += \
        mainwindow.ui

#INCLUDEPATH += -I D:\Anaconda3\include

#LIBS += -LD:\Anaconda3\libs -lpython36
# LIBS += "D:\Anaconda3\libs\python3.lib"
# LIBS += "D:\Anaconda3\libs\python36.lib"


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


