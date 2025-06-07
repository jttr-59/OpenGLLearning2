#ifndef USEFULMATH_H
#define USEFULMATH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float fLerp(float start, float end, float alpha) {

    return start + (end - start) * alpha;
}

glm::vec3 VecLerp(glm::vec3 start, glm::vec3 end, float alpha) {

    return start + (end - start) * alpha;
}



#endif