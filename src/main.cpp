#define STB_IMAGE_IMPLEMENTATION

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


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
#include "skybox.h"
#include "model.h"

#include <irrKlang/irrKlang.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

// mouse
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

    //tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_PROGRAM_POINT_SIZE);
    Shader particleShader("particle_test_vs.glsl", "particle_test_fs.glsl");
    Shader skyShader("skybox_test_vs.glsl", "skybox_text_fs.glsl");
<<<<<<< HEAD
=======

>>>>>>> 9a62f39ad558c26963c51de33a9e7ad3d10adf8b
    // Blinn_Phong Shader
    Shader lightingShader("Blinn_Phong_vs.glsl", "Blinn_Phong_fs.glsl");

    // ≤‚ ‘¡£◊”œµÕ≥
    /*ParticleSystem ps(10000);*/
    //ParticleSystem* ps = new ParticleSystem(10);
    //Particle p;
    //p.position = glm::fvec3(0.0f, 0.0f, 0.0f);
    //p.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
    //p.velocity = glm::fvec3(0.0f, 0.0100f, 0.0f);
    //p.life = 200.0f;
    //p.size = 5.0f;
    ////ps->initExplode(p, 0.1);
    //ps->initTrail(p);

    // ≤‚ ‘—Ãª®
    float explode_time = 4.0f;
    int new_fire = 0;
    int current_fire = 0;
    vector<Firework> fw;
    int trails_num = 60, explode_num = 0, max_trail = 60, min_trail = 40;
    //Firework fw(4.0f);
    //fireworkParam fp;
    //fp.trails_num = 60;
    //fp.explode_num = 0;
    //fp.tp.max_trail = 60;
    //fp.tp.min_trail = 40;
    //fw.init(fp);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // “Ù∆µ
    SoundEngine->play2D("./rise.wav", GL_FALSE);
    SoundEngine->play2D("./explosion.wav", GL_FALSE);
    SoundEngine->stopAllSounds();

    // ƒ£–Õ
    // Model Manor("./Castle/Castle OBJ.obj");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (open_gui) {
            ImGui::Begin("Fire Work GUI!",&open_gui);               // Create a window called "Hello, world!" and append into it
            ImGui::Text("Parameters of fireworks");               // Display some text (you can use a format strings too)
            if (ImGui::Button("Generate"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                new_fire++;

            ImGui::SliderFloat("explode_time", &explode_time, 2.0f, 6.0f);
            ImGui::SliderInt("trails_num", &trails_num, 30,150);
            ImGui::SliderInt("explode_num", &explode_num, 0,3);
            ImGui::SliderInt("max_trail", &max_trail, 30,trails_num);
            ImGui::SliderInt("min_trails", &min_trail, 30,trails_num);
            
            //ImGui::ColorEdit3("clear_color", (float*)&clear_color);
            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        } 

        // tell GLFW to capture our mouse
        if(open_gui) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// be sure to activate shader when setting uniforms/drawing objects
        //lightingShader.use();
        //lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //// view/projection transformations
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view = camera.GetViewMatrix();
        //lightingShader.setMat4("projection", projection);
        //lightingShader.setMat4("view", view);

        //// world transformation
        //glm::mat4 model = glm::mat4(1.0f);
        //lightingShader.setMat4("model", model);

        //// render the cube
        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);


        //// also draw the lamp object
        //lightCubeShader.use();
        //lightCubeShader.setMat4("projection", projection);
        //lightCubeShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //lightCubeShader.setMat4("model", model);

        //glBindVertexArray(lightCubeVAO);
        //glDrawArrays(GL_POINTS, 0, 36);


        //lightCubeShader.use();
        //lightCubeShader.setMat4("projection", projection);
        //lightCubeShader.setMat4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //lightCubeShader.setMat4("model", model);

        //glBindVertexArray(lightCubeVAO);
        //glDrawArrays(GL_POINTS, 0, 36); 
        
        if (new_fire == current_fire) {
            current_fire++;
            // ≤‚ ‘—Ãª®
            Firework new_fw(explode_time);
            fireworkParam fp;
            fp.trails_num = trails_num;
            fp.explode_num = explode_num;
            fp.tp.max_trail = max_trail;
            fp.tp.min_trail = min_trail;
            new_fw.init(fp);
            fw.push_back(new_fw);
        }
        float delta_time = timer();
        particleShader.use();
        ////ps->explode(delta_time);
        //ps->trail(delta_time);
        //ps->draw(particleShader);

        fw[new_fire].light(particleShader, delta_time);

        skyShader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        skyShader.setMat4("view", view);
        skyShader.setMat4("projection", projection);
        sb.draw(skyShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //delete ps;

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);*/

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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