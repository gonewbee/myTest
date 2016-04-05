#include <stdio.h>
#include <GLES2/gl2.h>

static GLuint programObject;
// 顶点着色器 vPosition句柄
static GLuint mPositionHandle;
// fragment着色器 vColor句柄
static GLuint mColorHandle;
static int g_width;
static int g_height;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
static GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
    return 0;

   // Load the shader source
   // 加载shader源码
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   // 编译shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

int triangle_init() {
    // Vertex shader源码，设置坐标，对应attribute操作
    // 声明的vPosition在glBindAttribLocation中使用
    GLbyte vShaderStr[] =  
      "attribute vec4 vPosition;    \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "}                            \n";
   
   // Fragment shader源码，设置颜色，对应uniform操作
   // 声明的vColor在glGetUniformLocation中使用
   GLbyte fShaderStr[] =  
      "precision mediump float; \n"
      "uniform vec4 vColor;     \n"
      "void main()              \n"
      "{                        \n"
      "  gl_FragColor = vColor; \n"
      "}                        \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;
    // Load the vertex/fragment shaders
    // Vertex Shader 形状的顶点
    vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
    // Fragment Shader 形状的颜色或纹理
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Program 是OpenGL ES的对象，该对象包含用来绘制一个或多个形状的着色器(shader)
    // Create the program object
    programObject = glCreateProgram ( );
    if ( programObject == 0 ) {
        return 0;
    }
    // 将顶点着色器添加到program对象
    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    mPositionHandle = 0;

    // Bind vPosition to attribute mPositionHandle
    // 绑定vPosition到mPositionHandle
    glBindAttribLocation ( programObject, mPositionHandle, "vPosition" );

    // Link the program
    // 创建OpenGL ES的可执行程序
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked ) {
        glDeleteProgram ( programObject );
        return 0;
    }
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    return 1;
}

int resize(int width, int height) {
    g_width = width;
    g_height = height;
}

int triangle_draw() {
   GLfloat vVertices[] = {  // in counterclockwise order 逆时针顺序
       0.0f,  0.5f, 0.0f,   // top
       -0.5f, -0.5f, 0.0f,  // bottom left
       0.5f, -0.5f, 0.0f    // bottom right
   };
   // 颜色
   GLfloat vColors[] = {0.0, 1.0, 1.0, 1.0};
   // 顶点的坐标纬数，(x, y, z)坐标纬数为3
   int coords_per_vertex = 3;
   // 计算顶点数目
   int vertexCount = sizeof(vVertices)/(sizeof(GLfloat)*3);
   // fprintf(stdout, "%s vertexCount:%d\n", __func__, vertexCount);
      
   // Set the viewport
   glViewport ( 0, 0, g_width, g_height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( programObject );

   // Load the vertex data
   glVertexAttribPointer ( mPositionHandle, coords_per_vertex, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( mPositionHandle );
    
   // 获取fragment shader的vColor
   mColorHandle = glGetUniformLocation(programObject, "vColor");
   // fprintf(stdout, "%s mColorHandle:%d\n", __func__, mColorHandle);
   glUniform4fv(mColorHandle, 1, vColors);

   glDrawArrays ( GL_TRIANGLES, 0, vertexCount );
}
