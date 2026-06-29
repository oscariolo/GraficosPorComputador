#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>
class Vertex{

    public: 
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;

        Vertex();
        Vertex(float position[3]);
        Vertex(float px, float py, float pz);
        void setNormal(float x, float y, float z);

};

#endif