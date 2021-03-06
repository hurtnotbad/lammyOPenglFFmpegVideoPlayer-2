//
// Created by lammy on 2019/4/16.
//

#ifndef LAMMYOPENGLVIDEOPLAYER_SHADER_H
#define LAMMYOPENGLVIDEOPLAYER_SHADER_H


extern "C"
{
#include <libavutil/pixfmt.h>
}
#include "ggl.h"

class GLProgram {

public:
    const char*  vsShaderCode;
    const char*  fsShaderCode;
    int program;

     float vers[12] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
    };

    //加入材质坐标数据
     float txts[8] = {
            1.0f,0.0f , //右下
            0.0f,0.0f,
            1.0f,1.0f,
            0.0,1.0
    };

    GLuint aPositionLocation , aTexCoordLocation;
    GLuint yTextureLocation , uTextureLocation,vTextureLocation;
    GLuint yuvTexture[3]={0};
    GLuint width , height;
    GLuint formatTypeLocation;
public:

    GLProgram();
    void init();
    void setUniform();


    void GetTexture(unsigned int index,int width,int height, unsigned char *buf, bool isa);
    void Draw(int width , int height, unsigned char * y,unsigned char *u,unsigned char *v,AVPixelFormat avPixelFormat, int* showSize);

};


#endif //LAMMYOPENGLVIDEOPLAYER_SHADER_H
