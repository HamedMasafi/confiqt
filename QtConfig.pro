#-------------------------------------------------
#
# Project created by QtCreator 2019-08-24T12:45:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtConfig
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
        configmanager.cpp \
        global.cpp \
        jsonconfig.cpp \
        main.cpp \
        optioneditwidget.cpp \
        optionsselectdelegate.cpp \
        pagecrosscompile.cpp \
        pagefeatures.cpp \
        pagelibs.cpp \
        pagelicense.cpp \
        pagemodules.cpp \
        pageselectbuildmethod.cpp \
        pageselectpaths.cpp \
        pageselectplatform.cpp \
        pagewelcome.cpp \
        qtconfigwizard.cpp \
        wizard.cpp \
        wizardpagebase.cpp

HEADERS += \
        configmanager.h \
        global.h \
        jsonconfig.h \
        optioneditwidget.h \
        optionsselectdelegate.h \
        pagecrosscompile.h \
        pagefeatures.h \
        pagelibs.h \
        pagelicense.h \
        pagemodules.h \
        pageselectbuildmethod.h \
        pageselectpaths.h \
        pageselectplatform.h \
        pagewelcome.h \
        qtconfigwizard.h \
        wizard.h \
        wizardpagebase.h

FORMS += \
        optioneditwidget.ui \
        pagecrosscompile.ui \
        pagefeatures.ui \
        pagelibs.ui \
        pagelicense.ui \
        pagemodules.ui \
        pageselectbuildmethod.ui \
        pageselectpaths.ui \
        pageselectplatform.ui \
        pagewelcome.ui \
        qtconfigwizard.ui \
        wizard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
