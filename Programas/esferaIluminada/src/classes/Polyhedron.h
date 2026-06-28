#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include <vector>
#include "Vertex.h"

class Polyhedron {
    // Class definition
    

    public:
        unsigned int VBO, VAO, EBO, shaderID;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indexData;


        void instantiate();
        void draw();
        void setShaders(unsigned int shaderID);



};

#endif // POLYHEDRON_H