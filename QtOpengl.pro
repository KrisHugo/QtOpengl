QT += core gui widgets opengl openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ObjData.cpp \
    ObjLoader.cpp \
    ObjRender.cpp \
    OpenGLWidgets.cpp \
    main.cpp \
    widget.cpp \
    camera.cpp

HEADERS += \
    ObjData.h \
    ObjLoader.h \
    ObjRender.h \
    OpenGLWidgets.h \
    widget.h \
    camera.h

RESOURCES += \
    obj.qrc \
    shaders.qrc \
    textures.qrc \


TRANSLATIONS += \
    QtOpengl_zh_CN.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


