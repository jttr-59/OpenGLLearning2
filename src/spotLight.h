#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

#include "shader.h"

class SpotLight
{
public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Specular = glm::vec3(0.0f, 0.0f, 0.0f);

    float Constant = 0.0f;
    float Linear = 0.0f;
    float Quadratic = 0.0f;

    glm::vec2 CutOff =glm::vec2(45.0f, 55.0f);

    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float c, float l, float q, glm::vec2 cutOff)
    {
        Position = position;
        Direction = direction;
        Ambient = ambient;
        Diffuse = diffuse;
        Specular = specular;

        Constant = c;
        Linear = l;
        Quadratic = q;

        CutOff = cutOff;

    }

    void addLight(Shader litShader, int lightNum)
    {

        litShader.setVec3("spotLights[" + std::to_string(lightNum) + "].position", Position);
        litShader.setVec3("spotLights[" + std::to_string(lightNum) + "].direction", Direction);

        litShader.setVec3("spotLights[" + std::to_string(lightNum) + "].ambient", Ambient);
        litShader.setVec3("spotLights[" + std::to_string(lightNum) + "].diffuse", Diffuse);
        litShader.setVec3("spotLights[" + std::to_string(lightNum) + "].specular", Specular);

        litShader.setFloat("spotLights[" + std::to_string(lightNum) + "].constant", Constant);
        litShader.setFloat("spotLights[" + std::to_string(lightNum) + "].linear", Linear);
        litShader.setFloat("spotLights[" + std::to_string(lightNum) + "].quadratic", Quadratic);

        litShader.setFloat("spotLights[" + std::to_string(lightNum) + "].cutOff", cos(glm::radians(CutOff.x)));
        litShader.setFloat("spotLights[" + std::to_string(lightNum) + "].outerCutOff", cos(glm::radians(CutOff.y)));
    }

    void drawLightCube(Shader lightCubeShader, glm::mat4 projection, glm::mat4 view, unsigned int lightVAO)
    {

        lightCubeShader.use();

        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Position);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
};

#endif