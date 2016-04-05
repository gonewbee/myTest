//
// Created by zsy on 16-4-5.
//
#include <string.h>
#include <stdio.h>
#include "esUtil.h"
#include "gles2_color_polygon.h"

static UserContext context;

typedef struct
{
    // Handle to a program object
    GLuint programObject;

} UserData;

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
    color_polygon_draw(&context);
}

int main ( int argc, char *argv[] )
{
    ESContext esContext;
    UserData  userData;

    esInitContext ( &esContext );
    esContext.userData = &userData;

    int width = 320;
    int height = 240;
    fprintf(stdout, "width:%d height:%d\n", width, height);

    esCreateWindow ( &esContext, "Hello Triangle", width, height, ES_WINDOW_RGB );

    memset(&context, 0, sizeof(context));

    if ( !color_polygon_create(&context) )
        return 1;

    setup_color_polygon_VBOs(&context);

    userResize(&context, width, height);
    esRegisterDrawFunc ( &esContext, Draw );

    esMainLoop ( &esContext );
    return 0;
}

