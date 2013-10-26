#include "cube.hpp"
#include "vertex_format.hpp"

#include <cstddef>

using namespace plush;

const glm::vec3 LEFT_BOTTOM_FRONT (-.5f, -.5f, .5f);
const glm::vec3 RIGHT_BOTTOM_FRONT( .5f, -.5f, .5f);
const glm::vec3 RIGHT_TOP_FRONT   ( .5f,  .5f, .5f);
const glm::vec3 LEFT_TOP_FRONT    (-.5f,  .5f, .5f);
const glm::vec3 LEFT_BOTTOM_BACK  (-.5f, -.5f, -.5f);
const glm::vec3 RIGHT_BOTTOM_BACK ( .5f, -.5f, -.5f);
const glm::vec3 RIGHT_TOP_BACK    ( .5f,  .5f, -.5f);
const glm::vec3 LEFT_TOP_BACK     (-.5f,  .5f, -.5f);

const glm::vec3 NORMAL_LEFT     (-1.0f,  0.0f,  0.0f);
const glm::vec3 NORMAL_RIGHT    ( 1.0f,  0.0f,  0.0f);
const glm::vec3 NORMAL_UP       ( 0.0f,  1.0f,  0.0f);
const glm::vec3 NORMAL_DOWN     ( 0.0f, -1.0f,  0.0f);
const glm::vec3 NORMAL_FRONT    ( 0.0f,  0.0f,  1.0f);
const glm::vec3 NORMAL_BACK     ( 0.0f,  0.0f, -1.0f);

static const TexturedVertex cubeVertices[] = {
    
    // front side
    { LEFT_BOTTOM_FRONT,  NORMAL_FRONT, glm::vec2(0.0f, 0.0f), },
    { RIGHT_BOTTOM_FRONT, NORMAL_FRONT, glm::vec2(0.0f, 1.0f), },
    { RIGHT_TOP_FRONT,    NORMAL_FRONT, glm::vec2(1.0f, 1.0f), },
    { LEFT_TOP_FRONT,     NORMAL_FRONT, glm::vec2(1.0f, 1.0f), },
    
    // back side
    { LEFT_BOTTOM_BACK,   NORMAL_BACK,  glm::vec2(0.0f, 0.0f), },
    { RIGHT_BOTTOM_BACK,  NORMAL_BACK,  glm::vec2(0.0f, 0.0f), },
    { RIGHT_TOP_BACK,     NORMAL_BACK,  glm::vec2(0.0f, 0.0f), },
    { LEFT_TOP_BACK,      NORMAL_BACK,  glm::vec2(0.0f, 0.0f), },

    // left side
    { LEFT_BOTTOM_FRONT,  NORMAL_LEFT, },
    { LEFT_BOTTOM_BACK,   NORMAL_LEFT, },
    { LEFT_TOP_BACK,      NORMAL_LEFT, },
    { LEFT_TOP_FRONT,     NORMAL_LEFT, },
    
    // right side
    { RIGHT_BOTTOM_FRONT, NORMAL_RIGHT, },
    { RIGHT_BOTTOM_BACK,  NORMAL_RIGHT, },
    { RIGHT_TOP_BACK,     NORMAL_RIGHT, },
    { RIGHT_TOP_FRONT,    NORMAL_RIGHT, },
    
    // bottom side
    { LEFT_BOTTOM_FRONT,  NORMAL_DOWN, },
    { RIGHT_BOTTOM_FRONT, NORMAL_DOWN, },
    { RIGHT_BOTTOM_BACK,  NORMAL_DOWN, },
    { LEFT_BOTTOM_BACK,   NORMAL_DOWN, },
    
    // top side
    { LEFT_TOP_FRONT,     NORMAL_UP, },
    { RIGHT_TOP_FRONT,    NORMAL_UP, },
    { RIGHT_TOP_BACK,     NORMAL_UP, },
    { LEFT_TOP_BACK,      NORMAL_UP, },
};

static const short cubeIndices[] = {
    0, 1, 2, 0, 2, 3,                   // front side
    4, 5, 6, 4, 6, 7,                   // back side
    8, 9, 10, 8, 10, 11,                // left side
    12, 13, 14, 12, 14, 15,             // right side
    16, 17, 18, 16, 18, 19,             // bottom side
    20, 21, 22, 20, 22, 23,             // top side
};

/////////////////////////////////////////////////////////////////////////////
// CubeModel
/////////////////////////////////////////////////////////////////////////////

CubeModel *CubeModel::s_instance = nullptr;

CubeModel::CubeModel()
    : coordsVBO(GL_STATIC_DRAW), indicesVBO(GL_STATIC_DRAW)
{
    VAOBinder vaoBinder(vao);

    coordsVBO.data(sizeof(cubeVertices), cubeVertices);
    
    VAO::vertexAttribPointer("vertexCoord", coordsVBO, 3, GL_FLOAT, false, sizeof(TexturedVertex), offsetof(TexturedVertex, coord));
    VAO::enableVertexAttribArray("vertexCoord");
    VAO::vertexAttribPointer("vertexNormal", coordsVBO, 3, GL_FLOAT, false, sizeof(TexturedVertex), offsetof(TexturedVertex, normal));
    VAO::enableVertexAttribArray("vertexNormal");

    indicesVBO.data(sizeof(cubeIndices), cubeIndices);
}

CubeModel::~CubeModel()
{
}

CubeModel *CubeModel::instance()
{
    if (!s_instance)
        s_instance = new CubeModel();
    return s_instance;
}

void CubeModel::render()
{
    vao.bind();

    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices)/3*sizeof(float), GL_UNSIGNED_SHORT, (GLvoid*) 0);
}

/////////////////////////////////////////////////////////////////////////////
// Cube
/////////////////////////////////////////////////////////////////////////////

Cube::Cube()
{
}

void Cube::prepare()
{
    (void) CubeModel::instance();
}

void Cube::render()
{
    CubeModel::instance()->render();    
}
