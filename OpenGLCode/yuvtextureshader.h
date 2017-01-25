#ifndef YUVTEXTURESHADER_H
#define YUVTEXTURESHADER_H

#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>
#include "shader.h"
class YUVtextureShader:public QGLWidget,protected QOpenGLFunctions_3_3_Core
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO, EBO;
    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    Shader* filterShader;
    Shader* showShader;
    Shader* rgb2yuvShader;
    GLuint FBO1, FBO2;
    GLuint yTexture, uTexture, vTexture;
    GLuint rgbTexture;
    unsigned char *image;
    unsigned char *plane[3];
    int width, height;
    QTime time_filter;
    QTime time_show;
    QTime time_save;
    QTime time_rgb2yuv;
public:
    GLfloat brightness;
    GLfloat contrast;
    GLfloat saturation;
    int isSepia;
    GLfloat sepiaIntensity;
    GLfloat temperature;
    GLfloat tint;
    explicit YUVtextureShader(QWidget* parent = 0);

    ~YUVtextureShader();
protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int screen_width, int screen_height  );
};

#endif // YUVTEXTURESHADER_H
