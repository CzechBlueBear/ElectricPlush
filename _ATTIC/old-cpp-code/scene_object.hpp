#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

namespace plush {
    
    /// Base class for objects in the 3d scene.
    class SceneObject {
    public:

        /**
         * Constructor.
         * Only non-OpenGL initialization should be done here (the OpenGL context
         * may not exist or be properly initialized yet).
         * All OpenGL-specific initialization should be done in prepare().
         */
        SceneObject() { };
        
        /**
         * Destructor.
         * All resources allocated by prepare() should be freed here, except
         * for shared ones.
         */
        virtual ~SceneObject();
        
        /**
         * Prepares the object for rendering (e.g. setting up VAOs and VBOs,
         * loading textures etc.)
         * The OpenGL context is guaranteed to be fully initialized and usable
         * at the time of calling, but the method should not draw anything visible
         * (it can draw to textures).
         * It is expected for this to take some time.
         * Repeated calls to prepare() must be safe.
         */
        virtual void prepare() = 0;
        
        /**
         * Renders the object using the current OpenGL context.
         */
        virtual void render() = 0;
    };
}

#endif