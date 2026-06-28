#include "Sphere.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

void Sphere::generateSphere() {

    //desde poligono base subdivision y normalizacion de vertices para generar esfera

    unsigned int numSubdivisions = 3; // Número de subdivisiones para aproximar la esfera

    //asumiendo radio 1, centrada en el origen

    std::vector<Vertex> vertices = { 
    Vertex(0.5f, 0.5f, 0.5f),
    Vertex(-0.5f, -0.5f, 0.5f),
    Vertex(-0.5f, 0.5f, -0.5f),
    Vertex(0.5f, -0.5f, -0.5f)
    };

    std::vector<unsigned int> indices = {
        0,1,2,
        1,2,3,
        0,2,3,
        0,1,3
    };

    this->indexData = indices;
    this->vertices = vertices;
    //dividir cada cara del tetrahedro en subtriangulos y normalizar los vertices para que esten en la superficie de la esfera

    for(unsigned int i = 0; i< numSubdivisions ; ++i){

        

    }


}