#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

#include "shader.h"

class DirectionalLight
{
    public:

    glm::vec3 Direction = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::vec3 Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Specular = glm::vec3(0.0f, 0.0f, 0.0f);

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
        Direction = direction;
        Ambient = ambient;
        Diffuse = diffuse;
        Specular = specular;
    }

    void addLight(Shader litShader, int lightNum) {

        litShader.setVec3("dirLights[" + std::to_string(lightNum) + "].direction", Direction);

        litShader.setVec3("dirLights[" + std::to_string(lightNum) + "].ambient", Ambient);
        litShader.setVec3("dirLights[" + std::to_string(lightNum) + "].diffuse", Diffuse);
        litShader.setVec3("dirLights[" + std::to_string(lightNum) + "].Specular", Specular);
    }
};


#endif