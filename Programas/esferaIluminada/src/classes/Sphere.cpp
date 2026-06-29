#include "Sphere.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

void Sphere::generateSphere(unsigned int numSubdivisions) {

    // 1. CÁLCULO MATEMÁTICO EXACTO DE LA MEMORIA FINAL
    // El tetraedro base tiene 4 vértices y 12 índices.
    // En cada iteración, se multiplica exactamente por 4.
    unsigned int finalVertexCount = 4;
    unsigned int finalIndexCount = 12;
    for (unsigned int i = 0; i < numSubdivisions; ++i) {
        finalVertexCount *= 4; 
        finalIndexCount *= 4;
    }

    // Limpiamos los vectores principales de la clase
    this->vertices.clear();
    this->indexData.clear();
    
    this->vertices.reserve(finalVertexCount);
    this->indexData.reserve(finalIndexCount);

    // 3. Crear el Tetraedro Base directamente en el vector blindado
    this->vertices.emplace_back(0.5f, 0.5f, 0.5f);
    this->vertices.emplace_back(-0.5f, -0.5f, 0.5f);
    this->vertices.emplace_back(-0.5f, 0.5f, -0.5f);
    this->vertices.emplace_back(0.5f, -0.5f, -0.5f);

    for (auto& v : this->vertices) {
        v.position = glm::normalize(v.position);
        v.setNormal(v.position.x, v.position.y, v.position.z); 
    }


    std::vector<unsigned int> currentIndices = { 
        0, 1, 2,  
        1, 2, 3,  
        0, 2, 3,  
        0, 1, 3 
    };

    // 4. Bucle de Subdivisión (Insertando In-Place)
    for (unsigned int i = 0; i < numSubdivisions; ++i) {
        
        std::vector<unsigned int> nextIndices;
        nextIndices.reserve(currentIndices.size() * 4);

        for (size_t j = 0; j < currentIndices.size(); j += 3) {
            
            unsigned int i0 = currentIndices[j];
            unsigned int i1 = currentIndices[j+1];
            unsigned int i2 = currentIndices[j+2];

            glm::vec3 v0 = this->vertices[i0].position;
            glm::vec3 v1 = this->vertices[i1].position;
            glm::vec3 v2 = this->vertices[i2].position;

            glm::vec3 m01 = glm::normalize(v0 + v1);
            glm::vec3 m12 = glm::normalize(v1 + v2);
            glm::vec3 m20 = glm::normalize(v2 + v0);

            unsigned int idx01 = this->vertices.size(); 
            this->vertices.emplace_back(m01.x, m01.y, m01.z);
            this->vertices.back().setNormal(m01.x, m01.y, m01.z);
            
            unsigned int idx12 = this->vertices.size(); 
            this->vertices.emplace_back(m12.x, m12.y, m12.z);
            this->vertices.back().setNormal(m12.x, m12.y, m12.z);
            
            unsigned int idx20 = this->vertices.size(); 
            this->vertices.emplace_back(m20.x, m20.y, m20.z);
            this->vertices.back().setNormal(m20.x, m20.y, m20.z);

            // Guardar nuevos triángulos
            nextIndices.push_back(i0);    nextIndices.push_back(idx01); nextIndices.push_back(idx20); 
            nextIndices.push_back(idx01); nextIndices.push_back(i1);    nextIndices.push_back(idx12); 
            nextIndices.push_back(idx20); nextIndices.push_back(idx12); nextIndices.push_back(i2);    
            nextIndices.push_back(idx01); nextIndices.push_back(idx12); nextIndices.push_back(idx20); 
        }
        
        currentIndices = nextIndices;
    }

    // Transferir índices finales a la clase
    this->indexData = currentIndices;
}