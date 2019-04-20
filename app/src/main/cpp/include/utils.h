//
// Created by zhangpeng30 on 2019/3/19.

#ifndef OPENGLSTUDY_UTILS_H
#define OPENGLSTUDY_UTILS_H
#include "ggl.h"

unsigned char * LoadFileContent(const char *path , int &fileSize);
char * LoadFileContent(const char *path );

GLuint CompileShader(GLenum shaderType , const char * shaderCode);

GLuint CreateProgram(GLuint vsShader , GLuint fsShader);
GLuint CreateProgram(const char * vsPath ,const char * fsPath);

GLuint CreateTextureFromBMP(const char * bmpPath);

GLuint CreateTexture2D(unsigned char *pixelData, int Width, int height ,GLenum type);
void  CreateTexture2D(GLuint * textures ,int number ,unsigned char *pixelData, int Width, int height ,GLenum type);

GLuint CreateVBO( GLsizeiptr size ,const void *data , GLenum usage=GL_STATIC_DRAW);

GLuint CreateEBO( GLsizeiptr size ,const void *data , GLenum usage=GL_STATIC_DRAW);

#endif //OPENGLSTUDY_UTILS_H



