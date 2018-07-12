#-------------------------------------------------
#
# Project created by QtCreator 2018-06-13T16:57:44
#
#-------------------------------------------------

QT       += core gui qml quick quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvText
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    rtspstreammuxtask.cpp

HEADERS += \
        mainwindow.h \
    rtspstreammuxtask.h


#INCLUDEPATH += $$PWD/../opencv/build/include/
#INCLUDEPATH += $$PWD/../opencv/build/include/opencv
#INCLUDEPATH += $$PWD/../opencv/build/include/opencv2

INCLUDEPATH += $$PWD/../../opencv/build/install/include
INCLUDEPATH += $$PWD/../../opencv/build/install/include/opencv
INCLUDEPATH += $$PWD/../../opencv/build/install/include/opencv2

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../opencv/build/x64/vc14/lib/ -lopencv_world340

#INCLUDEPATH += $$PWD/../opencv/build/x64/vc14
#DEPENDPATH += $$PWD/../opencv/build/x64/vc14

#win32: LIBS += -L$$PWD/../opencv/build/x64/vc14/lib/ -lopencv_ts340

#INCLUDEPATH += $$PWD/../opencv/build/x64/vc14
#DEPENDPATH += $$PWD/../opencv/build/x64/vc14


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/release/ -lopencv_ts341
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/debug/ -lopencv_ts341d

#INCLUDEPATH += $$PWD/../../build/lib/Release
#DEPENDPATH += $$PWD/../../build/lib/Release

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world341
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x64/vc14/lib/ -lopencv_world341d

#INCLUDEPATH += $$PWD/../../opencv/build/x64/vc14
#DEPENDPATH += $$PWD/../../opencv/build/x64/vc14



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x86/vc14/lib/ -lopencv_world341
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x86/vc14/lib/ -lopencv_world341d

INCLUDEPATH += $$PWD/../../opencv/build/x86/vc14
DEPENDPATH += $$PWD/../../opencv/build/x86/vc14

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv/build/x86/vc14/lib/ -lopencv_img_hash341
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv/build/x86/vc14/lib/ -lopencv_img_hash341d

INCLUDEPATH +=   $$PWD/../../FFMPEG/dev/include
LIBS +=         -L$$PWD/../../FFMPEG/dev/lib/ -lavcodec\
                -L$$PWD/../../FFMPEG/dev/lib/ -lavdevice\
                -L$$PWD/../../FFMPEG/dev/lib/ -lavfilter\
                -L$$PWD/../../FFMPEG/dev/lib/ -lavformat\
                -L$$PWD/../../FFMPEG/dev/lib/ -lavutil\
                -L$$PWD/../../FFMPEG/dev/lib/ -lpostproc\
                -L$$PWD/../../FFMPEG/dev/lib/ -lswresample\
                -L$$PWD/../../FFMPEG/dev/lib/ -lswscale

RESOURCES += \
    qml.qrc



