#include "yuvtextureshader.h"
#include "shader.h"
#include <SOIL/SOIL.h>
#include <QDebug>
YUVtextureShader::YUVtextureShader(QWidget *parent):
    QGLWidget(parent)
{
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CoreProfile);
    setFormat(format);
    brightness = 0.0;
    contrast = 1.0;
    saturation = 1.0;
    isSepia = 0;
    sepiaIntensity = 1.0;
    temperature = 0.0;
    tint = 0.0;
}
YUVtextureShader::~YUVtextureShader()
{
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
}

void YUVtextureShader::paintGL()
{
    time_filter.start();
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
    glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
    glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture2"), 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
    glUniform1i(glGetUniformLocation(filterShader->Program, "ourTexture3"), 2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    qDebug()<<"filter:"<<time_filter.elapsed()<<"ms************";
    time_show.start();
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
    qDebug()<<"show:"<<time_show.elapsed()<<"ms************";

    //rgb to yuv
    time_rgb2yuv.start();
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
    qDebug()<<"rgb2yuv:"<<time_rgb2yuv.elapsed()<<"ms************";


    time_save.start();
    //save yuv data
    //unsigned char* output_image = new unsigned char[width * height * 3];
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
    qDebug()<<"save:"<<time_save.elapsed()<<"ms************";

    FILE* outfile;
    if((outfile=fopen("./data/pano2.yuv", "wb"))==NULL){
      printf("cannot open this file\n");
      return;
    }
    fwrite(plane[0], 1, width*height, outfile);
    fwrite(plane[1], 1, width*height/4, outfile);
    fwrite(plane[2], 1, width*height/4, outfile);
    fclose(outfile);
    //QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);

}
void YUVtextureShader::initializeGL()
{
    initializeOpenGLFunctions();
    /*
     * yuvTexture
     * yuvFilter
     * yuvSobel
     * yuvHDR
     * yuvWatercolor*/
    filterShader = new Shader("shader\\textures.vert", "shader\\yuvTexture.frag");
    rgb2yuvShader = new Shader("shader\\rgb2yuv.vert", "shader\\rgb2yuv.frag");
    showShader = new Shader("shader\\show.vert","shader\\show.frag");
    // Set up vertex data (and buffer(s)) and attribute pointers
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
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps

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
    glBindTexture(GL_TEXTURE_2D, 0);

    // Load and create a texture
    width = 4096;
    height = 2048;
    image = new unsigned char[width*height*3/2];
    FILE* infile;
    if((infile=fopen("./data/pano.yuv", "rb"))==NULL){
      printf("cannot open this file\n");
      return;
    }
    fread(image, 1, width*height*3/2, infile);

    //YUV Data
    plane[0] = image;
    plane[1] = plane[0] + width*height;
    plane[2] = plane[1] + width*height/4;

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
}
void YUVtextureShader::resizeGL( int screen_width, int screen_height  )
{
    glViewport( 0, 0, (GLint)screen_width, (GLint)screen_height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)screen_width/(GLfloat)screen_height, 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
