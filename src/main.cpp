#define STB_IMAGE_IMPLEMENTATION

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stdlib.h>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "stb_image.h"
#include "particle_system.h"
#include "utility_tool.h"
#include "firework.h"
#include "bigfirework.h"

#include "innerburstfirework.h"
#include "skybox.h"
#include "model.h"
#include "blur.h"

#include <irrKlang/irrKlang.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// 传递点光源给着色器
void set_point_light(Shader& blinnphongshader);

//fireworks
std::vector<std::pair<Firework*, bool>>fireworks;

// settings
//const unsigned int SCR_WIDTH = 1600;
//const unsigned int SCR_HEIGHT = 900;
const unsigned int FIREWORK_TYPES = 3;
const unsigned int FIREWORK_LIMITATIONS = 50;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// sound
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

//KEY BOARD STATUS
bool PRESS[FIREWORK_TYPES] = { 0 };

//Firework param
glm::fvec3 pos(0.0f, -1.5f, -1.0f);
float explode_time = 4.0f;
int first_trails_num = 300;
int second_trails_num = 500;
int explode_num = 0;
int max_trail = 60;
int min_trail = 40;

// gui
bool open_gui = true;

int main()
{
    cout << _pgmptr << endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    glEnable(GL_PROGRAM_POINT_SIZE);
    Shader particleShader("particle_test_vs.glsl", "particle_test_fs.glsl");
    Shader skyShader("skybox_test_vs.glsl", "skybox_text_fs.glsl");
    Shader BlurShader("Result.vs", "Blur.fs");
    Shader ResultShader("Result.vs", "Result.fs");
    Shader CastleShader("model_shader_vs.glsl", "model_shader_fs.glsl");

    SkyBox sb;

    std::vector<std::string> boxes{
        std::string("./skybox/right.jpg"),
        std::string("./skybox/left.jpg"),
        std::string("./skybox/top.jpg"),
        std::string("./skybox/bottom.jpg"),
        std::string("./skybox/front.jpg"),
        std::string("./skybox/back.jpg"),
    };
    sb.loadMap(boxes);

    /*skyShader.use();
    skyShader.setInt("skybox", 0);*/

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 音频
    SoundEngine->play2D("./rise.wav", GL_FALSE);
    SoundEngine->play2D("./explosion.wav", GL_FALSE);
    SoundEngine->stopAllSounds();

    // 模型
    Model castle("Castle/Castle OBJ2.obj");

   /* bigfirework fw(4.0f);
    fireworkParam fp;
    fp.trails_num = 300;
    fp.explode_num = 0;
    fp.tp.max_trail = 60;
    fp.tp.min_trail = 40;
    fw.init(fp);*/

    /*groundfirework fw(4.0f);
    fireworkParam fp;
    fp.trails_num = 300;
    fp.explode_num = 0;
    fp.tp.max_trail = 60;
    fp.tp.min_trail = 40;
    fw.init(fp);*/
    
    Blur blur;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        //IMGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (open_gui) {
            ImGui::Begin("Fire Work GUI!", &open_gui);               // Create a window called "Hello, world!" and append into it
            ImGui::Text("Fireworks initialization settings");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("explode_time", &explode_time, 2.0f, 6.0f);
            ImGui::SliderFloat("The x position", (float*)&pos.x, -2.0f, 0.0f);
            ImGui::SliderFloat("The y position", (float*)&pos.y, -2.0f, 0.0f);
            ImGui::SliderFloat("The z position", (float*)&pos.z, -2.0f, 0.0f);
            
            ImGui::Separator();

            ImGui::Text("Parameters of fireworks");
            ImGui::SliderInt("first_trails_num", &first_trails_num, 60, 400);
            ImGui::SliderInt("second_trails_num", &second_trails_num, 60, 700);
            ImGui::SliderInt("explode_num", &explode_num, 0, 3);
            ImGui::SliderInt("max_trail", &max_trail, 30, first_trails_num);
            ImGui::SliderInt("min_trails", &min_trail, 30, first_trails_num);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // tell GLFW to capture our mouse
        if (open_gui) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        blur.bindFrameBuffer();

        float delta_time = timer();
        deltaTime = delta_time;

        glm::mat4 view = camera.GetViewMatrix(); // remove translation from the view matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        particleShader.use();
        particleShader.setMat4("view", view);
        particleShader.setMat4("projection", projection);
        
        for (int i = 0; i < fireworks.size(); i++)
        {
            if (fireworks[i].second == true)
            {
                if (fireworks[i].first->isAlive() == true)
                {
                    fireworks[i].first->light(particleShader, delta_time,second_trails_num);
                }
                else
                {
                    fireworks[i].first->destroy();
                    delete fireworks[i].first;
                    fireworks[i].second = false;
                }
            }
        }

        // model
        CastleShader.use();
        CastleShader.setMat4("view", view);
        CastleShader.setMat4("projection", projection);
        glm::mat4 castleTransform = glm::mat4(1.0f);
        castleTransform = glm::translate(castleTransform, glm::vec3(0.0f, -1.0f, 0.0f));
        castleTransform = glm::scale(castleTransform, glm::vec3(0.01f, 0.01f, 0.01f));
        castleTransform = glm::rotate(castleTransform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        CastleShader.setMat4("model", castleTransform);
        // 传递点光源给着色器
        set_point_light(CastleShader);
        // 渲染城堡模型
        castle.Draw(CastleShader);

        // skybox
        skyShader.use();
        skyShader.setInt("skybox", 0);
        skyShader.setMat4("view", view);
        skyShader.setMat4("projection", projection);
        //skyShader.setVec3("viewPos", camera.Position);
        //skyShader.setVec3("lightPos", 0.0f, 0.0f, 0.0f);
        sb.draw(skyShader);

        blur.blurTheFrame(BlurShader, ResultShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //delete ps;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
   
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    for (int i = 0; i < FIREWORK_TYPES; i++)
    {
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS)
        {
            // 只有按键按下瞬间会发射烟花(松开->按下)
            if (!PRESS[i] && fireworks.size() < FIREWORK_LIMITATIONS)
            {
                Firework* newFireWork = nullptr;
                if (i == 0)
                {
                    newFireWork = new innerburstfirework(explode_time,pos);

                    fireworkParam fp;
                    fp.trails_num = first_trails_num;
                    fp.explode_num = explode_num;
                    fp.tp.max_trail = max_trail;
                    fp.tp.min_trail = min_trail;
                    newFireWork->init(fp);
                }
                else if (i==1)
                {
                    newFireWork = new bigfirework(explode_time,pos);

                    fireworkParam fp;
                    fp.trails_num = first_trails_num;
                    fp.explode_num = explode_num;
                    fp.tp.max_trail = max_trail;
                    fp.tp.min_trail = min_trail;
                    newFireWork->init(fp);
                }
                
                else if (i == 2)
                {
                    newFireWork = new Firework(explode_time);

                    fireworkParam fp;
                    fp.trails_num = first_trails_num/50;
                    fp.explode_num = explode_num;
                    fp.tp.max_trail = max_trail;
                    fp.tp.min_trail = min_trail;
                    newFireWork->init(fp);
                }

                

                fireworks.push_back(make_pair(newFireWork, true));
            }
            PRESS[i] = true;
        }
        if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_RELEASE)
            PRESS[i] = false;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// 传递点光源给着色器
void set_point_light(Shader& blinnphongshader)
{
    int count = 0;
    string struct_string = "light_list[";
    string color_string = "].Color";
    string pos_string = "].Position";
    string intensity_string = "].intensity";
    for (int i = 0; i < fireworks.size(); i++)
    {
        if (fireworks[i].second == true && fireworks[i].first->isExploded() == true && fireworks[i].first->isAlive() == true)
        {
            Firework* ptr = fireworks[i].first;
            blinnphongshader.setVec3(struct_string + to_string(count) + color_string, ptr->get_explode_color());
            blinnphongshader.setVec3(struct_string + to_string(count) + pos_string, ptr->get_explode_position());
            blinnphongshader.setFloat(struct_string + to_string(count) + intensity_string, 1.0f);
            count++;
        }
    }
    blinnphongshader.setInt("num_lights", count);
}