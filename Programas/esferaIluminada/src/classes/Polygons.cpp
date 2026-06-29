#include "Polygons.hpp"

inline std::vector<float> toRGB(int hex) {
    std::vector<float> rgb;
    rgb.reserve(3);
    rgb.push_back((float) ((hex >> 16) & 0xFF) / 255.0f);
    rgb.push_back((float) ((hex >> 8) & 0xFF) / 255.0f);
    rgb.push_back((float) (hex & 0xFF) / 255.0f);
    return rgb;
}
Polyhedron::Polyhedron(std::vector<float> vertices, std::vector<float> colors, std::vector<unsigned int> indices, bool staticDraw) {
    this->vertices = vertices;
    this->colors = colors;
    this->indices = indices;
    this->staticDraw = staticDraw;

    size_t vertexByteSize = vertices.size() * sizeof(float);
    size_t colorByteSize = colors.size() * sizeof(float);
    size_t bufferSize = vertexByteSize + colorByteSize;
    size_t indexByteSize = indices.size() * sizeof(unsigned int);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexByteSize, vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertexByteSize, colorByteSize, colors.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize, indices.data(), staticDraw ? GL_STATIC_DRAW: GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, colorSize, (void*)(vertexByteSize));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Polyhedron::transform(glm::mat4 transform) {
    if (staticDraw) {
        return;
    }

    std::vector<float> transformedVerts;
    transformedVerts.reserve(vertices.size());

    for (int i = 0; i < vertices.size(); i += 4) {
        glm::vec4 vertex = glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i + 3]);
        glm::vec4 transformedVertex = transform * vertex;
        transformedVerts.push_back(transformedVertex.x);
        transformedVerts.push_back(transformedVertex.y);
        transformedVerts.push_back(transformedVertex.z);
        transformedVerts.push_back(transformedVertex.w);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), transformedVerts.data());
    transformed = true;
}

void Polyhedron::clearTransform() {
    if (staticDraw || !transformed) {
        return;
    }

    transformed = false;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
}

void Polyhedron::draw(bool filled) {
    glBindVertexArray(VAO);

    if (!filled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

std::vector<Vertex> Face::getTriangleVertices() const {
    std::vector<Vertex> vertices;

    vertices.reserve((sides.size() - 2) * 3);

    auto origin = sides[0].getStart();
    
    for (int i = 1; i < sides.size() - 1; i ++) {
        vertices.push_back(origin);
        vertices.push_back(sides[i].getStart());
        vertices.push_back(sides[i].getEnd());
    }

    return vertices;
}

Cube::Cube(float x, float y, float z, float size) {
    faces.reserve(6);

    Vertex origin = Vertex(x, y, z, 1.0f, 0x000000);

    // Cardinal points as seen from a top-down view (xz plane)
    Vertex bottomNW = origin;
    Vertex bottomSW = Vertex(origin.getX(), origin.getY(), origin.getZ() + size, 1.0f, 0x0000ff);
    Vertex bottomNE = Vertex(origin.getX() + size, origin.getY(), origin.getZ(), 1.0f, 0xff0000);
    Vertex bottomSE = Vertex(origin.getX() + size, origin.getY(), origin.getZ() + size, 1.0f, 0xff00ff);
    Vertex topNW = Vertex(origin.getX(), origin.getY() + size, origin.getZ(), 1.0f, 0x00ff00);
    Vertex topNE = Vertex(origin.getX() + size, origin.getY() + size, origin.getZ(), 1.0f, 0xffff00);
    Vertex topSW = Vertex(origin.getX(), origin.getY() + size, origin.getZ() + size, 1.0f, 0x00ffff);
    Vertex topSE = Vertex(origin.getX() + size, origin.getY() + size, origin.getZ() + size, 1.0f, 0xffffff);

    Vertex bottom[] = {bottomNW, bottomSW, bottomSE, bottomNE};
    Vertex top[] = {topNW, topSW, topSE, topNE};

    for (int i = 0; i < 6; i++) {
        std::vector<Side> sides;
        sides.reserve(4);

        switch (i) {
            default:
                sides.push_back(Side(bottom[i], bottom[(i + 1) % 4]));
                sides.push_back(Side(bottom[(i + 1) % 4], top[(i + 1) % 4]));
                sides.push_back(Side(top[(i + 1) % 4], top[i]));
                sides.push_back(Side(top[i], bottom[i]));
            break;

            case 4:
                for (int j = 0; j < 4; j++) {
                    sides.push_back(Side(bottom[j], bottom[(j + 1) % 4]));
                }
            break;

            case 5:
                for (int j = 0; j < 4; j++) {
                    sides.push_back(Side(top[j], top[(j + 1) % 4]));
                }
            break;
        }

        faces.push_back(Face(sides));
    }
}

Polyhedron Cube::toGeometry(bool staticDraw) {
    std::unordered_map<Vertex, int> vertices;
    std::vector<float> positionData;
    std::vector<float> colorData;
    std::vector<unsigned int> indices;

    int index = 0;

    for (Face face : faces) {
        for (Side side : face.getSides()) {
            for (Vertex vert : {side.getStart(), side.getEnd()}) {
                if (vertices.find(vert) == vertices.end()) {
                    vertices[vert] = index;
                    positionData.push_back(vert.getX());
                    positionData.push_back(vert.getY());
                    positionData.push_back(vert.getZ());
                    positionData.push_back(vert.getW());
    
                    auto rgbColor = toRGB(vert.getColor());
                    colorData.push_back(rgbColor[0]);
                    colorData.push_back(rgbColor[1]);
                    colorData.push_back(rgbColor[2]);
    
                    index++;
                }
            }
        }
    }

    for (Face face : faces) {
        for (Vertex vert : face.getTriangleVertices()) {
            indices.push_back(vertices[vert]);
        }
    }

    return Polyhedron(positionData, colorData, indices, staticDraw);
}

Polyhedron Cube::cubeMatrix(bool staticDraw) {
    std::vector<float> positionData;
    std::vector<float> colorData;
    std::vector<unsigned int> indices;

    int gridX = 30;
    int gridY = 30;
    int gridZ = 30;
    

    float spacing = 1.0f; 

    float startX = -((gridX - 1) * spacing) / 2.0f;
    float startY = -((gridY - 1) * spacing) / 2.0f;
    float startZ = -((gridZ - 1) * spacing) / 2.0f;

    int currentCube = 0;

    for (int x = 0; x < gridX; ++x) {
        for (int y = 0; y < gridY; ++y) {
            for (int z = 0; z < gridZ; ++z) {

                std::unordered_map<Vertex, int> vertices;
                int indexOffset = currentCube * 8;
                int index = 0;

                float offsetX = startX + (x * spacing);
                float offsetY = startY + (y * spacing);
                float offsetZ = startZ + (z * spacing);

                for (const Face& face : faces) {
                    for (const Side& side : face.getSides()) {
                        for (const Vertex& vert : {side.getStart(), side.getEnd()}) {
                            if (vertices.find(vert) == vertices.end()) {
                                vertices[vert] = index + indexOffset;
                                
                                positionData.push_back(vert.getX() + offsetX);
                                positionData.push_back(vert.getY() + offsetY);
                                positionData.push_back(vert.getZ() + offsetZ);
                                positionData.push_back(vert.getW());
                
                                auto rgbColor = toRGB(vert.getColor());
                                colorData.push_back(rgbColor[0]);
                                colorData.push_back(rgbColor[1]);
                                colorData.push_back(rgbColor[2]);
                
                                index++;
                            }
                        }
                    }
                }

                for (const Face& face : faces) {
                    for (const Vertex& vert : face.getTriangleVertices()) {
                        indices.push_back(vertices[vert]);
                    }
                }

                currentCube++;
            }
        }
    }

    return Polyhedron(positionData, colorData, indices, staticDraw);
}