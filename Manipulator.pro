QT       += core gui printsupport sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 debug

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller.cpp \
    main.cpp \
    model.cpp \
    openglwidget.cpp \
    qcustomplot.cpp \
    qsqlite.cpp \
    shader.cpp \
    view.cpp

HEADERS += \
    ControlCAN.h \
    controller.h \
    model.h \
    openglwidget.h \
    qcustomplot.h \ \
    qsqlite.h \
    shader.h \
    view.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/./ -lControlCAN

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/./ControlCAN.lib

DISTFILES += \
    resource/img/Icon.jpg \
    resource/shader/linkFragmentShader.frag \
    resource/shader/linkVertexShader.vert \
    resource/shader/recFragmentShader.frag \
    resource/shader/recVertexShader.vert \
    resource/shader/sphereFragmentShader.frag \
    resource/shader/sphereVertexShader.vert \
    resource/texture/al.jpg \
    resource/texture/awesomeface.png \
    resource/texture/container.jpg \
    resource/texture/lab.png \
    resource/texture/red.jpg


