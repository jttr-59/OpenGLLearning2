#include <glad/glad.h>
#include <GLFW/glfw3.h>
// stuff for file and string management
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "flyCamera.h"
#include "usefulMath.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, float delta);
void drawSpaceship();
void modelDraw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

std::string vertexShaderPath;
std::string fragmentShaderPath;

std::string shipTexturePath;
std::string astroidTexturePath;

flyCamera basicFlyCamera(glm::vec3(0.0f, 0.0f, 3.0f));

Shader *basicShader = nullptr;

//
glm::vec3 shipPos(0.0f, 0.0f, 0.0f);
glm::vec3 shipRot(0.0f, 0.0f, 0.0f);
glm::vec3 smoothInput(0.0f);
float shipSpeed = 4.0f;
float shipAccelAlpha = 2.0f;

glm::mat4 ship(1.0f);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// calculate delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// capturing mouse input
bool firstMouse = true;
double lastX, lastY;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f}; // unsigned int indices[] = {
                                     // };
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
/*
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

*/
int main()
{
#ifdef _WIN32
    vertexShaderPath = "../shaders/vertexShader.glsl";
    fragmentShaderPath = "../shaders/fragmentShader.glsl";

    shipTexturePath = "../textures/ShipL.png";
    astroidTexturePath = "../textures/astroidL.png";
#elif defined(__APPLE__) || defined(__unix__)
    vertexShaderPath = "./shaders/vertexShader.glsl";
    fragmentShaderPath = "./shaders/fragmentShader.glsl";

    crateTexturePath = "./textures/ShipL.png";
    awesomeFaceTexturePath = "./textures/astroidL.png";
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

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // create shader
    basicShader = new Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    float texCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};

    glm::vec3 cubePositions[50];
    glm::vec3 cubeRotationAxis[50];
    glm::vec3 cubeScale[50];

    for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {

        cubePositions[i] = glm::vec3((float)(((float)(rand() % 1000) / 1000.0f) - 0.5f) * 30.0f, (float)(((float)(rand() % 1000) / 1000.0f) - 0.5f) * 30.0f, (float)((float)(rand() % 1000) / 1000.0f) * -50.0f);
        cubeRotationAxis[i] = glm::normalize(glm::vec3((float)(((float)(rand() % 1000) / 1000.0f) - 0.5f), (float)(((float)(rand() % 1000) / 1000.0f) - 0.5f), (float)(((float)(rand() % 1000) / 1000.0f) - 0.5f)));
        cubeScale[i] = glm::vec3((float)(((float)(rand() % 1000) / 1000.0f) / 2 + 0.5 ), (float)(((float)(rand() % 1000) / 1000.0f) / 2 + 0.5 ), (float)(((float)(rand() % 1000) / 1000.0f) / 2 + 0.5 ));
    }

    // this just choses what to do when the texture overflows when drawing on a triangle
    // S and T respond to the X and Y axis of the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // this tells whether to add filters to the texture when up or down scaling it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // image loader creating string filled with image data, aswell as integers for the height width and channels (not really sure what this is)
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(shipTexturePath.c_str(), &width, &height, &nrChannels, 0);

    stbi_set_flip_vertically_on_load(true);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed To Load Texture From Image" << std::endl;
    }

    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load(astroidTexturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed To Load Texture From Image" << std::endl;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // free up the storage used creating the image string
    stbi_image_free(data);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    //  bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    basicShader->use();
    basicShader->setInt("texture1", 0);
    basicShader->setInt("texture2", 1);

    glEnable(GL_DEPTH_TEST);
    // fps inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    shipPos = glm::vec3(0.0f, 0.0f, 0.0f);
    shipRot = glm::vec3(0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        // input
        // -----

        // matrices

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // matrix for projection to screen
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(CAM_FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::vec3 camPos = shipPos + glm::vec3(0.0f, 2.0f, 3.0f);

        basicFlyCamera.Position = VecLerp(basicFlyCamera.Position, camPos, 4 * deltaTime);
        glm::mat4 view = basicFlyCamera.GetLookAtMatrix(shipPos + glm::vec3(0.0f, 0.0f, -5.0f));

        basicShader->setMat4("view", view);
        basicShader->setMat4("projection", projection);

        glBindVertexArray(VAO);

        ship = glm::mat4(1.0f);

        basicShader->setFloat("textureSet", 1.0f);
        basicShader->setMat4("ship", ship);
        //modelDraw(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
        {
            // model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model,  (float)glfwGetTime() * 6, cubeRotationAxis[i]);
            model = glm::scale(model, cubeScale[i] * 2.0f);

            cubePositions[i].z += 18 * deltaTime;

            if(cubePositions[i].z > 4) {
                cubePositions[i].z = -60;
            }

            basicShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        shipRot.z = fLerp(shipRot.z, -smoothInput.x * 900, 8 * deltaTime);
        shipRot.x = fLerp(shipRot.x, smoothInput.y * 900, 8 * deltaTime);

        ship = glm::translate(ship, shipPos);
        ship = glm::rotate(ship, glm::radians(shipRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        ship = glm::rotate(ship, glm::radians(shipRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ship = glm::rotate(ship, glm::radians(shipRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        ship = glm::scale(ship, glm::vec3(1.0f, 1.0f, 1.0f));
        basicShader->setMat4("ship", ship);

        drawSpaceship();

        // shipRot.z = 45.0f * glm::sin(glfwGetTime() / 2);
        //  glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float delta)
{
    glm::vec3 shipInputAxis(0.0f, 0.0f, 0.0f);

    // shipInputAxis = VecLerp(shipInputAxis, glm::vec3(0.0f, 0.0f, 0.0f), shipAccelAlpha * delta);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    shipInputAxis = glm::vec3(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        shipInputAxis.y = 1 * shipSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        shipInputAxis.y = -1 * shipSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        shipInputAxis.x = -1 * shipSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        shipInputAxis.x = 1 * shipSpeed * delta;

    smoothInput = VecLerp(smoothInput, shipInputAxis, shipAccelAlpha * delta);

    shipPos += smoothInput;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    basicFlyCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void modelDraw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    basicShader->setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawSpaceship()
{
    // modelDraw(position + glm::vec3(0.0f, 0.0f, 0.0f), rotation + glm::vec3(0.0f, 0.0f, 0.0f), scale + glm::vec3(1.0f, 1.0f, 1.0f));
    basicShader->setFloat("textureSet", 0.0f);

    modelDraw(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.5f, 1.3f));
    modelDraw(glm::vec3(0.0f, 0.0f, -0.6f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(0.8f, 0.3f, 0.8f));
    // wings
    modelDraw(glm::vec3(0.3f, -0.1f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 0.15f, 1.0f));
    modelDraw(glm::vec3(-0.3f, -0.1f, 0.0f), glm::vec3(0.0f, -20.0f, 0.0f), glm::vec3(1.0f, 0.15f, 1.0f));

    // thrusters
    modelDraw(glm::vec3(0.2f, 0.0f, 0.7f), glm::vec3(0.0f, 0.0f, 45.0f), glm::vec3(0.3f, 0.3f, 0.3f));
    modelDraw(glm::vec3(-0.2f, 0.0f, 0.7f), glm::vec3(0.0f, 0.0f, 45.0f), glm::vec3(0.3f, 0.3f, 0.3f));
}
