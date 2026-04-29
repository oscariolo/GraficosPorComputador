#ifndef TRIANGULO_SERPINSKI_H
#define TRIANGULO_SERPINSKI_H


class TrianguloSerpinski {
public:
    TrianguloSerpinski();

    unsigned int triangleVBO, triangleVAO, pointsVBO, pointsVAO;

    void bindTriangle();
    void bindPoints();
    void bindNextPoint();

private:
    float triangleVertices[9];
    float lastPoint[3];

    void generateNextPoint();
};

#endif