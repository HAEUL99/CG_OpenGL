#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
    };

    //creating vertex array object(정점 정보)
    glGenVertexArrays(1, &m_vertexArrayObject); 
    glBindVertexArray(m_vertexArrayObject);

    //creating vertex buffer object(정점 위치)
    glGenBuffers(1, &m_vertexBuffer); //새로운 buffer object를 만든다. -> vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //지금부터 사용할 buffer object를 지정한다, gl_array_buffer: 사용할 buffer object는 vertex data를 저장할 용도임을 알려줌.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW); // 지정된 buffer에 데이터를 복사한다

	glEnableVertexAttribArray(0); // 정점 attribute 중 n 번째를 사용하도록 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // 정점의 n번째 attribute

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});

    if (!m_program)
        return false;
    
    SPDLOG_INFO("program id: {}", m_program->Get()); 

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // 컬러 프레임버퍼 화면을 클리어할 색상 지정

    


    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_program->Get());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
