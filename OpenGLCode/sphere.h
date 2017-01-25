#ifndef SPHERE_H
#define SPHERE_H

#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QTime>
#include "shader.h"
class sphere:public QGLWidget,protected QOpenGLFunctions_3_3_Compatibility
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO, EBO;
    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    GLuint textureMark;
    Shader* filterShader;
    Shader* showShader;
    Shader* rgb2yuvShader;
    Shader* sphereShader;
    GLuint FBO1, FBO2;
    GLuint yTexture, uTexture, vTexture;
    GLuint rgbTexture;
    unsigned char *image;
    unsigned char *watermark;
    unsigned char *plane[3];
    int width, height;
    //QTime time_filter;
    //QTime time_show;
    //QTime time_save;
    //QTime time_rgb2yuv;
    /*sphere*/
    GLUquadricObj *mySphere;
    GLfloat xoffset = 0.0;
    GLfloat xRot, zRot;
    GLfloat xRot0, zRot0;
    int x0;
    int y0;
    float radius;
    //GLfloat zoom;
public:
    GLfloat zoom;

    GLfloat brightness;
    GLfloat contrast;
    GLfloat saturation;
    int isSepia;
    GLfloat sepiaIntensity;
    GLfloat temperature;
    GLfloat tint;
    explicit sphere(QWidget* parent = 0);
    bool drawSphere;
    void mousePressEvent ( QMouseEvent * e );
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent ( QMouseEvent * e );

    ~sphere();
protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int screen_width, int screen_height  );
};
#endif // SPHERE_H
