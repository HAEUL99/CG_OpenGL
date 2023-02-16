#include "context.h"
#include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    //[x y z r g b s t]
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    //creating vertex array object(정점 정보)
    //creating Vertex Array Object
    m_vertexLayout = VertexLayout::Create();
    //creating vertex buffer object(정점 위치)
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float)*32);

    //0번 attribute는 floating point값이 3개, normalize false, 그다음 attribute까지 바이트 수, 초기 offset
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3); // color가 시작되는 값.
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t)*6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});

    if (!m_program)
        return false;
    
    SPDLOG_INFO("program id: {}", m_program->Get()); 


    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // 컬러 프레임버퍼 화면을 클리어할 색상 지정

    
    auto image = Image::Load("./image/container.jpg");
    if (!image) 
       return false;
    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get()); //unique 포인터 -> 포인터

    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0); //0번 슬롯의 텍스처
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);
    

    // 0.5배 축소후 z축으로 90도 회전하는 행렬
    auto transform = glm::rotate(
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
        glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)
        );

    auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    m_program->Use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

    

