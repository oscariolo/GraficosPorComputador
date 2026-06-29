#pragma once
#ifndef POLYGONS_HPP
#define POLYGONS_HPP
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cstddef>
#include <functional>

class Polyhedron {
private:
    static const size_t vertexSize = sizeof(float) * 4;
    static const size_t colorSize = sizeof(float) * 3;
protected:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<unsigned int> indices;
    bool staticDraw, transformed = false;

    
public:
    Polyhedron(std::vector<float> vertices, std::vector<float> colors, std::vector<unsigned int> indices, bool staticDraw = true);
    ~Polyhedron() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void transform(glm::mat4 transform);
    void clearTransform();

    void draw(bool filled);
};

class Vertex {
private:
    float x, y, z, w;
    int color;
public:
    Vertex(float x, float y, float z, float w, int color) : x(x), y(y), z(z), w(w), color(color) {}

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    float getW() const { return w; }
    int getColor() const { return color; }

    bool operator==(const Vertex& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return hash<float>{}(vertex.getX()) ^ 
                    (hash<float>{}(vertex.getY()) + 0x9e3779b9 + (hash<float>{}(vertex.getX()) << 6) + (hash<float>{}(vertex.getX()) >> 2)) ^
                    (hash<float>{}(vertex.getZ()) + 0x9e3779b9 + (hash<float>{}(vertex.getY()) << 6) + (hash<float>{}(vertex.getY()) >> 2));
        }
    };
}

class Side {
private:
    Vertex start, end;
public:
    Side(Vertex start, Vertex end) : start(start), end(end) {}

    Vertex getStart() const { return start; }
    Vertex getEnd() const { return end; }
};

class Face {
private:
    std::vector<Side> sides;
public:
    Face(std::vector<Side> sides) : sides(sides) {}

    std::vector<Side> getSides() const { return sides; }
    std::vector<Vertex> getTriangleVertices() const;
};

class Cube {
private:
    std::vector<Face> faces;
public:
    Cube(float x, float y, float z, float size);

    Polyhedron toGeometry(bool staticDraw = true);

    Polyhedron cubeMatrix(bool staticDraw = true);
};
#endif // POLYGONS_HPP