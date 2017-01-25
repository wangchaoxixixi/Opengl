#include "textureshader.h"
#include "shader.h"
#include <QDebug>
// Other Libs
#include <SOIL/SOIL.h>
#include <QKeyEvent>
int screenWidth = 821;
int screenHeight = 577;


TextureShader::TextureShader(QWidget *parent):
    QGLWidget(parent)
{
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CoreProfile);
    setFormat(format);
    setWindowTitle( "6666" );
    mixValue = 0.5f;

}
TextureShader::~TextureShader()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void TextureShader::paintGL()
{
    paint();
}

void TextureShader::initializeGL()
{
    initializeOpenGLFunctions();
    ourShader = new Shader("shader\\textures.vert", "shader\\textures.frag");
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left
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
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    unsigned char* image = SOIL_load_image("data/1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

    // Framebuffers
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create a color attachment texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qDebug("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //*****************************************
    // Render
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    // Activate shader
    ourShader->Use();
    // Bind Textures using texture units
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture1"), 0);

    // Draw container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport( 0, 0, (GLint)this->size().width(), (GLint)this->size().height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)this->size().width()/(GLfloat)this->size().height(), 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Clear all relevant buffers
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

    // Draw Screen
    ourShader->Use();
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);	// Use the color attachment texture as the texture of the quad plane
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    paintGL();
}
void TextureShader::resizeGL( int width, int height )
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
void TextureShader::paint()
{
    //*****************************************
    // Render
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    // Activate shader
    ourShader->Use();
    // Bind Textures using texture units
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(ourShader->Program, "ourTexture1"), 0);

    // Draw container
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport( 0, 0, (GLint)this->size().width(), (GLint)this->size().height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)this->size().width()/(GLfloat)this->size().height(), 0.1, 100 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Clear all relevant buffers
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

    // Draw Screen
    ourShader->Use();
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);	// Use the color attachment texture as the texture of the quad plane
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
