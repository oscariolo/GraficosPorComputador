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
    const float alphaStep = PI / (N+1);      // skip the poles

    vertices.push_back(x_coordinate(center[0], radius, 0, 0));
    vertices.push_back(y_coordinate(center[1], radius, 0, 0));
    vertices.push_back(z_coordinate(center[2], radius, 0, 0));

    unsigned int idx=1;

    for (int i = 1; i <= N; ++i) {     // move from pole to polee
        
        float alpha = i * alphaStep;

        for (int j = 0; j < N; ++j) {   // move around the sphere
            
            float theta = j * thetaStep;

            vertices.push_back(x_coordinate(center[0], radius, theta, alpha));
            vertices.push_back(y_coordinate(center[1], radius, theta, alpha));
            vertices.push_back(z_coordinate(center[2], radius, theta, alpha));

            parallelIndices.push_back(idx);
            ++idx;
        }
        
    }

    vertices.push_back(x_coordinate(center[0], radius, 0, PI));
    vertices.push_back(y_coordinate(center[1], radius, 0, PI));
    vertices.push_back(z_coordinate(center[2], radius, 0, PI));

    setUpIndices();


};

void GridSphere::setUpIndices(){
    
    const int N = NUM_LINES;

    polarIndices.clear();

    for(int i = 0; i<N; ++i){
        for(int j = 0; j<N; ++j){
            meridianIndices.push_back((j*N+i) + 1); //Las indices del meridianos son la "transpuesta" del de paralelos (saltando uno por los vertices)
        }
    }

    // north fan: empieza en polo y toma indices consecuentes
    const unsigned int northPoleIndex = 0;
    const unsigned int firstRingStart = 1;
    polarIndices.push_back(northPoleIndex);
    for (int i = 0; i <= N; ++i) {
        if (i == N) {
            polarIndices.push_back(firstRingStart);
        } else {
            polarIndices.push_back(firstRingStart + i);
        }
    }

    // el ultimo indice es el vertice que empieza el fan
    const unsigned int southPoleIndex = N * N + 1;
    const unsigned int lastRingStart = (N - 1) * N + 1;
    polarIndices.push_back(southPoleIndex);
    for (int i = 0; i <= N; ++i) {
        if (i == N) {
            polarIndices.push_back(lastRingStart);
        } else {
            polarIndices.push_back(lastRingStart + i);
        }
    }

    indices.insert(indices.end(),polarIndices.begin(),polarIndices.end());
    indices.insert(indices.end(),meridianIndices.begin(),meridianIndices.end());
    indices.insert(indices.end(),parallelIndices.begin(),parallelIndices.end());

    indexPolarStartOffset = 0;
    indexMeridianStartOffset = polarIndices.size() * sizeof(unsigned int);
    indexParalelStartOffset = (polarIndices.size() + meridianIndices.size()) * sizeof(unsigned int);

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


