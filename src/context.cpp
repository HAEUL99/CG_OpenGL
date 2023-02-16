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

    //opengl text obj를 하나만들어서 그 아이디를 m_texture에 저장
    glGenTextures(1, &m_texture);
    //내가 앞으로 이용할 2d 텍스처는 m_texture이 가리키는 텍스처이다. 
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //S-> x축
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //T-> y축
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //cpu 이미지 데이터를 gpu 데이터로 이동
    //gl_texture_2d라는 타겟을 레벨값 0, GL_RGB(텍스처의 포맷),width, height, border, image의 포맷, 이미지가 하나의 채널을 표현하는 데이터 사이즈, image가 들어있는 포인터
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
        image->GetWidth(), image->GetHeight(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, image->GetData());

    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    m_program->Use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

    

