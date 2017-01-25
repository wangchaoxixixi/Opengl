#ifndef YUVsphere_H
#define YUVsphere_H

#include <QGLWidget>
#include <gl/glu.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QTime>
#include "shader.h"
class YUVsphere: public QGLWidget, protected QOpenGLFunctions_3_3_Compatibility
{
    //Q_OBJECT

private:
    GLuint shaderProgram;
    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    Shader* ourShader;
    unsigned char *image;
    unsigned char *plane[3];
    int width, height;
    GLUquadricObj *mySphere;
    GLfloat xoffset = 0.0;
    GLfloat xRot, zRot;
    GLfloat xRot0, zRot0;
    int x0;
    int y0;
    float radius;
signals:
    void zoomChanged();
public:
    GLfloat zoom;
    explicit YUVsphere(QWidget* parent = 0);
    void mousePressEvent ( QMouseEvent * e );
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent ( QMouseEvent * e );
    ~YUVsphere();
protected:
  void initializeGL();
  void paintGL();
  void resizeGL( int width, int height );
};

#endif // YUVsphere_H
