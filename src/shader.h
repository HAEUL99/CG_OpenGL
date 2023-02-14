#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

CLASS_PTR(Shader);
/*
class Shader;
using ShaderUPtr = std::unique_ptr<Shader>;
using ShaderPtr = std::unique_ptr<Shader>;
using ShaderWPtr = std::unique_ptr<Shader>;
*/
class Shader {
public:
  static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);
  ~Shader();
  uint32_t Get() const { return m_shader; } 
     
private:
  Shader() {}
  bool LoadFile(const std::string& filename, GLenum shaderType); //초기화 함수
  uint32_t m_shader { 0 }; //opengl shader object의 아이디
};

#endif // __SHADER_H__

/*
void test()
{
    ShaderUPtr shader = Shader::CreateFromFile("shader/simple.vs", GL_VERTEX_SHADER);
}
*/
