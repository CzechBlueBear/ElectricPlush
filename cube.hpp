#ifndef CUBE_HPP
#define CUBE_HPP

#include "gl.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "scene_object.hpp"

#include <glm/glm.hpp>

namespace plush {
    
    class CubeModel {
    protected:
        
        VAO vao;
        ArrayBuffer coordsVBO;
        ElementArrayBuffer indicesVBO;

        static CubeModel *s_instance;
        
        CubeModel();
        CubeModel(const CubeModel &src) = delete;
        CubeModel &operator =(const CubeModel &src) = delete;
        
    public:

        ~CubeModel();
        static CubeModel *instance();
        void render();
    };

    class Cube : public SceneObject {
    public:

        Cube();
        void prepare() override;
        void render() override;
    };
}

#endif
