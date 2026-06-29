#include "Cube.h"


void Cube :: generateCube() {
    // Define the vertices of the cube
    vertices = {
        Vertex(-0.5f, -0.5f, -0.5f), // 0
            Vertex( 0.5f, -0.5f, -0.5f), // 1
            Vertex( 0.5f,  0.5f, -0.5f), // 2
            Vertex(-0.5f,  0.5f, -0.5f), // 3
            Vertex(-0.5f, -0.5f,  0.5f), // 4
            Vertex( 0.5f, -0.5f,  0.5f), // 5
            Vertex( 0.5f,  0.5f,  0.5f), // 6
            Vertex(-0.5f,  0.5f,  0.5f)  // 7
        };

        // Define the indices for the cube's faces (two triangles per face)
        indexData = {
            // Back face
            0, 1, 2,
            2, 3, 0,
            // Front face
            4, 5, 6,
            6, 7, 4,
            // Left face
            4, 7, 3,
            3, 0, 4,
            // Right face
            1, 5, 6,
            6, 2, 1,
            // Bottom face
            4, 1, 0,
            1, 4, 5,
            // Top face
            3, 2, 6,
            6, 7, 3
        };

        //normals son fijas para cada cara, se asignan a los vertices de cada cara
        // Back face normals
        vertices[0].setNormal(0.0f, 0.0f, -1.0f);
        vertices[1].setNormal(0.0f, 0.0f, -1.0f);
        vertices[2].setNormal(0.0f, 0.0f, -1.0f);
        vertices[3].setNormal(0.0f, 0.0f, -1.0f);
        // Left face normals
        vertices[4].setNormal(-1.0f, 0.0f, 0.0f);
        vertices[7].setNormal(-1.0f, 0.0f, 0.0f);
        vertices[3].setNormal(-1.0f, 0.0f, 0.0f);
        vertices[0].setNormal(-1.0f, 0.0f, 0.0f);
        // Right face normals
        vertices[1].setNormal(1.0f, 0.0f, 0.0f);
        vertices[5].setNormal(1.0f, 0.0f, 0.0f);
        vertices[6].setNormal(1.0f, 0.0f, 0.0f);
        vertices[2].setNormal(1.0f, 0.0f, 0.0f);
        // Bottom face normals
        vertices[4].setNormal(0.0f, -1.0f, 0.0f);
        vertices[1].setNormal(0.0f, -1.0f, 0.0f);
        vertices[0].setNormal(0.0f, -1.0f, 0.0f);
        vertices[5].setNormal(0.0f, -1.0f, 0.0f);
        // Top face normals
        vertices[3].setNormal(0.0f, 1.0f, 0.0f);
        vertices[2].setNormal(0.0f, 1.0f, 0.0f);
        vertices[6].setNormal(0.0f, 1.0f, 0.0f);
        vertices[7].setNormal(0.0f, 1.0f, 0.0f);


    }
