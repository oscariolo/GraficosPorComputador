#include "Vertex.h"


Vertex::Vertex() 
    : position(0.0f), color(1.0f, 0.0f, 1.0f), normal(0.0f) {}


Vertex::Vertex(float px, float py, float pz) 
    : position(px, py, pz), 
      color(1.0f, 0.0f, 1.0f), 
      normal(0.0f, 0.0f, 0.0f) 
{
    
}

void Vertex::setNormal(float x, float y, float z){
   
    this->normal.x = x;
    this->normal.y = y;
    this->normal.z = z;
}