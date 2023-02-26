#include "context.h"
#include "image.h"
#include <imgui.h>


//브랜치테스트
ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init() {
    m_box = Mesh::CreateBox();

    //m_model = Model::Load("./model/backpack.obj");
    m_model = Model::Load("d:/git/CG/opengl_example/model/backpack.obj");

    if (!m_model)
        return false;

    //m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    m_simpleProgram = Program::Create("d:/git/CG/opengl_example/shader/simple.vs", "d:/git/CG/opengl_example/shader/simple.fs");
    if (!m_simpleProgram)
        return false;

    //m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    m_program = Program::Create("d:/git/CG/opengl_example/shader/lighting.vs", "d:/git/CG/opengl_example/shader/lighting.fs");
    if (!m_program)
        return false;


    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // 컬러 프레임버퍼 화면을 클리어할 색상 지정

    
    //auto image = Image::Load("./image/container.jpg");
    auto image = Image::Load("d:/git/CG/opengl_example/image/container.jpg");
    if (!image) 
       return false;
    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get()); //unique 포인터 -> 포인터

    //auto image2 = Image::Load("./image/awesomeface.png");
    auto image2 = Image::Load("d:/git/CG/opengl_example/image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    // m_material.diffuse = Texture::CreateFromImage(Image::Load("./image/container2.png").get());
	// m_material.specular = Texture::CreateFromImage( Image::Load("./image/container2_specular.png").get());

    m_material.diffuse = Texture::CreateFromImage(Image::CreateSingleColorImage(4, 4,
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());

    m_material.specular = Texture::CreateFromImage(Image::CreateSingleColorImage(4, 4,
    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    //m_program->SetUniform("tex", 0);
    //m_program->SetUniform("tex2", 1);

    return true;
}

void Context::Render() {

    if (ImGui::Begin("ui window")) {
        if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("l.position", glm::value_ptr(m_obj.position), 0.01f);
            ImGui::DragFloat3("l.direction", glm::value_ptr(m_obj.direction), 0.03f);
            ImGui::DragFloat3("l.angle", glm::value_ptr(m_obj.angle), 0.03f);
        }
        if(ImGui::Button("reset obj"))
        {
            m_obj.position = glm::vec3(0.0f, 0.0f, 3.0f);
            m_obj.direction = glm::vec3(0.0f, 0.0f, 3.0f);
     

        }
        if(ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
        {
            glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera vaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::DragFloat("field of camera(vertical)", &m_cameraFovy, 0.5f);
        ImGui::DragFloat("field of camera(horizontal)", &m_cameraFovx, 0.01f);
        ImGui::DragFloat("near clipping plane", &m_cameraNear, 0.01f);
        ImGui::DragFloat("far clipping plane", &m_cameraFar, 0.01f);
        ImGui::Separator();
        if(ImGui::Button("reset camera"))
        {
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;

        }
        // if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) {
        //     ImGui::DragFloat3("2.position", glm::value_ptr(m_light.position), 0.01f);
        //     ImGui::DragFloat3("2.direction", glm::value_ptr(m_light.direction), 0.01f);
        //     ImGui::DragFloat("2.cutoff",glm::value_ptr(m_light.cutoff), 0.5f, 0.0f, 180.0f);
        //     ImGui::DragFloat("2.distance", &m_light.distance, 0.5f, 0.0f, 3000.0f);
        //     ImGui::ColorEdit3("2.ambient", glm::value_ptr(m_light.ambient));
        //     ImGui::ColorEdit3("2.diffuse", glm::value_ptr(m_light.diffuse));
        //     ImGui::ColorEdit3("2.specular", glm::value_ptr(m_light.specular));
        // }
    
        // if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen)) {
        //     ImGui::DragFloat("m.shininess", &m_material.shininess, 1.0f, 1.0f, 256.0f);
        // }
        // ImGui::Checkbox("animation", &m_animation);

    }
    ImGui::End();

    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_program->Use();

    m_cameraFront =
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    //cf. fieldOfViewX = 2 * atan(tan(fieldOfViewY * 0.5) * aspect)
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    m_cameraFovx = (2.0f * atanf(tanf(m_cameraFovy * 0.5f) * aspect));
    //m_cameraFovy = 2 * atanf(tanf(m_cameraFovx* 0.5f) / aspect);
    auto projection = glm::perspective(glm::radians(m_cameraFovy), aspect, m_cameraNear, m_cameraFar);
    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    // after computing projection and view matrix
    // auto lightModelTransform = glm::translate(glm::mat4(1.0), m_light.position) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    // m_simpleProgram->Use();
    // m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
    // m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
    // m_box->Draw(m_simpleProgram.get());

    
    m_program->Use();
    m_program->SetUniform("viewPos", m_cameraPos);
    m_program->SetUniform("obj.position", m_obj.position);
    m_program->SetUniform("obj.direction", m_obj.direction);

    // m_program->SetUniform("light.position", m_light.position);
    m_program->SetUniform("light.direction", m_light.direction);
    // m_program->SetUniform("light.cutoff", glm::vec2(
    // cosf(glm::radians(m_light.cutoff[0])),
    // cosf(glm::radians(m_light.cutoff[0] + m_light.cutoff[1]))));
    //m_program->SetUniform("light.attenuation", GetAttenuationCoeff(m_light.distance));
    m_program->SetUniform("light.ambient", m_light.ambient);
    m_program->SetUniform("light.diffuse", m_light.diffuse);
    m_program->SetUniform("light.specular", m_light.specular);

    m_program->SetUniform("material.diffuse", 0);
    m_program->SetUniform("material.specular", 1);
    m_program->SetUniform("material.shininess", m_material.shininess);
    glActiveTexture(GL_TEXTURE0);
    m_material.diffuse->Bind();
    glActiveTexture(GL_TEXTURE1);
    m_material.specular->Bind();

    

    // obj tranform
    auto transitionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_obj.position.x, m_obj.position.y, m_obj.position.z));
    auto rotateTansform = glm::mat4(1.0f);
    auto modelTransform = glm::mat4(1.0f);
    if(m_objWorldControl)
    {
        rotateTansform = glm::rotate(glm::mat4(1.0f), glm::radians(m_obj.angle.x), glm::vec3(1.0, 0.0, 0.0));
        rotateTansform = glm::rotate(rotateTansform, glm::radians(m_obj.angle.y), glm::vec3(0.0, 1.0, 0.0));
        modelTransform = rotateTansform * transitionTransform;
    }
    else
    {
        rotateTansform = glm::rotate(glm::mat4(1.0f), glm::radians(m_obj.direction.x), glm::vec3(1.0, 0.0, 0.0));
        rotateTansform = glm::rotate(rotateTansform, glm::radians(m_obj.direction.y), glm::vec3(0.0, 1.0, 0.0));
        rotateTansform = glm::rotate(rotateTansform, glm::radians(m_obj.direction.z), glm::vec3(0.0, 0.0, 1.0));
        modelTransform = transitionTransform * rotateTansform;
    }

    auto transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());

}

void Context::ProcessInput(GLFWwindow* window) {


    if (!m_cameraControl && !m_objWorldControl)
        return;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_objWorldControlKey = true;
    
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraRight;    

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraUp;
    

}

 void Context::Reshape(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y) {
    if (!m_cameraControl && !m_objControl && !m_objWorldControl)
        return;
    if(m_cameraControl)
    {
        auto pos = glm::vec2((float)x, (float)y);
        auto deltaPos = pos - m_prevMousePos;

        const float cameraRotSpeed = 0.8f;
        m_cameraYaw -= deltaPos.x * cameraRotSpeed;
        m_cameraPitch -= deltaPos.y * cameraRotSpeed;

        // if (m_cameraYaw < 0.0f)   m_cameraYaw += 360.0f;
        // if (m_cameraYaw > 360.0f) m_cameraYaw -= 360.0f;
        if (m_cameraYaw < 89.0f)   m_cameraYaw += 89.0f;
        if (m_cameraYaw > -89.0f) m_cameraYaw -= 89.0f;

        if (m_cameraPitch > 89.0f)  m_cameraPitch = 89.0f;
        if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;

        m_prevMousePos = pos;     
    }
    if(m_objControl)
    {
        auto pos = glm::vec2((float)x, (float)y);
        auto deltaPos = pos - m_prevMousePos_obj;

        const float objRotSpeed = 0.8f;
        m_obj.direction.x -= deltaPos.y * objRotSpeed;
        m_obj.direction.y -= deltaPos.x * objRotSpeed;

        if (m_obj.direction.x < 0.0f)   m_obj.direction.x += 360.0f;
        if (m_obj.direction.x > 360.0f) m_obj.direction.x -= 360.0f;

        if (m_obj.direction.y < 0.0f)  m_obj.direction.y += 360.0f;
        if (m_obj.direction.y > 360.0f) m_obj.direction.y -= 360.0f;

        //if (m_obj.direction.z < 0.0f)  m_obj.direction.z += 360.0f;
        //if (m_obj.direction.z > 360.0f) m_obj.direction.z -= 360.0f;

        m_prevMousePos_obj = pos; 

    }
    if(m_objWorldControl && m_objWorldControlKey)
    {
        auto pos = glm::vec2((float)x, (float)y);
        auto deltaPos = pos - m_prevMousePos_objWorld;

        const float objRotSpeed = 0.8f;
        m_obj.angle.x -= deltaPos.y * objRotSpeed;
        m_obj.angle.y -= deltaPos.x * objRotSpeed;

        if (m_obj.angle.x < 0.0f)   m_obj.angle.x += 360.0f;
        if (m_obj.angle.x > 360.0f) m_obj.angle.x -= 360.0f;

        if (m_obj.angle.y < 0.0f)  m_obj.angle.y += 360.0f;
        if (m_obj.angle.y > 360.0f) m_obj.angle.y -= 360.0f;

        //if (m_obj.direction.z < 0.0f)  m_obj.direction.z += 360.0f;
        //if (m_obj.direction.z > 360.0f) m_obj.direction.z -= 360.0f;

        m_prevMousePos_objWorld = pos; 

    }
}

void Context::MouseButton(int button, int action, double x, double y) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            // 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
            m_prevMousePos = glm::vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE) {
            m_cameraControl = false;
        }
    }

    if(button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if(action == GLFW_PRESS)
        {
            m_prevMousePos_obj = glm::vec2((float)x, (float)y);
            m_objControl = true;
        }
        else if(action == GLFW_RELEASE)
        {
            m_objControl = false;
        }

    }

    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS)
        {
            m_prevMousePos_objWorld = glm::vec2((float)x, (float)y);
            m_objWorldControl = true;
        }
        else if(action == GLFW_RELEASE)
        {
            m_objWorldControl = false;
        }

    }
}   


