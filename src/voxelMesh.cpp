#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VoxelMesh.h"
#include "Shader.h"
#include "mesh.h"

float cubeVerts[] = {
    // positions           // normals           // texture coords
    -0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.25f,  0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.25f, -0.25f, -0.25f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.25f,  0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.25f, -0.25f,  0.25f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.25f,  0.25f, -0.25f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.25f, -0.25f, -0.25f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.25f, -0.25f,  0.25f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.25f,  0.25f,  0.25f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.25f,  0.25f,  0.25f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.25f,  0.25f, -0.25f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.25f, -0.25f, -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.25f, -0.25f, -0.25f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.25f, -0.25f,  0.25f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.25f,  0.25f,  0.25f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.25f, -0.25f,  0.25f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.25f, -0.25f, -0.25f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.25f,  0.25f,  0.25f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.25f,  0.25f, -0.25f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


std::vector<unsigned int> fullIndices;

void VoxelMesh::setupRenderer() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

}

void VoxelMesh::draw(Shader shader, glm::mat4 view, glm::mat4 projection) {
    shader.use();
    for(unsigned int i = 0; i < voxelBuffer.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, voxelBuffer[i].position / 2.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        shader.setMat3("normalWorld", normalMatrix);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, cubeVerts[0], 36);
        glBindVertexArray(0);

    }
}

void VoxelMesh::addVoxel(glm::vec3 position) {
    voxel vox;

    vox.position = position;
    vox.uFace = true;
    vox.dFace = true;
    vox.nFace = true;
    vox.eFace = true;
    vox.sFace = true;
    vox.wFace = true;

    VoxelMesh::voxelBuffer.push_back(vox);
}