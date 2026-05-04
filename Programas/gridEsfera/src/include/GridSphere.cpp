#include "GridSphere.h"
#include <glad/glad.h>
#include <math.h>
#include <vector>

float x_coordinate(float center_x, float radius, float theta,float alpha){
    return center_x + radius*sin(theta)*cos(alpha);
};

float y_coordinate(float center_y, float radius, float theta,float alpha){
    return center_y + radius*sin(theta)*sin(alpha);
};

float z_coordinate(float center_z, float radius, float theta){
    return center_z + radius*cos(theta);
};

GridSphere::GridSphere():meridianVertices(VERTEX_SIZE*NUM_LINES*3),paralelVertices(VERTEX_SIZE*NUM_LINES*3)
{

}


void GridSphere::setShape(){
    //Meridianos es rotacion en theta
    //paralelos es rotacion en alpha
    //los polos se generan a partir del ultimo vertice que encaja con los meridianos? o de los paralelos?
    //vertice polo es cuando el theta es 180 o 0



    const int step = 360/VERTEX_SIZE; //La separación segun cantidad de vertices es divido de una rotación completa a la esfera (esto sirve solo para paralelo)

    int theta = 0;
    int alpha = 0;
    for(size_t i = 0; i + 2 < paralelVertices.size(); i += 3){
        paralelVertices[i] = x_coordinate(center[0],radius,theta,alpha);
        paralelVertices[i+1] = y_coordinate(center[1],radius,theta,alpha);
        paralelVertices[i+2] = z_coordinate(center[2],radius,theta);
        alpha += step;
        if(alpha >= 360){
            alpha = 0;
            theta+=step;
        };
    }

};

void GridSphere::bindSphere(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*paralelVertices.size(), paralelVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
};

