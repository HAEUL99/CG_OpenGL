#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
    auto shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader); //shader의 소유권을 이전하기 위해
}

Shader::~Shader()
{
    if(m_shader)
    {
        glDeleteShader(m_shader); //shader object 제거
    }

}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    auto result = LoadTextFile(filename);
    if (!result.has_value())
        return false;

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // create and compile shader
    m_shader = glCreateShader(shaderType); //opengl shader object 생성
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength); //shader에 소스 코드 설정
    glCompileShader(m_shader); //shader 컴파일

    // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); //shader에 대한 정수형 정보를 얻어옴
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog); // shader에 대한 로그를 얻어옴. 컴파일 에러 얻어내는 용도로 사용
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }
    return true;
}