
#include <stdlib.h>
#include "glm/gtc/matrix_transform.hpp"

#include "globals.h"
#include "LineMode.h"


LineMode::LineMode(void)
{
    this->points = new std::list<glm::vec3>();
}


LineMode::~LineMode(void)
{
    delete this->points;
}

void LineMode::Initialise()
{
    // GL Init
    glClearColor(0, 0, 0, 0);
    glLineWidth(4);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create Programs
    this->lineProgram = shader::LoadProgramFromFiles("shaders/line.vert", "shaders/line.frag");
    this->quadsProgram = shader::LoadProgramFromFiles("shaders/quads.vert", "shaders/quads.frag");
    this->alphaLineProgram = shader::LoadProgramFromFiles("shaders/passthrough.vert", "shaders/line_alpha.frag");
    this->passthroughProgram = shader::LoadProgramFromFiles("shaders/passthrough.vert", "shaders/passthrough.frag");
    this->gaussianHProgram = shader::LoadProgramFromFiles("shaders/passthrough.vert", "shaders/gaussian_hor.frag");
    this->gaussianVProgram = shader::LoadProgramFromFiles("shaders/passthrough.vert", "shaders/gaussian_vert.frag");
    
    // Line Program Variables
    this->uniformLineViewport = glGetUniformLocation(this->lineProgram.id, "viewport");
    this->uniformLineTime = glGetUniformLocation(this->lineProgram.id, "time");
    
    // Quads Program Variables
    this->uniformQuadsViewport = glGetUniformLocation(this->quadsProgram.id, "viewport");
    this->uniformQuadsTime = glGetUniformLocation(this->quadsProgram.id, "time");
    
    glm::mat4 ortho = glm::ortho<float>(0, 640, 480, 0);

    shader::Bind(this->lineProgram);
    glUniformMatrix4fv(this->uniformLineViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);

    shader::Bind(this->quadsProgram);
    glUniformMatrix4fv(this->uniformQuadsViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);
    
    // Line Buffers (array, elements, vao)
    glGenBuffers(1, &this->lineArrayBuffer);
    glGenBuffers(1, &this->lineElementBuffer);
    glGenVertexArrays(1, &this->lineVertexArray);

    glBindVertexArray(this->lineVertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->lineArrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*) (sizeof(GLfloat) * 2));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->lineElementBuffer);
    }
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Passthrough Buffers (just the array)
    glGenBuffers(1, &this->passthroughBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
    {
        GLfloat data[] = { 1.f, 1.f, -1.f, 1.f, -1.f,-1.f,
					      -1.f,-1.f,  1.f,-1.f,  1.f, 1.f };
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, data, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Quads Buffers (just the array for now)
    glGenBuffers(1, &this->quadsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadsBuffer);
    {
        GLfloat data[] = {
            0, 0, 0, 1, 0, 1,
            10, 0, 1, 0, 0, 1,
            10, 10, 0, 0, 1, 1,

            0, 0, 0, 1, 0, 1,
            10, 10, 0, 0, 1, 1,
            0, 10, 1, 1, 0, 1,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, data, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*) (sizeof(GLfloat) * 2));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Scene Frame Buffer
    glGenTextures(1, &this->sceneFrameTexture);
    glBindTexture(GL_TEXTURE_2D, this->sceneFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 640, 480, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &this->sceneFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->sceneFrameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->sceneFrameTexture, 0);
    
    glGenTextures(1, &this->alphaSceneFrameTexture);
    glBindTexture(GL_TEXTURE_2D, this->alphaSceneFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 640, 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &this->alphaSceneFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->alphaSceneFrameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->alphaSceneFrameTexture, 0);

    // Blur Buffers
    glGenTextures(1, &this->gaussianHFrameTexture);
    glBindTexture(GL_TEXTURE_2D, this->gaussianHFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 640, 480, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &this->gaussianHFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->gaussianHFrameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gaussianHFrameTexture, 0);
    
    glGenTextures(1, &this->gaussianVFrameTexture);
    glBindTexture(GL_TEXTURE_2D, this->gaussianVFrameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 640, 480, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &this->gaussianVFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->gaussianVFrameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gaussianVFrameTexture, 0);
}

void LineMode::Destroy()
{
    glDeleteTextures(1, &this->gaussianHFrameTexture);
    glDeleteFramebuffers(1, &this->gaussianHFrameBuffer);
    glDeleteTextures(1, &this->gaussianVFrameTexture);
    glDeleteFramebuffers(1, &this->gaussianVFrameBuffer);

    glDeleteTextures(1, &this->sceneFrameTexture);
    glDeleteFramebuffers(1, &this->sceneFrameBuffer);
    
    glDeleteBuffers(1, &this->quadsBuffer);
    glDeleteBuffers(1, &this->passthroughBuffer);

    glDeleteVertexArrays(1, &this->lineVertexArray);
    glDeleteBuffers(1, &this->lineArrayBuffer);
    glDeleteBuffers(1, &this->lineElementBuffer);
    
    shader::DestroyProgram(this->lineProgram);
    shader::DestroyProgram(this->quadsProgram);
    shader::DestroyProgram(this->passthroughProgram);
    shader::DestroyProgram(this->gaussianHProgram);
    shader::DestroyProgram(this->gaussianVProgram);
}

void LineMode::Update()
{
    int anum = this->points->size() * 3;
    int inum = (this->points->size() - 1) * 2;
    GLfloat *adata = (GLfloat*) malloc(anum * sizeof(GLfloat));
    GLuint *idata = (GLuint*) malloc(inum * sizeof(GLuint));
    GLfloat *aptr = adata;
    GLuint *iptr = idata;
    GLuint count = 0;
    GLfloat size = this->points->size();
    for (std::list<glm::vec3>::iterator it = this->points->begin(); it != this->points->end(); ++it) {
        if (count > 0) {
            *iptr++ = count - 1;
            *iptr++ = count;
        }
        *aptr++ = it->x;
        *aptr++ = it->y;
        *aptr++ = it->z;
        count++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->lineArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * anum, adata, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->lineElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inum, idata, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(adata);
    free(idata);
}

void LineMode::Render()
{
    float time = this->currentFrame.tick / 1000.0;

    // Draw the standard scene
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->sceneFrameBuffer);
    {
        glClear(GL_COLOR_BUFFER_BIT);
        shader::Bind(this->lineProgram);
        {
            glUniform1f(this->uniformLineTime, time);
            glBindVertexArray(this->lineVertexArray);
            {
                int inum = (this->points->size() - 1) * 2;
                glDrawElements(GL_LINES, inum, GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);
        }
        shader::Unbind();
    }

    // Convert it to alpha image
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->alphaSceneFrameBuffer);
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, 0);
        shader::Bind(this->quadsProgram);
        {
            glUniform1f(this->uniformQuadsTime, time);
            glBindBuffer(GL_ARRAY_BUFFER, this->quadsBuffer);
            {
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        shader::Unbind();
        glBindTexture(GL_TEXTURE_2D, this->sceneFrameTexture);
        shader::Bind(this->alphaLineProgram);
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
            {
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        shader::Unbind();
    }

    // Draw to the first blur buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->gaussianHFrameBuffer);
    {
        glBindTexture(GL_TEXTURE_2D, this->alphaSceneFrameTexture);
        glClear(GL_COLOR_BUFFER_BIT);
        shader::Bind(this->gaussianHProgram);
        {
            glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
            {
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        shader::Unbind();
    }
    
    // Do some blurring
    for (int i = 0; i < 8; i++)
    {
        int buf = i % 2 == 0 ? this->gaussianVFrameBuffer : this->gaussianHFrameBuffer;
        int tex = i % 2 == 0 ? this->gaussianHFrameTexture : this->gaussianVFrameTexture;
        shader::program prg = i % 2 == 0 ? this->gaussianVProgram : this->gaussianHProgram;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buf);
        {
            glBindTexture(GL_TEXTURE_2D, tex);
            glClear(GL_COLOR_BUFFER_BIT);
            shader::Bind(prg);
            {
                glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
                {
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            shader::Unbind();
        }
    }

    // Draw the blur to the screen
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, this->gaussianHFrameTexture);
    shader::Bind(this->passthroughProgram);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
        {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    shader::Unbind();

    return;
    
    glBindTexture(GL_TEXTURE_2D, this->alphaSceneFrameTexture);
    shader::Bind(this->passthroughProgram);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
        {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    shader::Unbind();
}

void LineMode::OnMouseMove(int x, int y, int deltaX, int deltaY)
{
    float time = this->currentFrame.tick / 1000.0;
    this->points->push_back(glm::vec3(x, y, time));
    if (this->points->size() > LineMode::MAX_POINTS) this->points->pop_front();
}

void LineMode::OnResize(int w, int h)
{

}