#include "GridSphere.h"
#include <glad/glad.h>
#include <math.h>
#include <vector>

float x_coordinate(float center_x, float radius, float theta,float alpha){
    return center_x + radius*sin(theta)*cos(alpha);
};

float y_coordinate(float center_y, float radius, float theta,float alpha){
    return center_y + radius*cos(theta);
};

float z_coordinate(float center_z, float radius, float theta,float alpha){
    return center_z + radius*sin(theta)*sin(alpha);
};



void GridSphere::setShape(){
    //Meridianos es rotacion en theta
    //paralelos es rotacion en alpha
    //los polos se generan a partir del ultimo vertice que encaja con los meridianos? o de los paralelos?
    //vertice polo es cuando el theta es 180 o 0

    vertices.clear();

    const int N = NUM_LINES; // same count for meridians and parallels

    const float meridianStep = 2.0f * PI / N;      // alpha
    const float parallelStep = PI / (N + 1.0f);    // theta (skip poles)

    //insert the poles
    vertices.push_back(x_coordinate(center[0], radius, 0.0f, 0.0f)); // North pole
    vertices.push_back(y_coordinate(center[1], radius, 0.0f, 0.0f));
    vertices.push_back(z_coordinate(center[2], radius, 0.0f, 0.0f));

    vertices.push_back(x_coordinate(center[0], radius, PI, 0.0f)); // South pole
    vertices.push_back(y_coordinate(center[1], radius, PI, 0.0f));
    vertices.push_back(z_coordinate(center[2], radius, PI, 0.0f));

    for (int i = 1; i <= N; ++i) {                 // parallels
        float theta = i * parallelStep;

        for (int j = 0; j < N; ++j) {              // meridians
            float alpha = j * meridianStep;

            vertices.push_back(x_coordinate(center[0], radius, theta, alpha));
            vertices.push_back(y_coordinate(center[1], radius, theta, alpha));
            vertices.push_back(z_coordinate(center[2], radius, theta, alpha));
        }
    }



};

void GridSphere::bindSphere(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
};

