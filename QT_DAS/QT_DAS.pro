QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dynamicdraw.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
    data_config.h \
    dynamicdraw.h \
    mainwindow.h \
    mythread.h \
    pcie_api/ald8xxapi.h \
    pcie_api/pcie_fun.h \
    qcustomplot/qcustomplot.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QT_DAS_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# pcie所需命令
win32: LIBS += -lSetupAPI
# qcustom 所需命令

QT += printsupport
HEADERS +=\$$PWD/qcustomplot/qcustomplot.h
SOURCES$$PWD/qcustomplot/qcustomplot.cpp
LIBS += -L$$PWD/pcie_api -lald8xxapi
#LIBS += -L$$PWD/pcie_api -lald8xxapi_function

RC_ICONS = logo.ico

