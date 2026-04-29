#include <cstdlib>
#include <glad/glad.h>

#include "trianguloSerpinski.h"

TrianguloSerpinski::TrianguloSerpinski()
        : triangleVBO(0), triangleVAO(0), pointsVBO(0), pointsVAO(0),numVertex(0),

            triangleVertices{
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
            },

            lastPoint{0.0f, 0.0f, 0.0f}
{
    
}

void TrianguloSerpinski::generateNextPoint()
{
    int randomVertexIndex = (rand() % 3) * 3;
    float vertexPosition[2] = {triangleVertices[randomVertexIndex], triangleVertices[randomVertexIndex + 1]};

    float nextPoint[3] = {
        (lastPoint[0] + vertexPosition[0]) / 2,
        (lastPoint[1] + vertexPosition[1]) / 2,
        0
    };

    lastPoint[0] = nextPoint[0];
    lastPoint[1] = nextPoint[1];
    lastPoint[2] = nextPoint[2];

}

void TrianguloSerpinski::bindTriangle()
{
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void TrianguloSerpinski::bindPoints()
{
    glGenVertexArrays(1, &pointsVAO);
    glGenBuffers(1, &pointsVBO);

    glBindVertexArray(pointsVAO);

    glBindBuffer(GL_ARRAY_BUFFER,pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*MAX_VERTICES, NULL, GL_STREAM_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

}

void TrianguloSerpinski::bindNextPoint()
{
    generateNextPoint();
    glBindBuffer(GL_ARRAY_BUFFER,pointsVBO);
    glBufferSubData(GL_ARRAY_BUFFER, numVertex*sizeof(float)*3, sizeof(lastPoint),lastPoint);
    numVertex = numVertex + 1;

}