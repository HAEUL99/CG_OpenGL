#include "context.h"
#include "image.h"
#include <imgui.h>
#include "framework.h"
#include "Scene.h"
#include <tuple>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>


ContextUPtr Context::Create(bool isLocal) {
    auto context = ContextUPtr(new Context());
    if(isLocal)
    {
        if (!context->Init())
            return nullptr;
    }
    else
    {
        if (!context->GlobalInit())
            return nullptr;
    }
    return std::move(context);
}


void Context::Destroy()
{
    //delete[] context;
}

bool Context::Init() {
    m_box = Mesh::CreateBox();
    m_cow = Mesh::CreateBox();

    m_model = Model::Load("./model/backpack.obj");
    if (!m_model)
        return false;


    m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
    if (!m_simpleProgram)
        return false;

    m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
    if (!m_program)
        return false;

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // 컬러 프레임버퍼 화면을 클리어할 색상 지정

    TexturePtr grayTexture = Texture::CreateFromImage(
    Image::CreateSingleColorImage(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());
    
    m_planeMaterial = Material::Create();
    m_planeMaterial->diffuse = Texture::CreateFromImage(Image::Load("./image/marble.jpg").get());
    m_planeMaterial->specular = grayTexture;
    m_planeMaterial->shininess = 128.0f;


    cow_Material = Material::Create();
    cow_Material->diffuse = Texture::CreateFromImage(Image::Load("./image/cow-tex-fin.jpg").get());
    cow_Material->specular = grayTexture;
    cow_Material->shininess = 128.0f;

    return true;
}

void Context::Render() {

    if (ImGui::Begin("ui window")) {
        if(ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
        {
            glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
        ImGui::DragFloat("camera vaw", &m_cameraYaw, 0.5f);
        ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if(ImGui::Button("reset camera"))
        {
            m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            m_cameraYaw = 0.0f;
            m_cameraPitch = 0.0f;

        }
        if (ImGui::CollapsingHeader("1.light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("1.bool", &IsLight);
            ImGui::DragFloat3("l.direction", glm::value_ptr(m_light.position), 0.01f);
            ImGui::DragFloat("1.distance", &m_light.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
            ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
            ImGui::ColorEdit3("l.specular", glm::value_ptr(m_light.specular));
        }
        if (ImGui::CollapsingHeader("2.light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("2.bool", &IsLight1);
            ImGui::DragFloat3("2.direction", glm::value_ptr(m_light1.position), 0.01f);
            ImGui::DragFloat("2.distance", &m_light1.distance, 0.5f, 0.0f, 3000.0f);
            ImGui::ColorEdit3("2.ambient", glm::value_ptr(m_light1.ambient));
            ImGui::ColorEdit3("2.diffuse", glm::value_ptr(m_light1.diffuse));
            ImGui::ColorEdit3("2.specular", glm::value_ptr(m_light1.specular));
        }
        if (ImGui::CollapsingHeader("Global illumination", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("Global", &IsLocal);
        }   


    }
    ImGui::End();

    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_program->Use();

    m_cameraFront =
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    auto projection = glm::perspective(glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 20.0f);


    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

    // after computing projection and view matrix
    auto lightModelTransform = glm::translate(glm::mat4(1.0), m_light.position) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    m_simpleProgram->Use();
    m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
    m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
    m_box->Draw(m_simpleProgram.get());

    auto lightModelTransform1 = glm::translate(glm::mat4(1.0), m_light1.position) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    m_simpleProgram->Use();
    m_simpleProgram->SetUniform("color", glm::vec4(m_light1.ambient + m_light1.diffuse, 1.0f));
    m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform1);
    m_box->Draw(m_simpleProgram.get());

    

    
    m_program->Use();
    m_program->SetUniform("viewPos", m_cameraPos);
    m_program->SetUniform("obj.position", m_obj.position);
    m_program->SetUniform("obj.direction", m_obj.direction);

    //light
    m_program->SetUniform("pointLights[0].position", m_light.position);
    m_program->SetUniform("pointLights[0].ambient", m_light.ambient);
    m_program->SetUniform("pointLights[0].diffuse", m_light.diffuse);
    m_program->SetUniform("pointLights[0].specular", m_light.specular);
    m_program->SetUniform("pointLights[0].constant", 1.0f);
    m_program->SetUniform("pointLights[0].linear", 0.09f);
    m_program->SetUniform("pointLights[0].quadratic", 0.032f);
    m_program->SetUniform("pointLights[0].IsOn", true);

    if(IsLight == false)
    {
       m_program->SetUniform("pointLights[0].IsOn", false);
    }
    if(IsLight == true)
    {
        m_program->SetUniform("pointLights[0].IsOn", true);
    }
    
    //light1
    m_program->SetUniform("pointLights[1].position", m_light1.position);
    m_program->SetUniform("pointLights[1].ambient", m_light1.ambient);
    m_program->SetUniform("pointLights[1].diffuse", m_light1.diffuse);
    m_program->SetUniform("pointLights[1].specular", m_light1.specular);
    m_program->SetUniform("pointLights[1].constant", 1.0f);
    m_program->SetUniform("pointLights[1].linear", 0.09f);
    m_program->SetUniform("pointLights[1].quadratic", 0.032f);
    m_program->SetUniform("pointLights[1].IsOn", true);
    if(IsLight1 == false)
    {
       m_program->SetUniform("pointLights[1].IsOn", false);
    }
    if(IsLight1 == true)
    {
        m_program->SetUniform("pointLights[1].IsOn", true);
    }
        
    m_program->SetUniform("material.diffuse", 0.5f);
    m_program->SetUniform("material.specular", 1);
    //m_program->SetUniform("material.shininess", m_material.shininess);


    //ground 
    auto modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 0.3f, 10.0f));
    auto transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    //wall left
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 10.0f, 10.0f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    //wall right
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 10.0f, 10.0f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    //wall back
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f,-5.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.3f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_planeMaterial->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());

    //cow box
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f,0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    cow_Material->SetToProgram(m_program.get());
    m_box->Draw(m_program.get());


    {
        //bag 1
    // modelTransform = 
    //     glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, 0.0f)) *
    //     glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
    // modelTransform = glm::rotate(modelTransform, -90.0f, glm::vec3(1.0, 0.0, 0.0));
    // transform = projection * view * modelTransform;
    // m_program->SetUniform("transform", transform);
    // m_program->SetUniform("modelTransform", modelTransform);
    // m_model->Draw(m_program.get());

    

    //bag 2
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 3.0f, 2.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    modelTransform = glm::rotate(modelTransform, 90.0f, glm::vec3(0.0, 1.0, 0.0));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());

    //bag 3
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -3.0f, 2.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    modelTransform = glm::rotate(modelTransform, 60.0f, glm::vec3(0.0, 0.0, 1.0));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());

    //bag 4
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -3.0f, -1.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    modelTransform = glm::rotate(modelTransform, 90.0f, glm::vec3(1.0, 1.0, 1.0));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());

    //bag 5
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.5f, 2.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    modelTransform = glm::rotate(modelTransform, 35.0f, glm::vec3(1.0, 1.0, 1.0));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());

    //bag 6
    modelTransform = 
        glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -3.0f, 2.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    modelTransform = glm::rotate(modelTransform, -70.0f, glm::vec3(1.0, 1.0, 1.0));
    transform = projection * view * modelTransform;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", modelTransform);
    m_model->Draw(m_program.get());
    }
}

FullScreenTexturedQuad* fullScreen;
bool Context::GlobalInit()
{

    m_globalProgram = Program::Create("./shader/lightingGlobal.vs", "./shader/lightingGlobal.fs");
    if (!m_globalProgram)
            return false;

    glClearColor(0.0f, 0.1f, 0.2f, 0.0f); // 컬러 프레임버퍼 화면을 클리어할 색상 지정

    scene = new Scene;
    fullScreen = new FullScreenTexturedQuad;
    fullScreen->Settings(m_width, m_height);
    scene->build();
    m_globalProgram->Set(m_width, m_height);

    return true;

}

void Context::GlobalRender()
{
    m_globalProgram->Use();
    std::vector<Vec4f> image(m_width * m_height);



    scene->render(image);
    fullScreen->LoadTexture(image);
    fullScreen->Draw();


}
void Context::ProcessInput(GLFWwindow* window) {
    if (!m_cameraControl)
        return;
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
    if (!m_cameraControl)
        return;
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
}   

