QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml network

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/DownloadManager.pri)

DESTDIR = $$APP_OUTPUT_DIR

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    TARGET = DownloadManagerD
} else {
    TARGET = DownloadManager
}

SOURCES += \
    DownloadManager.cpp \
    DownloadTask.cpp \
    NewTaskDialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DownloadManager.h \
    DownloadTask.h \
    NewTaskDialog.h \
    mainwindow.h

FORMS += \
    NewTaskDialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    DownloadManager_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
