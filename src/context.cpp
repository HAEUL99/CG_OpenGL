#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    //creating vertex array object(정점 정보)
    glGenVertexArrays(1, &m_vertexArrayObject); 
    glBindVertexArray(m_vertexArrayObject);

    m_vertexLayout = VertexLayout::Create();
    //creating vertex buffer object(정점 위치)
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*12);

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glEnableVertexAttribArray(0); // 정점 attribute 중 n 번째를 사용하도록 설정
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); // 정점의 n번째 attribute

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6);

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
    //->Use();
    glUseProgram(m_program->Get());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

    

