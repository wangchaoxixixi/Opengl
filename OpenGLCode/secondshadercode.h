#ifndef SECONDSHADERCODE_H
#define SECONDSHADERCODE_H

#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>
class SecondShaderCode:public QGLWidget,protected QOpenGLFunctions_3_3_Core
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO;
public:
    explicit SecondShaderCode(QWidget* parent = 0);
    ~SecondShaderCode();
protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
};

#endif // SECONDSHADERCODE_H
