#include "yuvsphere.h"
#include "shader.h"
#include <QDebug>
#include <QMouseEvent>
// Other Libs
#include <SOIL/SOIL.h>
#include <QWidget>


YUVsphere::YUVsphere(QWidget *parent):
    QGLWidget(parent)
{
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CompatibilityProfile);
    setFormat(format);
    xRot = zRot = 0.0;
    zRot0 = xRot0 = 0.0;
    zoom = 0.0;
    radius = 50;
    x0 = 0;
    y0 = 0;

}
YUVsphere::~YUVsphere()
{
}

void YUVsphere::paintGL()
{
    // Render
    // Clear the colorbuffer
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Activate shader
    ourShader->Use();
    glUniform3f(glGetUniformLocation(ourShader->Program, "LightPosition"),
                4.0, 0.0, 0.0 );
    // Bind Textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture2"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture3"), 2);

    mySphere = gluNewQuadric();
    gluQuadricDrawStyle(mySphere, GLU_FILL);
    gluQuadricNormals(mySphere, GLU_SMOOTH);
    gluQuadricTexture(mySphere, GL_TRUE);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,zoom);
    glRotatef( xRot, 1.0,  0.0,  0.0 );
    glRotatef( zRot, 0.0,  0.0,  1.0 );
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    //glTranslated(0.0, 0.0, -4.0);
    //使地球沿Y轴旋转
    gluSphere(mySphere, radius, 100, 100);
    glPopMatrix();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);

}
void YUVsphere::initializeGL()
{
    initializeOpenGLFunctions();
    ourShader = new Shader("shader\\yuvSphere.vert",
                           "shader\\yuvSphere.frag");

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    // Load and create a texture
    width = 4096;
    height = 2048;
    image = new unsigned char[width*height*3/2];
    FILE* infile;
    if((infile=fopen("data\\pano.yuv", "rb"))==NULL){
      printf("cannot open this file\n");
      return;
    }
    fread(image, 1, width*height*3/2, infile);

    //YUV Data
    plane[0] = image;
    plane[1] = plane[0] + width*height;
    plane[2] = plane[1] + width*height/4;

    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ===================
    // Texture 3
    // ===================
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}
void YUVsphere::resizeGL( int width, int height )
{
    if ( height == 0 )
    {
      height = 1;
    }
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
void YUVsphere::mousePressEvent ( QMouseEvent * e )
{

    x0 = e->x();
    y0 = e->y();
    /*zRot0 = zRot;
    xRot0 = xRot;*/
}

void YUVsphere::wheelEvent(QWheelEvent *event)
 {
     int delta = event->delta() / 15;
     zoom+=delta;
     //if ((zoom<0?-zoom:zoom)>=radius)
         //zoom =zoom<0?-(radius-1):radius-1;
     updateGL();
 }
void YUVsphere::mouseMoveEvent ( QMouseEvent * e )
{
     int x1 = e->x();
     int y1 = e->y();
     int delta_x = x0 - x1;
     int delta_y = y0 - y1;
     zRot= zRot0 - (double)delta_x/9;
     xRot= xRot0 + (double)delta_y/9;
     x0 = e->x();
     y0 = e->y();
     zRot0 = zRot;
     xRot0 = xRot;
     updateGL();
}
