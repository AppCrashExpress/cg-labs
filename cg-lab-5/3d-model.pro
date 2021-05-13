QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glwidget.cpp \
    horseshoe.cpp \
    light.cpp \
    lightmenu.cpp \
    main.cpp \
    mainwindow.cpp \
    polygon.cpp \
    vectorinput.cpp \
    window.cpp

HEADERS += \
    glwidget.h \
    horseshoe.h \
    light.h \
    lightmenu.h \
    mainwindow.h \
    polygon.h \
    vectorinput.h \
    window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    light_fragment_shader.fsh \
    light_vertex_shader.vsh \
    main_fragment_shader.fsh \
    main_vertex_shader.vsh

RESOURCES += \
    shaders.qrc
