#ifndef FIRSTSHADERCODE_H
#define FIRSTSHADERCODE_H
#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
class FirstShaderCode:public QGLWidget,protected QOpenGLFunctions_3_3_Core
{
private:
    GLuint shaderProgram;
    GLuint VBO, VAO;
public:
    explicit FirstShaderCode(QWidget* parent = 0);
    ~FirstShaderCode();
protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
};

#endif // FIRSTSHADERCODE_H
