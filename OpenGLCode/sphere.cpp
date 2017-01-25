#include "sphere.h"
#include "shader.h"
#include <QMouseEvent>
#include <SOIL/SOIL.h>
#include <QDebug>
sphere::sphere(QWidget *parent):
    QGLWidget(parent)
{
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CompatibilityProfile);
    setFormat(format);

    brightness = 0.0;
    contrast = 1.0;
    saturation = 1.0;
    isSepia = 0;
    sepiaIntensity = 1.0;
    temperature = 0.0;
    tint = 0.0;
    /*sphere*/
    xRot = zRot = 0.0;
    zRot0 = xRot0 = 0.0;
    zoom = 0.0;
    radius = 50;
    x0 = 0;
    y0 = 0;
    drawSphere = false;
}
sphere::~sphere()
{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
}

void sphere::paintGL()
{
    if(drawSphere)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        sphereShader->Use();
        glUniform3f(glGetUniformLocation(sphereShader->Program, "LightPosition"),
                    4.0, 0.0, 0.0 );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rgbTexture);
        glUniform1i(glGetUniformLocation(sphereShader->Program, "ourTexture"), 0);
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
        gluSphere(mySphere, radius, 100, 100);
        glPopMatrix();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    else
    {
        //Show
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
        glViewport( 0, 0, (GLint)this->size().width(), (GLint)this->size().height() );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 45.0, (GLfloat)this->size().width()/(GLfloat)this->size().height(), 0.1, 100 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        showShader->Use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rgbTexture);
        glUniform1i(glGetUniformLocation(showShader->Program, "texture1"), 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
}
void sphere::initializeGL()
{
    initializeOpenGLFunctions();
    sphereShader = new Shader("shader\\yuvSphere.vert",
                              "shader\\yuvSphere.frag");
    filterShader = new Shader("shader\\textures.vert", "shader\\yuvTexture.frag");
    rgb2yuvShader = new Shader("shader\\rgb2yuv.vert", "shader\\rgb2yuv.frag");
    showShader = new Shader("shader\\show.vert","shader\\show.frag");
    glClearColor(1.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    // Load and create a texture
    width = 4096;
    height = 2048;
    image = new unsigned char[width*height*3/2];
    watermark = new unsigned char[width*height*4];
    FILE* infile;
    if((infile=fopen("data\\pano.yuv", "rb"))==NULL){
      printf("cannot open this file\n");
      return;
    }
    fread(image, 1, width*height*3/2, infile);
    fclose(infile);
    watermark = SOIL_load_image("data/watermark2.png", &width, &height, 0, SOIL_LOAD_RGBA);

    //YUV Data
    plane[0] = image;
    plane[1] = plane[0] + width*height;
    plane[2] = plane[1] + width*height/4;

    GLfloat vertices[] = {
           // Positions          // Colors           // Texture Coords
            1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
            1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
           -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
           -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
       };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    // Load and create a texture
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ===================
    // Texture 3
    // ===================
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ===================
    // TextureMark
    // ===================
    glGenTextures(1, &textureMark);
    glBindTexture(GL_TEXTURE_2D, textureMark); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, watermark);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

   // FBO1
   glGenFramebuffers(1, &FBO1);
   glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
   // RGB texture
   glGenTextures(1, &rgbTexture);
   glBindTexture(GL_TEXTURE_2D, rgbTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBindTexture(GL_TEXTURE_2D, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rgbTexture, 0);
   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
     qDebug("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   //FBO2
   glGenFramebuffers(1, &FBO2);
   glBindFramebuffer(GL_FRAMEBUFFER, FBO2);
   // y texture
   glGenTextures(1, &yTexture);
   glBindTexture(GL_TEXTURE_2D, yTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBindTexture(GL_TEXTURE_2D, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, yTexture, 0);
   // u texture
   glGenTextures(1, &uTexture);
   glBindTexture(GL_TEXTURE_2D, uTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, uTexture, 0);
   // v texture;
   glGenTextures(1, &vTexture);
   glBindTexture(GL_TEXTURE_2D, vTexture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, vTexture, 0);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
     qDebug("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   // Filter
   glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
   glViewport( 0, 0, (GLint)width, (GLint)height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glClear(GL_COLOR_BUFFER_BIT);
   GLint TexSizeLocation = glGetUniformLocation(filterShader->Program, "TexSize");
   GLint BrightnessLocation = glGetUniformLocation(filterShader->Program, "Brightness");
   GLint ContrastLocation = glGetUniformLocation(filterShader->Program, "Contrast");
   GLint SaturationLocation = glGetUniformLocation(filterShader->Program, "Saturation");
   GLint isSepiaLocation = glGetUniformLocation(filterShader->Program, "isSepia");
   GLint SepiaIntensityLocation = glGetUniformLocation(filterShader->Program, "intensity");
   GLint temperatureLocation = glGetUniformLocation(filterShader->Program, "temperature");
   GLint tintLocation = glGetUniformLocation(filterShader->Program, "tint");

   filterShader->Use();
   glUniform2f(TexSizeLocation, width, height);
   glUniform1f(BrightnessLocation, brightness);
   glUniform1f(ContrastLocation, contrast);
   glUniform1f(SaturationLocation, saturation);
   glUniform1f(SepiaIntensityLocation, sepiaIntensity);
   glUniform1i(isSepiaLocation, isSepia);
   glUniform1f(temperatureLocation, temperature);
   glUniform1f(tintLocation, tint);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture1);
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
   glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture1"), 0);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture2);
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
   glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture2"), 1);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, texture3);
   //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
   glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture3"), 2);

   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_2D, textureMark);
   glUniform1i(glGetUniformLocation(filterShader->Program, "ourTextureMark"), 3);

   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   //rgb to yuv
   glBindFramebuffer(GL_FRAMEBUFFER, FBO2);
   glClear(GL_COLOR_BUFFER_BIT);
   glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
   glViewport( 0, 0, width, height);
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 45.0, width/height, 0.1, 100 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   rgb2yuvShader->Use();
   glBindVertexArray(VAO);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, rgbTexture);
   glUniform1i(glGetUniformLocation(rgb2yuvShader->Program, "rgbTexture"), 0);
   GLenum yuv_bufs[] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
   glDrawBuffers(3, yuv_bufs);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);

   //save yuv data
   unsigned char* plane[3];
   plane[0] = new unsigned char[width*height];
   plane[1] = new unsigned char[width*height/4];
   plane[2] = new unsigned char[width*height/4];

   glBindTexture(GL_TEXTURE_2D, yTexture);
   glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);

   glBindTexture(GL_TEXTURE_2D, uTexture);
   glGenerateMipmap(GL_TEXTURE_2D);
   glGetTexImage(GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE, plane[1]);

   glBindTexture(GL_TEXTURE_2D, vTexture);
   glGenerateMipmap(GL_TEXTURE_2D);
   glGetTexImage(GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE, plane[2]);

   FILE* outfile;
   if((outfile=fopen("./data/pano2.yuv", "wb"))==NULL){
     printf("cannot open this file\n");
     return;
   }
   fwrite(plane[0], 1, width*height, outfile);
   fwrite(plane[1], 1, width*height/4, outfile);
   fwrite(plane[2], 1, width*height/4, outfile);
   fclose(outfile);
}
void sphere::resizeGL( int screen_width, int screen_height  )
{
    if ( height == 0 )
    {
      height = 1;
    }
    glViewport( 0, 0, (GLint)screen_width, (GLint)screen_height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)screen_width/(GLfloat)screen_height, 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
void sphere::mousePressEvent ( QMouseEvent * e )
{

    x0 = e->x();
    y0 = e->y();
    /*zRot0 = zRot;
    xRot0 = xRot;*/
}

void sphere::wheelEvent(QWheelEvent *event)
 {
     int delta = event->delta() / 15;
     zoom+=delta;
     //if ((zoom<0?-zoom:zoom)>=radius)
         //zoom =zoom<0?-(radius-1):radius-1;
     updateGL();
 }
void sphere::mouseMoveEvent ( QMouseEvent * e )
{
     int x1 = e->x();
     int y1 = e->y();
     int delta_x = x0 - x1;
     int delta_y = y0 - y1;
     zRot= zRot0 + (double)delta_x/9;
     xRot= xRot0 + (double)delta_y/9;
     x0 = e->x();
     y0 = e->y();
     zRot0 = zRot;
     xRot0 = xRot;
     updateGL();
}
