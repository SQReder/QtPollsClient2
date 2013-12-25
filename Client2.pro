#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T18:35:37
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client2
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
    votedialog.cpp

HEADERS  += connectdialog.h \
    scanworker.h \
    clientconnection.h \
    clicablelabel.h \
    category.h \
    categoryselectwindow.h \
    categoriesrepository.h \
    viewcategorydialog.h \
    votedialog.h

FORMS    += connectdialog.ui \
    categoryselectwindow.ui \
    viewcategorydialog.ui \
    votedialog.ui

#QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += --std=c++11 #-O3


LIBS += -L$$PWD/lib/ -lzxing
LIBS += -Lc:\cv\opencv\build\x86\mingw\lib
LIBS += -lopencv_core247.dll
LIBS += -lopencv_highgui247.dll -lopencv_imgproc247.dll

INCLUDEPATH += $$PWD/include
INCLUDEPATH += c:\cv\opencv\build\include

RESOURCES += \
    resources.qrc
