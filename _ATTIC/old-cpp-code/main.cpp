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
#include "uniform.hpp"

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

    UniformMat4 uniform_modelMatrix;
    UniformMat4 uniform_viewMatrix;
    UniformMat4 uniform_projectionMatrix;
    UniformVec3 uniform_normalMatrix;
    UniformVec3 uniform_hollywoodLight;

    Camera testCamera;

    Cube testCube;

public:
    
    MyApp();
    void onRedraw() override;
    void onKey(const SDL_KeyboardEvent &event) override;
};

MyApp::MyApp()
    : testFragShader("shaders/trivial_color.fsh"),
    testVertShader("shaders/transform_coord_color_normal.vsh"),
    uniform_modelMatrix("modelMatrix"),
    uniform_viewMatrix("viewMatrix"),
    uniform_projectionMatrix("projectionMatrix"),
    uniform_normalMatrix("normalMatrix"),
    uniform_hollywoodLight("hollywoodLight")
{
    testProgram.attach(testFragShader);
    testProgram.attach(testVertShader);
    testProgram.link();
    testProgram.use();

    uniform_modelMatrix.find();
    uniform_projectionMatrix.find();
    uniform_viewMatrix.find();
    uniform_normalMatrix.find();
    uniform_hollywoodLight.find();

    testCamera.setCoord(glm::vec3(0.0f, 0.0f, 1.5f));
}

void MyApp::onRedraw()
{
    float northLightFactor = cos(testCamera.getAzimuth()/180.0f*M_PI);
    glm::vec3 horizonColor = glm::vec3(0.2 + 0.1*northLightFactor,
                                       0.2 + 0.1*northLightFactor,
                                       0.4 + 0.1*northLightFactor);
    if (horizonColor[2] < 0.1f)
            horizonColor[2] = 0.1f;

    glClearColor(horizonColor[0], horizonColor[1], horizonColor[2], 1.0f);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    testCamera.upload(uniform_viewMatrix, uniform_projectionMatrix);
  
    uniform_hollywoodLight.set(glm::vec3(0.0f, 0.0f, 1.0f));

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
