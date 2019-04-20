#include <Log.h>
#include "utils.h"
#include "ggl.h"


GLuint CompileShader(GLenum shaderType , const char * shaderCode){

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1 , &shaderCode, nullptr);//1 表示多少句代码，所有代码都放着。第三个表示如果多句代码，
    //则要与前面的代码的长度。
    glCompileShader(shader);

    GLint compileResult = GL_TRUE;
    // 查看编译状态
    glGetShaderiv(shader,GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE){
        char szLog[1024] = {0};
        GLsizei  logLen = 0;// 存储日志长度
        glGetShaderInfoLog(shader , 1024 , &logLen , szLog);//1024 为日志的最大长度
        LOGE("compile error , log: %s" , szLog);
        LOGE("compile error ,shader %s" , shaderCode);
        glDeleteShader(shader);
        return  0;
    }
    return shader;

}

GLuint CreateProgram(GLuint vsShader , GLuint fsShader){
    GLuint program = glCreateProgram();
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);
    glLinkProgram(program);
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);
    GLint nResult;
    glGetProgramiv(program , GL_LINK_STATUS, &nResult);
    if(nResult == GL_FALSE){
        char log[1024] = {0};
        GLsizei  len = 0;// 存储日志长度
        glGetShaderInfoLog(program , 1024 , &len , log);//1024 为日志的最大长度
        LOGE("create program error , log: %s" , log);
        glDeleteProgram(program);
        return  0;
    }
    LOGE("create program success  " );
    return program;
}

GLuint CreateProgram(const char * vsPath ,const char * fsPath){
    LOGE("CreateProgram create");
    int fileSize = 0;
    unsigned  char * shaderCode = LoadFileContent(vsPath,fileSize);
    GLuint vsShader = CompileShader(GL_VERTEX_SHADER,(char *)shaderCode);
    delete shaderCode;
    shaderCode = LoadFileContent(fsPath, fileSize);
    GLint fsShader = CompileShader(GL_FRAGMENT_SHADER , (char *)shaderCode);
    GLuint program = CreateProgram(vsShader , fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    LOGE("CreateProgram create success");
    return program;
}
GLuint CreateTexture2D(unsigned char *pixelData, int width, int height ,GLenum type){
    GLuint texture;
    glGenTextures(1 , &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 表示图像放大时候，使用线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 表示图像缩小时候，使用线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0 , type, width ,height, 0, type,GL_UNSIGNED_BYTE, pixelData);//GL_RGBA
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
void CreateTexture2D( GLuint* textures ,int number ,unsigned char *pixelData, int width, int height ,GLenum type){
    glGenTextures(number ,textures);
    for(int i=0 ;i< number ; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 表示图像放大时候，使用线性过滤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 表示图像缩小时候，使用线性过滤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE,
                     pixelData);//GL_RGBA
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

unsigned  char* DecodeBMP(unsigned char * bmpFileData, int &width ,int &height){
    if(0x4D42 == *((unsigned short*)bmpFileData)){ // 数据头是否为0x4D42 判断是否是 24位的位图,
        // 读格式头
        int pixelDataOffset = * ((int *)(bmpFileData + 10));// 取出 像素数据在内存块的偏移地址
        width = *((int *)(bmpFileData + 18));
        height= *((int *)(bmpFileData + 22));
        unsigned char *pixelData = bmpFileData + pixelDataOffset;
        // 位图 像素数据 是 bgr排布的，所以 更换 r b的位置
        for(int i =0 ; i < width * height * 3 ; i += 3){
            unsigned char temp = pixelData[i];
            pixelData[i] = pixelData[i + 2];
            pixelData[i+2] = temp;

        }
        LOGE("DecodeBMP success " );
        return pixelData;
    }
    LOGE("DecodeBMP error " );
    return nullptr;
}

GLuint CreateTextureFromBMP(const char * bmpPath){
    int nFileSize = 0;
    unsigned char *bmpFileContent = LoadFileContent(bmpPath, nFileSize);
    if(bmpFileContent==NULL){
        return 0;
    }
    int bmpWidth= 0,bmpHeight =0;
    unsigned char *pixelData = DecodeBMP(bmpFileContent,bmpWidth,bmpHeight);
    LOGE("CreateTextureFromBMP width = %d , height = %d  " , bmpWidth, bmpHeight );
    if(pixelData==NULL){
        delete[] bmpFileContent;
        LOGE("CreateTextureFromBMP error " );
        return 0;
    }
    GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight,GL_RGB);
    delete [] bmpFileContent;
    LOGE("CreateTextureFromBMP success " );
    return texture;
}

/***
 *
 * @param size
 * @param data
 * @param usage  //GL_STATIC_DRAW       表示 放入显卡 不会修改 ，这里数据跑到显卡了
 * GL_DYNAMIC_DRAW    表示 放入显卡 可能会修改 ，这里数据跑到显卡了
 * @return
 */
GLuint CreateVBO( GLsizeiptr size ,const void *data , GLenum usage){
    GLuint vbo;
    glGenBuffers(1, &vbo); // 1 表示需要一个vbo , 后面的vbo 指向显存块
    glBindBuffer(GL_ARRAY_BUFFER , vbo);//绑定显存地址
    glBufferData(GL_ARRAY_BUFFER, size , data , usage);
    glBindBuffer(GL_ARRAY_BUFFER,0);// 设置当前buffer为0 即解绑
    return  vbo;
}

GLuint CreateEBO( GLsizeiptr size ,const void *data , GLenum usage){
    GLuint ebo;
    glGenBuffers(1, &ebo); // 1 表示需要一个vbo , 后面的vbo 指向显存块
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , ebo);//绑定显存地址
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size , data , usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);// 设置当前buffer为0 即解绑
    return  ebo;
}


