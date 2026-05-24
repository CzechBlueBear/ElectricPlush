#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include <string>
#include <glm/glm.hpp>


namespace plush {

class ShaderProgram;

/**
 * Base class for uniforms.
 */
class Uniform {
protected:
    
    /// Name of the uniform.
    std::string m_name;
    
    /// Program-specific numeric index of the uniform (0 is valid, -1 isn't).
    int m_index;
    
public:
    
    Uniform(const std::string &name);

    /// Returns the index of the uniform (-1 if invalid).
    int index() const { return m_index; }

    /// Finds the uniform in the current program and stores its index (-1 if not found).
    bool find();
};

/**
 * A uniform that uses 3-element vectors.
 */
class UniformVec3 : public Uniform
{
public:
    
    UniformVec3(const std::string &name) : Uniform(name) { }
    void set(const glm::vec3 &v);
};

/**
 * A uniform that uses 4x4 matrices.
 */
class UniformMat4 : public Uniform
{
public:
    
    UniformMat4(const std::string &name) : Uniform(name) { }
    void set(const glm::mat4 &v);
};

} // namespace

#endif