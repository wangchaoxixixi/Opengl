#include "secondshadercode.h"
#include <QDebug>
#include <QtMath>

SecondShaderCode::SecondShaderCode(QWidget *parent):
    QGLWidget(parent)
{
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CoreProfile);
    setFormat(format);
    setWindowTitle( "6666" );
}
SecondShaderCode::~SecondShaderCode()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void SecondShaderCode::paintGL()
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    QMetaObject::invokeMethod(this,"updateGL",Qt::QueuedConnection);
}
void SecondShaderCode::initializeGL()
{
    initializeOpenGLFunctions();
    // Shaders
    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0\n"
        "layout (location = 1) in vec3 color;    // 颜色变量的属性位置值为 1\n"
        "out vec3 ourPosition; // 向片段着色器输出一个颜色\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x , position.y, position.z, 1.0);\n"
        "ourPosition = position;\n"
        "}\0";
    const GLchar* fragmentShaderSource = "#version 330 core\n"
        "in vec3 ourPosition;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(ourPosition, 1.0f);\n"
        "}\n\0";
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",infoLog);
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",infoLog);
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s",infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}
void SecondShaderCode::resizeGL( int width, int height )
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
