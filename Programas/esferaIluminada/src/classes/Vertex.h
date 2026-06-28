#ifndef VERTEX_H
#define VERTEX_H
struct Vertex{

    float position[3];
    float color[3];
    float normal[3];

    Vertex(float position[3]);
    Vertex(float px, float py, float pz);

};

#endif