#include "cube.hpp"

using namespace plush;

const glm::vec3 LEFT_BOTTOM_FRONT (-.5f, -.5f, .5f);
const glm::vec3 RIGHT_BOTTOM_FRONT( .5f, -.5f, .5f);
const glm::vec3 RIGHT_TOP_FRONT   ( .5f,  .5f, .5f);
const glm::vec3 LEFT_TOP_FRONT    (-.5f,  .5f, .5f);
const glm::vec3 LEFT_BOTTOM_BACK  (-.5f, -.5f, -.5f);
const glm::vec3 RIGHT_BOTTOM_BACK ( .5f, -.5f, -.5f);
const glm::vec3 RIGHT_TOP_BACK    ( .5f,  .5f, -.5f);
const glm::vec3 LEFT_TOP_BACK     (-.5f,  .5f, -.5f);

static const glm::vec3 cubeCoords[] = {
  
    // front side
    LEFT_BOTTOM_FRONT, RIGHT_BOTTOM_FRONT, RIGHT_TOP_FRONT, LEFT_TOP_FRONT,
    
    // back side
    LEFT_BOTTOM_BACK, RIGHT_BOTTOM_BACK, RIGHT_TOP_BACK, LEFT_TOP_BACK,
    
    // left side
    LEFT_BOTTOM_FRONT, LEFT_BOTTOM_BACK, LEFT_TOP_BACK, LEFT_TOP_FRONT,
    
    // right side
    RIGHT_BOTTOM_FRONT, RIGHT_BOTTOM_BACK, RIGHT_TOP_BACK, RIGHT_TOP_FRONT,
    
    // bottom side
    LEFT_BOTTOM_FRONT, RIGHT_BOTTOM_FRONT, RIGHT_BOTTOM_BACK, LEFT_BOTTOM_BACK,
    
    // top side
    LEFT_TOP_FRONT, RIGHT_TOP_FRONT, RIGHT_TOP_BACK, LEFT_TOP_BACK,
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

    coordsVBO.data(sizeof(cubeCoords), cubeCoords);
    VAO::vertexAttribPointer("vertexCoord", coordsVBO, 3, GL_FLOAT, false, 0, 0);
    VAO::enableVertexAttribArray("vertexCoord");

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
