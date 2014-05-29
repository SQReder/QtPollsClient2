#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T18:35:37
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PollClient
TEMPLATE = app


SOURCES += main.cpp\
        connectdialog.cpp \
    scanworker.cpp \
    clientconnection.cpp \
    clicablelabel.cpp \
    category.cpp \
    categoryselectwindow.cpp \
    categoriesrepository.cpp \
    viewcategorydialog.cpp \
    votedialog.cpp \
    ../Server/logger.cpp

HEADERS  += connectdialog.h \
    scanworker.h \
    clientconnection.h \
    clicablelabel.h \
    category.h \
    categoryselectwindow.h \
    categoriesrepository.h \
    viewcategorydialog.h \
    votedialog.h \
    ../Server/logger.h

FORMS    += connectdialog.ui \
    categoryselectwindow.ui \
    viewcategorydialog.ui \
    votedialog.ui

#QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += #-O3


LIBS += -L$$PWD/lib/ -lzxing
LIBS += -Lc:\opencv\build\x86\mingw\lib
LIBS += -lopencv_core249.dll -lopencv_highgui249.dll -lopencv_imgproc249.dll

INCLUDEPATH += $$PWD/include
INCLUDEPATH += C:\opencv\build\include

RESOURCES += \
    resources.qrc

CONFIG += c++11
