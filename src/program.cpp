#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
    return nullptr;
    return std::move(program);
}

ProgramUPtr Program::Create( const std::string& vertShaderFilename, const std::string& fragShaderFilename) {
  ShaderPtr vs = Shader::CreateFromFile(vertShaderFilename,
    GL_VERTEX_SHADER);
  ShaderPtr fs = Shader::CreateFromFile(fragShaderFilename,
    GL_FRAGMENT_SHADER);
  if (!vs || !fs)
    return nullptr;
  return std::move(Create({vs, fs}));
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

    //글로벌
    //glBindFragDataLocation(m_program,0,"fragmentColor");

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

void Program::Set(int windowWidth, int windowHeight)
{
  glGenVertexArrays(1, &vao);	// create 1 vertex array object
  glBindVertexArray(vao);		// make it active

  unsigned int vbo;		// vertex buffer objects
  glGenBuffers(1, &vbo);	// Generate 1 vertex buffer objects

  // vertex coordinates: vbo0 -> Attrib Array 0 -> vertexPosition of the vertex shader
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // make it active, it is an array
  float vertexCoords[] = { -1, -1,  1, -1,  1, 1,  -1, 1 };	// two triangles forming a quad
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  glGenTextures(1, &textureId);  				// id generation
  glBindTexture(GL_TEXTURE_2D, textureId);    // binding
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // sampling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Program::LoadTexture(std::vector<vec4>& image, int windowWidth, int windowHeight) 
{
		glBindTexture(GL_TEXTURE_2D, textureId);    // binding
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, &image[0]); // To GPU
}

void Program:: Draw() {
  glBindVertexArray(vao);	// make the vao and its vbos active playing the role of the data source
  int location = glGetUniformLocation(m_program, "textureUnit");
  const unsigned int textureUnit = 0;
  if (location >= 0) {
    glUniform1i(location, textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureId);
  }
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);	// draw two triangles forming a quad
}
// program.cpp
void Program::SetUniform(const std::string& name, int value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform1i(loc, value);
}

void Program::SetUniform(const std::string& name, const glm::mat4& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, float value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string& name, const glm::vec2& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::vec3& value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name,
  const glm::vec4& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform4fv(loc, 1, glm::value_ptr(value));
}
