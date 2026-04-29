#ifndef TRIANGULO_SERPINSKI_H
#define TRIANGULO_SERPINSKI_H


class TrianguloSerpinski {
public:
    TrianguloSerpinski();

    unsigned int triangleVBO, triangleVAO, pointsVBO, pointsVAO;
    int numVertex;

    void bindTriangle();
    void bindPoints();
    void bindNextPoint();

private:
    float triangleVertices[9];
    float lastPoint[3];
    float pointsVertex[30000];

    int nextVertexIndex;

    void generateNextPoint();
};

#endif