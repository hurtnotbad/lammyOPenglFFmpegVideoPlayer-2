//
// Created by lammy on 2019/4/16.
//

#include <Log.h>
#include <GLProgram.h>
extern "C"
{
#include <libavutil/pixfmt.h>
}

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
        uniform int formatType;
        void main(){
            vec3 yuv;
            vec3 rgb;
            if(formatType==1)//yuv420p
            {
                yuv.r = texture2D(yTexture,vTexCoord).r;
                yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;//
                yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            }else if(formatType==25)//nv12
            {
                yuv.r = texture2D(yTexture,vTexCoord).r;
                yuv.g = texture2D(uTexture,vTexCoord).r - 0.5 ;
                // shader 会将数据归一化，而 uv 的取值区间本身存在-128到正128 然后归一化到0-1 为了正确计算成rgb，
                // 则需要归一化到 -0.5 - 0.5的区间
                yuv.b = texture2D(uTexture,vTexCoord).a - 0.5;
            }else if(formatType==26)//nv21
            {
                yuv.r = texture2D(yTexture,vTexCoord).r;
                yuv.g = texture2D(uTexture,vTexCoord).a - 0.5 ;
                // shader 会将数据归一化，而 uv 的取值区间本身存在-128到正128 然后归一化到0-1 为了正确计算成rgb，
                // 则需要归一化到 -0.5 - 0.5的区间
                yuv.b = texture2D(uTexture,vTexCoord).r - 0.5;
            }
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragNV12 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
//            gl_FragColor = vec4(128.0, 127.0 ,2.01, 255.0);
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

    formatTypeLocation = glGetUniformLocation(program, "formatType");


}

void GLProgram::setUniform()
{

    glEnableVertexAttribArray(aPositionLocation);
    //传递顶点
    glVertexAttribPointer(aPositionLocation,3,GL_FLOAT,GL_FALSE,12 ,vers);//sizeof(float)*3

    glEnableVertexAttribArray(aTexCoordLocation);
    glVertexAttribPointer(aTexCoordLocation,2,GL_FLOAT,GL_FALSE,8 ,txts);//sizeof(float)*2


}

void GLProgram::GetTexture(unsigned int index,int width,int height, unsigned char *buf, bool isa)
{

    unsigned int format =GL_LUMINANCE; // 这里是 灰度图，单通道
    if(isa)
        format = GL_LUMINANCE_ALPHA; // 这表示的是 带 alpha通道的单通道图，即 2通道，r 和 a

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
                     format,//gpu内部格式 亮度，灰度图
                     width,height, //拉升到全屏
                     0,             //边框
                     format,//数据的像素格式 亮度，灰度图 要与上面一致
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL                    //纹理的数据
        );
    }


    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,yuvTexture[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,format,GL_UNSIGNED_BYTE,buf);

}
int i = 0 ;

void GLProgram::Draw(int width , int height,unsigned char * y, unsigned char *u, unsigned char *v,AVPixelFormat avPixelFormat, int* showSize)
{

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
    glViewport(showSize[0],showSize[1], showSize[2], showSize[3]);
    glUseProgram(program);
    setUniform();

    glUniform1i(formatTypeLocation, avPixelFormat);

    if(avPixelFormat == AV_PIX_FMT_YUV420P)
    {
        GetTexture(0,width,height,y, false);  // Y
        glUniform1i(yTextureLocation,0);
        GetTexture(1,width/2,height/2,u,false);  // U
        glUniform1i(uTextureLocation,1);
        GetTexture(2,width/2,height/2,v,false);  // V
        glUniform1i(vTextureLocation,2);
    }
    else if(avPixelFormat == AV_PIX_FMT_NV12 || avPixelFormat == AV_PIX_FMT_NV21)
    {
        GetTexture(0,width,height,y,false);  // Y
        glUniform1i(yTextureLocation,0);
        GetTexture(1,width/2,height/2,u,true);  // Uv
        glUniform1i(uTextureLocation,1);
    }


    //三维绘制
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTexCoordLocation);

}


