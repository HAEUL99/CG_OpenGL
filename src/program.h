#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"
#include "framework.h"
CLASS_PTR(Program)
class Program {
public:
    static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
    static ProgramUPtr Create(const std::string& vertShaderFilename,
    const std::string& fragShaderFilename);

    ~Program();
    uint32_t Get() const { return m_program; }   
    void Use() const; 

    void Set(int windowWidth, int windowHeight);
    void LoadTexture(std::vector<Vec4f>& image, int windowWidth, int windowHeight);
    void Draw();
    
    // ... in Program class declaration
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    
    uint32_t m_program { 0 };
private:
    Program() {}
    bool Link(const std::vector<ShaderPtr>& shaders);
    
    
    unsigned int vao = 0, vbo = 0, textureId = 0;
};

#endif // __PROGRAM_H__