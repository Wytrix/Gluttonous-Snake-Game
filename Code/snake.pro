QT       += core gui
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app

RC_ICONS = ico.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    food.cpp \
    gamecontroller.cpp \
    snake.cpp \
    wall.cpp \
    button.cpp \
    cloud.cpp

HEADERS  += mainwindow.h \
    food.h \
    gamecontroller.h \
    constants.h \
    snake.h \
    wall.h \
    button.h \
    cloud.h

RESOURCES += \
    image.qrc
