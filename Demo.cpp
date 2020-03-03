#include <glm/glm.hpp>
#include <camera.h>
#include <Engine/Window.h>
#include <model.h>
#include <string>
#include <imgui/imgui.h>
#include <Engine/shader.h>
#include <Drawable/sceneElements.h>
#include <Drawable/drawableObject.h>
#include "Drawable/GUI.h"


void hello_world();

int main() {
    hello_world();
    return 0;
}

void hello_world() {

    /*创建摄像头*/
    glm::vec3 start_position(0.0f, 20.0f, 0.0f);
    Camera camera(start_position);
    /*初始参数*/
    std::string model_src = "resources/model/yorker/car.obj";
    glm::vec3 fogColor(0.5, 0.6, 0.7);//雾色（环境色）
    glm::vec3 lightColor(255, 255, 255);//光源颜色
    lightColor /= 255.0;
    glm::vec3 lightDir = glm::vec3(-0.5, 0.5, 1.0);//光源方向
    glm::vec3 lightPosition = glm::vec3(1.0, 1.0, 1.0);//光源位置
    glm::vec3 seed = glm::vec3(0);//光源位置
    bool wireframe = false;    // 渲染方式

    /*创建窗口*/
    int success;
    Window window(success, 1024, 768);
    if (!success) return;


    GUI gui(window);


    /*定义视域空间*/
    glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
    /*创建缓冲对象*/
    FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);

    window.camera = &camera;//绑定摄像头
    /*创建一个全局场景*/
    sceneElements scene;
    scene.cam = &camera;
    scene.fogColor = fogColor;
    scene.lightColor = lightColor;
    scene.lightDir = lightDir;
    scene.lightPos = lightPosition;
    scene.projMatrix = proj;
    scene.sceneFBO = &SceneFBO;
    scene.seed = seed;
    scene.wireframe = wireframe;
    /*作为静态全局成员*/
    drawableObject::scene = &scene;

    /*读取一个模型*/
    Model car(model_src.c_str());

    Shader* shader = new Shader("Default");
    shader->attachShader("shaders/model.vert")
        ->attachShader("shaders/model.frag")
        ->linkPrograms();


    while (window.continueLoop()) {
        /*参数重置*/
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///*更新场景参数*/
        //scene.lightDir = glm::normalize(camera.Front);
        //scene.lightPos = camera.Position;

        ///*更新绘制方式*/
        //if (scene.wireframe) {
        //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //} else {
        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //}

        /*处理输入*/
        float frametime = 1 / ImGui::GetIO().Framerate; // 获得刷新间隔时长
        //std::cout << frametime << std::endl;
        window.processInput(1 / 30.0f);

        //glm::mat4 model = glm::mat4(1.0);
        //glm::mat4 view = scene.cam->GetViewMatrix();
        //scene.projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
        //shader->setMat4("model", model);
        //shader->setMat4("view", view);
        //shader->setMat4("projection", scene.projMatrix);

        //car.Draw(*shader);

        window.swapBuffersAndPollEvents();
    }
    window.terminate();
}