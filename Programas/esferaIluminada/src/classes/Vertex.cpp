#include "Vertex.h"

Vertex::Vertex(float position[3]){
    this->position[0] = position[0];
    this->position[1] = position[1];
    this->position[2] = position[2];
    this->color[0] = 1.0f;
    this->color[1] = 1.0f;
    this->color[2] = 1.0f;
    this->normal[0] = 0.0f;
    this->normal[1] = 0.0f;
    this->normal[2] = 0.0f;
}

Vertex::Vertex(float px, float py, float pz){
    this->position[0] = px;
    this->position[1] = py;
    this->position[2] = pz;
    this->color[0] = 1.0f;
    this->color[1] = 1.0f;
    this->color[2] = 1.0f;
    this->normal[0] = 0.0f;
    this->normal[1] = 0.0f;
    this->normal[2] = 0.0f;
}



