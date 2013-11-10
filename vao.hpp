#ifndef VAO_HPP
#define VAO_HPP

#include "gl_object.hpp"
#include "bindable.hpp"

#include <string>

namespace plush {

    class VBO;
    
    /**
     * An OpenGL Vertex Array Object.
     * 
     * The VAO, when bound, records all changes to attribute arrays
     * (how they are linked to appropriate vertex attributes)
     * and resets them to the recorded state when bound again.
     */
    class VAO : public GLObject, public GLBindable {
    public:
        
        VAO();
        ~VAO();
        
        void bind() override;
        void unbind() override;

        /**
         * Enables feeding the attribute from an attribute array.
         */
        static void enableVertexAttribArray(const std::string &attribName);
        
        /**
         * Disables feeding the attribute from an attribute array.
         * After this call, all rendering will use the default value
         * for that attribute.
         */
        static void disableVertexAttribArray(const std::string &attribName);
        
        /**
         * Sets up an attribute array to feed the given attribute from.
         * 
         * Attribute arrays are stored in buffer objects. A single buffer
         * object can optionally contain more than one attribute array,
         * either interleaved, or sequentially placed.
         * The @a stride and @offset arguments are then used to locate
         * the array in the buffer.
         * 
         * @arg attribName Name of the attribute.
         * @arg sourceArray Array to be used as a source.
         * @arg valuesPerVertex Number of values to be used per each rendered vertex.
         * @arg valueType Type of the values used (e.g. GL_FLOAT, GL_INT, etc.)
         * @arg normalize Whether the values should be normalized.
         * @arg stride Distance (in bytes) to add to the array pointer after rendering a vertex. If 0, the values are considered tightly packed.
         * @arg offset Distance (in bytes) where the first value starts in the array.
         */
        static void vertexAttribPointer(const std::string &attribName,
                                        VBO &buf,
                                        unsigned int valueCount, GLenum valueType,
                                        bool normalize=false,
                                        int stride=0, int offset=0);
    };
}

#endif