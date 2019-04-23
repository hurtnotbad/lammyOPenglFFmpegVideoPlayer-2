//
// Created by lammy on 2019/4/16.
//

#include <Log.h>
#include <GLProgram.h>

#include "GLProgram.h"
#include "utils.h"
//顶点着色器glsl
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //顶点坐标
        attribute vec2 aTexCoord; //材质顶点坐标
        varying vec2 vTexCoord;   //输出的材质坐标
        void main(){
            vTexCoord = vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position = aPosition;
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragYUV420P = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            //gl_FragColor = vec4(rgb.r,rgb.g,rgb.b,1.0);
            gl_FragColor = vec4(rgb,1.0);

//            gl_FragColor = vec4(1.0,.0, 0.0 ,1.0);
        }
);

GLProgram::GLProgram()
{

}

void GLProgram::init()
{
    vsShaderCode = vertexShader;
    fsShaderCode = fragYUV420P;

    GLuint vsShader = CompileShader(GL_VERTEX_SHADER,(char *)vsShaderCode);
    GLint fsShader = CompileShader(GL_FRAGMENT_SHADER , (char *)fsShaderCode);
    program = CreateProgram(vsShader , fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    LOGE("CreateProgram create success");

    aPositionLocation = (GLuint)glGetAttribLocation(program,"aPosition");
    aTexCoordLocation = (GLuint)glGetAttribLocation(program,"aTexCoord");

    yTextureLocation = glGetUniformLocation(program, "yTexture");
    uTextureLocation = glGetUniformLocation(program, "uTexture");
    vTextureLocation = glGetUniformLocation(program, "vTexture");



}

void GLProgram::setUniform()
{

    glEnableVertexAttribArray(aPositionLocation);
    //传递顶点
    glVertexAttribPointer(aPositionLocation,3,GL_FLOAT,GL_FALSE,12 ,vers);//sizeof(float)*3

    glEnableVertexAttribArray(aTexCoordLocation);
    glVertexAttribPointer(aTexCoordLocation,2,GL_FLOAT,GL_FALSE,8 ,txts);//sizeof(float)*2


}

void GLProgram::GetTexture(unsigned int index,int width,int height, unsigned char *buf)
{
    LOGE("创建纹理yuvTexture[0] = %d ", yuvTexture[0]);
    LOGE("创建纹理yuvTexture[1] = %d ", yuvTexture[1]);
    LOGE("创建纹理yuvTexture[2] = %d ", yuvTexture[2]);

    if(yuvTexture[index] == 0)
    {
        //材质初始化
        glGenTextures(1,&yuvTexture[index]);
         LOGE("创建纹理yuvTexture[index] = %d ", yuvTexture[index]);

        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D,yuvTexture[index]);
        //缩小的过滤器
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //设置纹理的格式和大小
        glTexImage2D(GL_TEXTURE_2D,
                     0,           //细节基本 0默认
                     GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                     width,height, //拉升到全屏
                     0,             //边框
                     GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL                    //纹理的数据
        );
    }


    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,yuvTexture[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_LUMINANCE,GL_UNSIGNED_BYTE,buf);

}
int i = 0 ;

void GLProgram::Draw(int width , int height,unsigned char * y, unsigned char *u, unsigned char *v)
{

//    if(i%100 > 50){
//        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glViewport(0, 0, 1000, 1000);
//        glUseProgram(program);
//        setUniform();
//        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
//
//    }else{
//        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glViewport(0, 0, 1000, 1000);
//        glUseProgram(program);
//        setUniform();
//        //三维绘制
//        glDrawArrays(GL_TRIANGLES,0,3);
//    }
//    i ++;


    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);

    glViewport(0, 0, width, height);
    glUseProgram(program);
    setUniform();


    GetTexture(0,width,height,y);  // Y
    glUniform1i(yTextureLocation,0);
    GetTexture(1,width/2,height/2,u);  // U
    glUniform1i(uTextureLocation,1);
    GetTexture(2,width/2,height/2,v);  // V
    glUniform1i(vTextureLocation,2);

    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTexCoordLocation);

}


