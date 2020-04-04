#-------------------------------------------------
#
# Project created by QtCreator 2019-11-07T20:58:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = multiThreadCamera
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:/opencv/build/include
win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/build/x64/vc14/lib/ -lopencv_world320d
else:unix: LIBS += -LC:/opencv/build/x64/vc14/lib/ -lopencv_world320

INCLUDEPATH += C:/opencv/build/x64/vc14
DEPENDPATH += C:/opencv/build/x64/vc14

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/opencv/build/x64/vc14/lib/libopencv_world320.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/opencv/build/x64/vc14/lib/libopencv_world320d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/opencv/build/x64/vc14/lib/opencv_world320.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/opencv/build/x64/vc14/lib/opencv_world320d.lib
else:unix: PRE_TARGETDEPS += C:/opencv/build/x64/vc14/lib/libopencv_world320.a
