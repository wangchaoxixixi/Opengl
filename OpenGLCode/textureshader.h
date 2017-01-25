#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>
#include <QKeyEvent>
#include "shader.h"
class TextureShader:public QGLWidget,protected QOpenGLFunctions_3_3_Core
{
private:
   // GLuint shaderProgram;
    GLuint VBO, VAO, EBO;
    GLuint texture1;
    Shader* ourShader;
    int width, height;
    GLuint FBO;
    GLuint texture;
    GLuint RBO;


public:
    GLfloat mixValue;
    explicit TextureShader(QWidget* parent = 0);
    void paint();
    ~TextureShader();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );

};

#endif // TEXTURESHADER_H
