#ifndef _SHADERS_H_
#define _SHADERS_H_

void draw();
void create_vertex_buffer();
void load_shader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType);
void compile_shaders();

#endif
