#include "GridSphere.h"
#include <glad/glad.h>
#include <math.h>
#include <vector>
#include <iostream>

float x_coordinate(float center_x, float radius, float theta, float alpha){
    return center_x + radius*sin(alpha)*cos(theta); //0
};

float y_coordinate(float center_y, float radius, float theta, float alpha){
    return center_y + radius*cos(alpha);//1
};

float z_coordinate(float center_z, float radius, float theta, float alpha){
    return center_z + radius*sin(alpha)*sin(theta); //0
};

void GridSphere::setShape(){

    vertices.clear();
    indices.clear();
    parallelIndices.clear();
    meridianIndices.clear();
    polarIndices.clear();

    const int N = NUM_LINES; // same count for meridians and parallels (10)

    const float thetaStep = 2.0f * PI / N;      // around the sphere
    const float alphaStep = PI / (N);      // pole to pole

    for (int i = 0; i <= N+1; ++i) {     // move from pole to pole (incluye los polos)
        
        float alpha = i * alphaStep;

        for (int j = 0; j < N; ++j) {   // move around the sphere

            if(i==0 && !vertices.empty()){
                break; //evita tomar mas vertice punto en el polo
            }

            float theta = j * thetaStep;

            vertices.push_back(x_coordinate(center[0], radius, theta, alpha));
            vertices.push_back(y_coordinate(center[1], radius, theta, alpha));
            vertices.push_back(z_coordinate(center[2], radius, theta, alpha));

            if(i==N+1){
                break; //Si fue el ultimo punto rompe el bucle
            }
            
            
        }
        
    }

    std::cout << vertices.size()/3;

    setUpIndices();


};

void GridSphere::setUpIndices(){
    const int N = NUM_LINES;


    


}


void GridSphere::setUpBuffers(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),indices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
};


