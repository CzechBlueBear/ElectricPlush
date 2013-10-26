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
#include "cube.hpp"

#include <array>
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace plush;

class MyApp : public App {
protected:
    
    FragmentShader *testFragShader;
    VertexShader *testVertShader;
    ShaderProgram *testProgram;

    GLuint uniform_modelMatrix;
    GLuint uniform_viewMatrix;
    GLuint uniform_projectionMatrix;
    GLuint uniform_normalMatrix;
    GLuint uniform_hollywoodLight;

    Camera testCamera;

    Cube *testCube;

public:
    
    MyApp();
    void onRedraw() override;
    void onKey(const SDL_KeyboardEvent &event) override;
};

MyApp::MyApp()
{
    testFragShader = new FragmentShader("shaders/lighted_color.fsh");
    testVertShader = new VertexShader("shaders/transform_coord_color_normal.vsh");

    testProgram = new ShaderProgram();
    testProgram->attach(*testFragShader);
    testProgram->attach(*testVertShader);
    testProgram->link();
    testProgram->use();

    uniform_modelMatrix = testProgram->getUniformLocation("modelMatrix");
    uniform_projectionMatrix = testProgram->getUniformLocation("projectionMatrix");
    uniform_viewMatrix = testProgram->getUniformLocation("viewMatrix");
    uniform_normalMatrix = testProgram->getUniformLocation("normalMatrix");
    uniform_hollywoodLight = testProgram->getUniformLocation("hollywoodLight");

    testCamera.setCoord(glm::vec3(0.0f, 0.0f, 1.5f));
    
    testCube = new Cube();
    testCube->prepare();
}

void MyApp::onRedraw()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    testCamera.upload(uniform_modelMatrix, uniform_viewMatrix, uniform_projectionMatrix, uniform_normalMatrix);
    
    glm::vec3 hollywoodLight = glm::vec3(testCamera.viewMatrix() * glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f));
    glUniform3fv(uniform_hollywoodLight, GL_FALSE, glm::value_ptr(hollywoodLight));

    testCube->render();
    
    GLError::check("redrawDemo()");
}

void MyApp::onKey(const SDL_KeyboardEvent &event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.keysym.sym) {
            case SDLK_UP:
                testCamera.setCoord(testCamera.getCoord() + 0.2f * testCamera.walkVector());
                break;
                
            case SDLK_DOWN:
                testCamera.setCoord(testCamera.getCoord() - 0.2f * testCamera.walkVector());
                break;
                
            case SDLK_LEFT:
                testCamera.setAzimuth(testCamera.getAzimuth() - 1);
                std::cerr << "azimuth now: " << testCamera.getAzimuth() << "\n";
                break;
                
            case SDLK_RIGHT:
                testCamera.setAzimuth(testCamera.getAzimuth() + 1);
                std::cerr << "azimuth now: " << testCamera.getAzimuth() << "\n";
                break;
            
            case SDLK_PAGEDOWN:
                testCamera.setElevation(testCamera.getElevation() - 1);
                break;
            
            case SDLK_PAGEUP:
                testCamera.setElevation(testCamera.getElevation() + 1);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    MyApp app;
    
    if (!app.ok())
        return 1;
 
    app.eventLoop();
    
    return 0;
}
