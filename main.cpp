#include "app.hpp"
#include "gl.hpp"
#include "gl_error.hpp"
#include "event_handler.hpp"
#include "navigation_event_handler.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "vertex_format.hpp"
#include "camera.hpp"

#include <array>
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

using namespace plush;

NavigationEventHandler navEvHandler;

VAO *testVAO;
FragmentShader *testFragShader;
VertexShader *testVertShader;
ShaderProgram *testProgram;

glm::vec3 positions[] = {
    { glm::vec3(-1.0f, -1.0f, 0.0f) },
    { glm::vec3(1.0f, -1.0f, 0.0f) },
    { glm::vec3(1.0f, 1.0f, 0.0f) },
    
    { glm::vec3(0.5f, 0.5f, 0.0f) },
    { glm::vec3(-0.5f, 0.5f, 0.0f) },
    { glm::vec3(-0.5f, -0.5f, 0.0f) },
};

glm::vec3 colors[] = {
    { glm::vec3(1.0f, 0.0f, 0.0f) },
    { glm::vec3(0.0f, 1.0f, 0.0f) },
    { glm::vec3(0.0f, 0.0f, 1.0f) },
    { glm::vec3(1.0f, 0.0f, 0.0f) },
    { glm::vec3(0.0f, 1.0f, 0.0f) },
    { glm::vec3(0.0f, 0.0f, 1.0f) }
};

GLuint attrib_vertexCoord;
GLuint attrib_vertexColor;
GLuint uniform_modelMatrix;
GLuint uniform_viewMatrix;
GLuint uniform_projectionMatrix;

short indices[] = { 0, 1, 2, 0, 1, 3, 4, 5, 6, 4, 5, 7 };

ArrayBuffer *posBuffer;
ArrayBuffer *colorBuffer;

Camera testCamera(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f));

void redrawDemo()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    testVAO->bind();
    
    testCamera.upload(uniform_modelMatrix, uniform_viewMatrix, uniform_projectionMatrix);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawElements(GL_TRIANGLES, sizeof(testIndices), GL_UNSIGNED_SHORT, (const GLvoid*) 0);
    GLError::check("redrawDemo()");
}

void onKey(const SDL_KeyboardEvent &event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.keysym.sym) {
            case SDLK_UP:
                testCamera.setCoord(testCamera.getCoord() + 0.2f * testCamera.forwardVector());
                break;
                
            case SDLK_DOWN:
                testCamera.setCoord(testCamera.getCoord() - 0.2f * testCamera.forwardVector());
                break;
                
            case SDLK_LEFT:
                testCamera.setAzimuth(testCamera.getAzimuth() - 0.5f);
                std::cerr << "azimuth now: " << testCamera.getAzimuth() << "\n";
                break;
                
            case SDLK_RIGHT:
                testCamera.setAzimuth(testCamera.getAzimuth() + 0.5f);
                std::cerr << "azimuth now: " << testCamera.getAzimuth() << "\n";
                break;
        }
    }
}

static void prepareDemo()
{
    testFragShader = new FragmentShader("shaders/trivial_color.fsh");
    testVertShader = new VertexShader("shaders/transform_coord_color.vsh");
    
    testProgram = new ShaderProgram();
    testProgram->attach(*testFragShader);
    testProgram->attach(*testVertShader);
    testProgram->link();
    testProgram->use();

    attrib_vertexCoord = testProgram->getAttribLocation("vertexCoord");
    attrib_vertexColor = testProgram->getAttribLocation("vertexColor");
    uniform_modelMatrix = testProgram->getUniformLocation("modelMatrix");
    uniform_projectionMatrix = testProgram->getUniformLocation("projectionMatrix");
    uniform_viewMatrix = testProgram->getUniformLocation("viewMatrix");
    
    testVAO = new VAO();
    testVAO->bind();

    posBuffer = new ArrayBuffer(GL_STATIC_DRAW);
    posBuffer->data(sizeof(positions), positions);
    colorBuffer = new ArrayBuffer(GL_STATIC_DRAW);
    colorBuffer->data(sizeof(colors), colors);

    posBuffer->bind();
    glVertexAttribPointer(attrib_vertexCoord, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(0));

    GLError::check("prepareDemo: after glVertexAttribPointer(attr_coord, ...)");

    colorBuffer->bind();
    glVertexAttribPointer(attrib_vertexColor, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(0));

    GLError::check("prepareDemo: after glVertexAttribPointer(attr_color, ...)");
    
    glEnableVertexAttribArray(attrib_vertexCoord);
    glEnableVertexAttribArray(attrib_vertexColor);
    
    GLError::check("prepareDemo: after glEnableVertexAttribArray()");
    
    navEvHandler.makeCurrent();
    
    App::setRedrawFunc(redrawDemo);
    App::setKeyFunc(onKey);
}

int main(int argc, char **argv)
{
    if (!App::init())
        return 1;

    prepareDemo(); 
    App::eventLoop();
    
finish:
    App::finish();
    return 0;
}
