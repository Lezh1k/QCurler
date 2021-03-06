#-------------------------------------------------
#
# Project created by QtCreator 2018-07-16T13:20:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCurler
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

INCLUDEPATH += include

SOURCES += \
    src/Commons.cpp \
    src/ResourceChecker.cpp \
    src/ResourceProviderHardcodePolicy.cpp \
    src/ResourceProviderNetworkServicePolicy.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/ConnectionInfoModel.cpp

HEADERS += \
    include/InternetResource.h \
    include/ResourceChecker.h \
    include/ResourceProvider.h \
    include/ResourceProviderHardcodePolicy.h \
    include/ResourceProviderNetworkServicePolicy.h \
    include/mainwindow.h \
    include/ConnectionInfoModel.h \
    include/Commons.h

FORMS += \
        forms/mainwindow.ui

LIBS += -lcurl

target.path = /home/pi/qcurler/qcurler
INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    url.cfg
