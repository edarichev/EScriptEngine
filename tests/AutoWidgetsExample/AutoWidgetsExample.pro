QT       += core gui
QMAKE_CXXFLAGS += -Wno-deprecated-copy

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += PRECOMPILE_HEADER

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mycell.cpp \
    mycellcolor.cpp \
    mycellstyle.cpp \
    myspreadsheet.cpp \
    stdafx.cpp

HEADERS += \
    mainwindow.h \
    mycell.h \
    mycellcolor.h \
    mycellstyle.h \
    myspreadsheet.h \
    stdafx.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    AutoWidgetsExample_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../EScript/release/ -lEScript
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../EScript/debug/ -lEScript
else:unix: LIBS += -L$$OUT_PWD/../../EScript/ -lEScript

INCLUDEPATH += $$PWD/../../EScript
DEPENDPATH += $$PWD/../../EScript

PRECOMPILED_HEADER = stdafx.h
