#include <glad/glad.h>
#include <GLFW/glfw3.h>
// stuff for file and string management
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cmath>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "flyCamera.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);
GLFWmonitor* monitor;

// settings
const int WINDOWED_WIDTH = 1920/2;
const int WINDOWED_HEIGHT = 1080/2;

int currentWidth = 1920;
int currentHeight = 1080;

int monitorWidth;
int monitorHeight;
int monitorRefreshRate;

flyCamera basicFlyCamera(glm::vec3(0.0f, 0.0f, 3.0f));

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// calculate delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float lightAmbientPower = 0.2f;
float lightDiffusePower = 0.5f;

// capturing mouse input
bool firstMouse = true;
double lastX, lastY;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

// unsigned int indices[] = {
// };
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

std::string vertexShaderPath;
std::string fragmentShaderPath;

std::string lightFragmentShaderPath;
std::string lightVertexShaderPath;

std::string crateDiffuseTexturePath;
std::string createSpecularTexturePath;

std::string backpackModelPath;

int main()
{
#ifdef _WIN32
    vertexShaderPath = "../shaders/vertexShader.glsl";
    fragmentShaderPath = "../shaders/fragmentShader.glsl";

    lightFragmentShaderPath = "../shaders/lightFragment.glsl";
    lightVertexShaderPath = "../shaders/lightVertex.glsl";

    crateDiffuseTexturePath = "../textures/oak_veneer_01_diff_4k.jpg";
    createSpecularTexturePath = "../textures/container2_specular.png";

    backpackModelPath = "../models/backpack/backpack.obj";
#elif defined(__APPLE__) || defined(__unix__)
    vertexShaderPath = "./shaders/vertexShader.glsl";
    fragmentShaderPath = "./shaders/fragmentShader.glsl";

    lightFragmentShaderPath = "./shaders/lightFragment.glsl";
    lightVertexShaderPath = "./shaders/lightVertex.glsl";

    crateDiffuseTexturePath = "./textures/container2.png";
    createSpecularTexturePath = "./textures/container2_specular.png";

    backpackModelPath = "./models/backpack/backpack.obj";
#else
#error "Unsupported platform"
#endif

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#ifdef NDEBUG
    std::cout << "Running in RELEASE mode" << std::endl;
#else
    std::cout << "Running in DEBUG mode" << std::endl;
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(currentWidth, currentHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

    monitorWidth = vidMode->width;
    monitorHeight = vidMode->height;
    monitorRefreshRate = vidMode->refreshRate;

    std::cout << monitorWidth << " : " << monitorHeight << std::endl;

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // create shader
    Shader litShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    Shader lightCubeShader(lightVertexShaderPath.c_str(), lightFragmentShaderPath.c_str());

    glm::vec3 cubePositions[20];

    float texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};

    for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {

        cubePositions[i] = glm::vec3((float)(((float)(rand() % 1000) / 1000.0f) - 0.5f) * 8.0f, (float)(((float)(rand() % 1000) / 1000.0f) - 0.5f) * 8.0f, (float)(((float)(rand() % 1000) / 1000.0f) - 0.5f) * 8.0f);
    }

    // this just choses what to do when the texture overflows when drawing on a triangle
    // S and T respond to the X and Y axis of the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // this tells whether to add filters to the texture when up or down scaling it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int diffuseMap = loadTexture(crateDiffuseTexturePath.c_str());
    unsigned int specularMap = loadTexture(createSpecularTexturePath.c_str());

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    //  bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // no need to restate bindBufferData because it is already there.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    // texture coord attribute

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------

    glEnable(GL_DEPTH_TEST);
    // fps inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

        // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Init backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    DirectionalLight dirLight(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f));
    PointLight pointLight1(glm::vec3( 0.7f,  0.2f,  2.0f), glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    PointLight pointLight2(glm::vec3( 2.3f, -3.3f, -4.0f), glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    PointLight pointLight3(glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);
    PointLight pointLight4(glm::vec3( 0.0f,  0.0f, -3.0f), glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f);

    Model backpackModel(backpackModelPath);

    litShader.use();
    dirLight.addLight(litShader, 0); 

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        litShader.use();
        pointLight1.addLight(litShader, 0);
        pointLight2.addLight(litShader, 1);
        pointLight3.addLight(litShader, 2);
        pointLight4.addLight(litShader, 3);
        
        pointLight1.Position.x = (float)sin(glfwGetTime());
        pointLight2.Position.x = (float)cos(glfwGetTime());
        pointLight3.Position.x = (float)sin(glfwGetTime());
        pointLight4.Position.x = (float)cos(glfwGetTime());
        /*
        //using cosine makes calculations easier
        litShader.setFloat("light.cutOff", glm::cos(glm::radians(20.0f)));
        litShader.setFloat("light.outerCutOff", glm::cos(glm::radians(30.0f)));
        */

        litShader.setVec3("viewPos", basicFlyCamera.Position);

        litShader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        litShader.setInt("material.specular", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        litShader.setFloat("material.shininess", 32.0f);

        //litShader.setFloat("ambientStrength", ambientLight);
        //litShader.setFloat("specularStrength", specularLight);

        lightPos.z = 2.0f * (float)sin(glfwGetTime());

        // matrix for projection to screen
        float aspect = (float)currentWidth / (float)currentHeight;
        //to avoid dividing by zero if tabbed out;
        if (currentHeight == 0 || aspect < 0.001f)
            aspect = 1.0f;
    
        glm::mat4 projection = glm::perspective(glm::radians(CAM_FOV), aspect, 0.1f, 100.0f);
        glm::mat4 view = basicFlyCamera.GetViewMatrix();
        litShader.setMat4("projection", projection);
        litShader.setMat4("view", view);


        for(unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        //model = glm::scale(model, glm::vec3(30.0f, 1.0f, 1.0f));
        litShader.setMat4("model", model);
        litShader.setMat3("normalWorld", glm::mat3(glm::transpose(glm::inverse(model))));

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // adjust position
        model = glm::scale(model, glm::vec3(1.0f)); // scale up or down if needed
        litShader.setMat4("model", model);
        litShader.setMat3("normalWorld", glm::mat3(glm::transpose(glm::inverse(model))));
        backpackModel.Draw(litShader);

        pointLight1.drawLightCube(lightCubeShader, projection, view, lightVAO);
        pointLight2.drawLightCube(lightCubeShader, projection, view, lightVAO);
        pointLight3.drawLightCube(lightCubeShader, projection, view, lightVAO);
        pointLight4.drawLightCube(lightCubeShader, projection, view, lightVAO);
        // gl draw elements requires a indeci

        // LIGHT SHADER DRAWING

                        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        // Start new ImGui frame

        // ImGui widgets
        ImGui::Begin("Debug");
        ImGui::Text(std::to_string((int)(1.0f/deltaTime)).c_str());
        ImGui::SeparatorText("Light Debug");
        //ImGui::SliderFloat("Light Red Value", &lightColor.r, 0.0f, 1.0f);
        //ImGui::SliderFloat("Light green Value", &lightColor.g, 0.0f, 1.0f);
        //ImGui::SliderFloat("Light blue Value", &lightColor.b, 0.0f, 1.0f);
        ImGui::SeparatorText("Light Power");
        ImGui::SliderFloat("Light Diffuse Intensity", &lightDiffusePower, 0.0f, 5.0f);
        ImGui::SliderFloat("Light Ambient Intensity", &lightAmbientPower, 0.0f, 1.0f);
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // -------------------------------------------------------------------------------

        // glBindVertexArray(0); // no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

bool escapeLatch = false;
bool controlLatch = false;

void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        if(escapeLatch == false) {
            escapeLatch = true;
            if(glfwGetWindowMonitor(window) == NULL)
                glfwSetWindowMonitor(window, monitor, 0, 0, monitorWidth, monitorHeight, GLFW_DONT_CARE);
            else
                glfwSetWindowMonitor(window, NULL, monitorWidth/2, monitorHeight/2, WINDOWED_WIDTH, WINDOWED_HEIGHT, GLFW_DONT_CARE);
        }
    }
    else
        escapeLatch = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        if(controlLatch == false) {
        controlLatch = true;
        if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    else
        controlLatch = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        basicFlyCamera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        basicFlyCamera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        basicFlyCamera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        basicFlyCamera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    currentWidth = width;
    currentHeight = height;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = (float)(xposIn);
    float ypos = (float)(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    basicFlyCamera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    basicFlyCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}