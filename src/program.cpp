#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
    return nullptr;
    return std::move(program);
}

Program::~Program() {
  if (m_program) {
    glDeleteProgram(m_program); //program object 제거
  }
}

bool Program::Link(
    const std::vector<ShaderPtr>& shaders) {
    m_program = glCreateProgram();  //opengl program object 생성

    for (auto& shader: shaders)
        glAttachShader(m_program, shader->Get()); //program에 shader를 붙이기

    glLinkProgram(m_program); //program 링크 
    int success = 0;

    glGetProgramiv(m_program, GL_LINK_STATUS, &success); //program에 대한 정수형 정보를 얻어옴
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog); //program에 대한 로그를 얻어옴. 링크 에러 얻어내는 용도로 사용
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }
    return true;
}

void Program::Use() const
{
  glUseProgram(m_program);
}

// program.cpp
void Program::SetUniform(const std::string& name, int value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform1i(loc, value);
}

void Program::SetUniform(const std::string& name,
  const glm::mat4& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}