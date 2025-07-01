#ifndef VOXELMESH_H
#define VOXELMESH_H

#include <glad/glad.h>

//stuff for file and string management
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


struct voxel {
    glm::vec3 position;

    bool uFace;
    bool dFace;
    bool nFace;
    bool eFace;
    bool sFace;
    bool wFace;

};

class VoxelMesh {
    public:
        VoxelMesh() {
            setupRenderer();
        }

        void draw(Shader shader, glm::mat4 view, glm::mat4 projection);

        void addVoxel(glm::vec3 position);
    private:
        unsigned int VAO, VBO, EBO;
        std::vector<voxel> voxelBuffer;

        void setupRenderer();

};

#endif