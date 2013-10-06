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
#include "default_shaders.hpp"

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

const GLuint ATTRIB_VERTEX_COORD = 0;
const GLuint ATTRIB_VERTEX_COLOR = 1;

short indices[] = { 0, 1, 2, 0, 1, 3, 4, 5, 6, 4, 5, 7 };

ArrayBuffer *posBuffer;
ArrayBuffer *colorBuffer;

void redrawDemo()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    testVAO->bind();
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawElements(GL_TRIANGLES, sizeof(testIndices), GL_UNSIGNED_SHORT, (const GLvoid*) 0);
    GLError::check("redrawDemo()");
}

static void prepareDemo()
{
    testFragShader = new FragmentShader("shaders/trivial_color.fsh");
    testVertShader = new VertexShader("shaders/trivial_coord_color.vsh");
    
    testProgram = new ShaderProgram();
    testProgram->attach(*testFragShader);
    testProgram->attach(*testVertShader);
    testProgram->link();
    testProgram->use();
    
    testVAO = new VAO();
    testVAO->bind();

    posBuffer = new ArrayBuffer(GL_STATIC_DRAW);
    posBuffer->data(sizeof(positions), positions);
    colorBuffer = new ArrayBuffer(GL_STATIC_DRAW);
    colorBuffer->data(sizeof(colors), colors);
    
    //GLint attr_coord = testProgram->getAttribLocation("coord");
    //GLint attr_color = testProgram->getAttribLocation("color");

    //GLError::check("prepareDemo: after getAttribLocation()");

    posBuffer->bind();
    glVertexAttribPointer(ATTRIB_VERTEX_COORD, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(0));

    GLError::check("prepareDemo: after glVertexAttribPointer(attr_coord, ...)");

    colorBuffer->bind();
    glVertexAttribPointer(ATTRIB_VERTEX_COLOR, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(0));

    GLError::check("prepareDemo: after glVertexAttribPointer(attr_color, ...)");
    
    glEnableVertexAttribArray(ATTRIB_VERTEX_COORD);
    glEnableVertexAttribArray(ATTRIB_VERTEX_COLOR);
    
    glVertexAttrib4f(ATTRIB_VERTEX_COLOR, 1.0f, 0.0f, 0.0f, 0.0f);
    
    GLError::check("prepareDemo: after glEnableVertexAttribArray()");
    
    navEvHandler.makeCurrent();
    
    App::setRedrawFunc(redrawDemo);
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
