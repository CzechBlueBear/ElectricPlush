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
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace plush;

class MyApp : public App {
protected:
    
    FragmentShader testFragShader;
    VertexShader testVertShader;
    ShaderProgram testProgram;

    GLuint uniform_modelMatrix;
    GLuint uniform_viewMatrix;
    GLuint uniform_projectionMatrix;
    GLuint uniform_normalMatrix;
    GLuint uniform_hollywoodLight;

    Camera testCamera;

    Cube testCube;

public:
    
    MyApp();
    void onRedraw() override;
    void onKey(const SDL_KeyboardEvent &event) override;
};

MyApp::MyApp()
    : testFragShader("shaders/trivial_color.fsh"),
    testVertShader("shaders/transform_coord_color_normal.vsh")
{
    testProgram.attach(testFragShader);
    testProgram.attach(testVertShader);
    testProgram.link();
    testProgram.use();

    uniform_modelMatrix = testProgram.getUniformLocation("modelMatrix");
    uniform_projectionMatrix = testProgram.getUniformLocation("projectionMatrix");
    uniform_viewMatrix = testProgram.getUniformLocation("viewMatrix");
    uniform_normalMatrix = testProgram.getUniformLocation("normalMatrix");
    uniform_hollywoodLight = testProgram.getUniformLocation("hollywoodLight");

    testCamera.setCoord(glm::vec3(0.0f, 0.0f, 1.5f));
}

void MyApp::onRedraw()
{
    float northLightFactor = cos(testCamera.getAzimuth()/180.0f*M_PI);
    glm::vec3 horizonColor = glm::vec3(0.2 + 0.3*northLightFactor,
                                       0.2 + 0.3*northLightFactor,
                                       0.4 + 0.4*northLightFactor);
    if (horizonColor[2] < 0.1f)
            horizonColor[2] = 0.1f;

    glClearColor(horizonColor[0], horizonColor[1], horizonColor[2], 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    testCamera.upload(uniform_viewMatrix, uniform_projectionMatrix);
    
    glm::vec3 hollywoodLight = glm::vec3(0.0f, 0.0f, 1.0f);
    glUniform3fv(uniform_hollywoodLight, 1, glm::value_ptr(hollywoodLight));

    testCube.render();
    
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
                testCamera.setAzimuth(testCamera.getAzimuth() - 4);
                std::cerr << "azimuth now: " << testCamera.getAzimuth() << "\n";
                break;
                
            case SDLK_RIGHT:
                testCamera.setAzimuth(testCamera.getAzimuth() + 4);
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
