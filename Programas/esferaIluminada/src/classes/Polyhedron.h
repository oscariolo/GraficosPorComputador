#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

class Polyhedron {
    // Class definition
    

    public:
        unsigned int VBO, VAO, EBO, shaderID;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indexData;
        glm::mat4 model = glm::mat4(1.0f);

        void instantiate();
        void draw();
        void setShaders(unsigned int shaderID);

        void setUniformColor(glm::vec3 color);
        void calculateNormals(); //calcula las normales por caras



};

#endif // POLYHEDRON_H