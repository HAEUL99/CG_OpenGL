#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "Scene.h"
#include "FullScreenTexutredQuad.h"


CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create(bool isLocal);
    void Render();  
    void GlobalRender();
    void ProcessInput(GLFWwindow* window);  
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
    bool IsLocal {true};
    void Destroy();

private:
    Context() {}
    bool Init();
    bool GlobalInit();
    ProgramUPtr m_program;
    ProgramUPtr m_program1;
    ProgramUPtr m_simpleProgram;
    ProgramUPtr m_globalProgram;
    Scene* scene;

	MeshUPtr m_box;
    ModelUPtr m_model;
    TextureUPtr m_texture;
    TextureUPtr m_texture2;

    // animation
    bool m_animation { true };
    bool IsLight {true};
    bool IsLight1 {true};
    bool IsLight2 {true};

    

    // clear color
    glm::vec4 m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    //obj parameter
    struct Obj
    {
        glm::vec3 position { glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 direction { glm::vec3(0.0f, 0.0f, 0.0f) };
    };
    Obj m_obj;

    // light parameter
    struct Light {
        glm::vec3 position { glm::vec3(2.0f, 2.0f, 2.0f) };
        glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
        glm::vec2 cutoff {glm::vec2(20.0f, 5.0f)};
        float distance { 150.0f };
        glm::vec3 ambient { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
        glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;
    Light m_light1;    

    MaterialPtr m_planeMaterial;

    // camera parameter
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };

    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};
};
 
#endif // __CONTEXT_H__