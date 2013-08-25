#-------------------------------------------------
#
# Project created by QtCreator 2013-08-25T16:51:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_Creator-OpenCV
TEMPLATE = app

#opencv include files
INCLUDEPATH += C:\opencv-mingw\install\include

#opencv libs
LIBS += -L"C:/opencv-mingw/install/bin"
LIBS += -lopencv_core246 -lopencv_highgui246 -lopencv_imgproc246  # here just use dll file names without "lib" just add "-l" in front

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
