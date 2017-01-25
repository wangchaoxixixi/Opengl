#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T18:07:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLCode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    firstshadercode.cpp \
    secondshadercode.cpp \
    textureshader.cpp \
    SOIL/image_DXT.c \
    SOIL/image_helper.c \
    SOIL/SOIL.c \
    SOIL/stb_image_aug.c \
    shader.cpp \
    yuvtextureshader.cpp \
    yuvsphere.cpp \
    sphere.cpp


HEADERS  += mainwindow.h \
    firstshadercode.h \
    secondshadercode.h \
    shader.h \
    textureshader.h \
    SOIL/image_DXT.h \
    SOIL/image_helper.h \
    SOIL/SOIL.h \
    SOIL/stb_image_aug.h \
    SOIL/stbi_DDS_aug.h \
    SOIL/stbi_DDS_aug_c.h \
    yuvtextureshader.h \
    yuvsphere.h \
    sphere.h


FORMS    += mainwindow.ui
QT += widgets
QT += opengl

DISTFILES += \
    data/test_yuv420p_320x180.yuv \
    data/1.jpg \
    data/2.jpg \
    data/3.jpg \
    data/4.jpg \
    data/5.jpg \
    data/pan.jpg \
    data/Panorama.jpg \
    data/NV12.yuv \
    data/pano.yuv \
    data/pano_1.yuv \
    data/testyuv.yuv \
    shader/textures.frag \
    shader/textures.vert \
    shader/emboss.frag \
    shader/yuvGray.frag \
    shader/yuvMosaic.frag \
    shader/yuvTexture.frag \
    shader/yuvEmboss.frag \
    shader/yuvBrightness.frag \
    shader/yuvFilter.frag \
    shader/yuvSobel.frag \
    shader/yuvHDR.frag \
    shader/yuvWatercolor.frag \
    shader/yuvSphere.vert \
    shader/yuvSphere.frag \
    shader/rgb2yuv.frag \
    shader/rgbTexture.frag \
    shader/rgb2yuv.vert \
    shader/show.frag \
    shader/show.vert





