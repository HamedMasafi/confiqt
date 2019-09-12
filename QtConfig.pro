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

INCLUDEPATH += config models ui pages

SOURCES += \
        config/condition.cpp \
        config/conditiondata.cpp \
        config/configdata.cpp \
        config/configmanager.cpp \
        config/option.cpp \
        models/featurefilterproxy.cpp \
        global.cpp \
        config/jsonconfig.cpp \
        main.cpp \
        models/featuresmodel.cpp \
        models/optionsfilterproxy.cpp \
        models/optionsmodel.cpp \
        pages/pagecommon.cpp \
        pages/pageoptions.cpp \
        ui/editors/abstractoptioneditor.cpp \
        ui/editors/optionbooleditor.cpp \
        ui/optioneditwidget.cpp \
        ui/optionsselectdelegate.cpp \
        pages/pagecrosscompile.cpp \
        pages/pagefeatures.cpp \
        pages/pagefinish.cpp \
        pages/pagelicense.cpp \
        pages/pagemodules.cpp \
        pages/pagenomake.cpp \
        pages/pagerun.cpp \
        pages/pageselectbuildmethod.cpp \
        pages/pageselectpaths.cpp \
        pages/pageselectplatform.cpp \
        pages/pagewelcome.cpp \
        ui/plaintextcombobox.cpp \
        ui/qtconfigwizard.cpp \
        ui/qtlitedialog.cpp \
        ui/saveconfirmdialog.cpp \
        pages/wizardpagebase.cpp

HEADERS += \
        config/condition.h \
        config/conditiondata.h \
        config/configdata.h \
        config/configmanager.h \
        config/option.h \
        models/featurefilterproxy.h \
        global.h \
        config/jsonconfig.h \
        models/featuresmodel.h \
        models/optionsfilterproxy.h \
        models/optionsmodel.h \
        pages/pagecommon.h \
        pages/pageoptions.h \
        ui/editors/abstractoptioneditor.h \
        ui/editors/optionbooleditor.h \
        ui/optioneditwidget.h \
        ui/optionsselectdelegate.h \
        pages/pagecrosscompile.h \
        pages/pagefeatures.h \
        pages/pagefinish.h \
        pages/pagelicense.h \
        pages/pagemodules.h \
        pages/pagenomake.h \
        pages/pagerun.h \
        pages/pageselectbuildmethod.h \
        pages/pageselectpaths.h \
        pages/pageselectplatform.h \
        pages/pagewelcome.h \
        ui/plaintextcombobox.h \
        ui/qtconfigwizard.h \
        ui/qtlitedialog.h \
        ui/saveconfirmdialog.h \
        pages/wizardpagebase.h

FORMS += \
        pages/pagecommon.ui \
        pages/pageoptions.ui \
        ui/optioneditwidget.ui \
        pages/pagecrosscompile.ui \
        pages/pagefeatures.ui \
        pages/pagefinish.ui \
        pages/pagelicense.ui \
        pages/pagemodules.ui \
        pages/pagenomake.ui \
        pages/pagerun.ui \
        pages/pageselectbuildmethod.ui \
        pages/pageselectpaths.ui \
        pages/pageselectplatform.ui \
        pages/pagewelcome.ui \
        ui/qtconfigwizard.ui \
        ui/qtlitedialog.ui \
        ui/saveconfirmdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../WebRequest/WebRequest.pri)
