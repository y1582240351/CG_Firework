#define STB_IMAGE_IMPLEMENTATION

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

//fireworks
std::vector<std::pair<Firework*, bool>>fireworks;


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

    // Blinn_Phong Shader
    Shader lightingShader("Blinn_Phong_vs.glsl", "Blinn_Phong_fs.glsl");

    Shader BlurShader("Result.vs", "Blur.fs");
    Shader ResultShader("Result.vs", "Result.fs");

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

    skyShader.use();
    skyShader.setInt("skybox", 0);

    // 音频
    SoundEngine->play2D("./rise.wav", GL_FALSE);
    SoundEngine->play2D("./explosion.wav", GL_FALSE);
    SoundEngine->stopAllSounds();

    


    // 模型
    // Model Manor("./Castle/Castle OBJ.obj");


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

        blur.bindFrameBuffer();

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
                    fireworks[i].first->light(particleShader, delta_time);
                }
                else
                {
                    fireworks[i].first->destroy();
                    delete fireworks[i].first;
                    fireworks[i].second = false;
                }
            }
        }

        skyShader.use();
        
        skyShader.setMat4("view", view);
        skyShader.setMat4("projection", projection);
        sb.draw(skyShader);

        blur.blurTheFrame(BlurShader, ResultShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //delete ps;

   
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
                    newFireWork = new innerburstfirework(4.0f);
                }
                else
                {
                    newFireWork = new bigfirework(4.0f);
                }
                fireworkParam fp;
                fp.trails_num = 300;
                fp.explode_num = 0;
                fp.tp.max_trail = 60;
                fp.tp.min_trail = 40;
                newFireWork->init(fp);

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