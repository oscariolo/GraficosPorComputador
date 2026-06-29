#include "Sphere.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "Sphere.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

void Sphere::generateSphere() {

    // 1. Usar vectores locales (Sandbox) para evitar corrupción de memoria en la clase
    std::vector<Vertex> tempVertices = { 
        Vertex(0.5f, 0.5f, 0.5f),
        Vertex(-0.5f, -0.5f, 0.5f),
        Vertex(-0.5f, 0.5f, -0.5f),
        Vertex(0.5f, -0.5f, -0.5f)
    };

    std::vector<unsigned int> tempIndices = {
        0, 1, 2,
        1, 2, 3,
        0, 2, 3,
        0, 1, 3
    };

    // 2. Normalizar la base 
    for (auto& v : tempVertices) {
        v.position = glm::normalize(v.position);
        v.setNormal(v.position.x, v.position.y, v.position.z); 
    }

    // Mantén las subdivisiones a un máximo de 5 para gráficos fluidos
    unsigned int numSubdivisions = 3; 

    // 3. Bucle de subdivisión (Totalmente aislado de 'this')
    for (unsigned int i = 0; i < numSubdivisions; ++i) {
        
        // Copiamos el estado actual para leer de él
        std::vector<Vertex> nextVertices = tempVertices; 
        std::vector<unsigned int> nextIndices;
        
        std::cout << "Step0";

        std::cout << tempVertices.size() << " " << tempIndices.size() << std::endl;
        // ¡CRÍTICO! Pre-asignar la memoria para evitar reasignaciones (que es lo que te crashea)
        nextVertices.reserve(tempVertices.size() + tempIndices.size());
        nextIndices.reserve(tempIndices.size() * 4);

         std::cout << "Step0.2";

        for (size_t j = 0; j < tempIndices.size(); j += 3) {
            
            unsigned int i0 = tempIndices[j];
            unsigned int i1 = tempIndices[j+1];
            unsigned int i2 = tempIndices[j+2];

            glm::vec3 v0 = tempVertices[i0].position;
            glm::vec3 v1 = tempVertices[i1].position;
            glm::vec3 v2 = tempVertices[i2].position;

            // Al sumar vectores en una esfera, basta con normalizar el resultado
            glm::vec3 m01 = glm::normalize(v0 + v1);
            glm::vec3 m12 = glm::normalize(v1 + v2);
            glm::vec3 m20 = glm::normalize(v2 + v0);

            std::cout << "Step1";

            // Crear y guardar los vértices de manera clásica y segura
            unsigned int idx01 = nextVertices.size(); 
            Vertex vert01(m01.x, m01.y, m01.z);
            std::cout << "Step2";
            vert01.setNormal(m01.x, m01.y, m01.z);
            std::cout << "Step3";
            nextVertices.push_back(vert01);
            std::cout << "Step4";
            
            unsigned int idx12 = nextVertices.size(); 
            Vertex vert12(m12.x, m12.y, m12.z);
            vert12.setNormal(m12.x, m12.y, m12.z);
            nextVertices.push_back(vert12);
            
            unsigned int idx20 = nextVertices.size(); 
            Vertex vert20(m20.x, m20.y, m20.z);
            vert20.setNormal(m20.x, m20.y, m20.z);
            nextVertices.push_back(vert20);

            // Generar los 4 subtriángulos
            nextIndices.push_back(i0);    nextIndices.push_back(idx01); nextIndices.push_back(idx20); // Top
            nextIndices.push_back(idx01); nextIndices.push_back(i1);    nextIndices.push_back(idx12); // Bot Left
            nextIndices.push_back(idx20); nextIndices.push_back(idx12); nextIndices.push_back(i2);    // Bot Right
            nextIndices.push_back(idx01); nextIndices.push_back(idx12); nextIndices.push_back(idx20); // Center
        }
        
        // Actualizamos los temporales para la siguiente iteración
        tempVertices = nextVertices;
        tempIndices = nextIndices;
    }

    // 4. Una vez que toda la matemática está 100% terminada y segura, asignamos a la clase
    this->vertices = tempVertices;
    this->indexData = tempIndices;
}