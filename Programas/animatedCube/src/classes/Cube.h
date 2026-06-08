#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <array>

class Cube{

    public:

        Cube();

        struct Vertex{
            std::array<float,3> position;
            std::array<float,3> color = {1,1,1};

            Vertex(){
            }

            //Only position
            Vertex(float x, float y, float z){
                position[0] = x;
                position[1] = y;
                position[2] = z;
            }

            //Position and color
            Vertex(float x, float y, float z, float r, float g, float b):Vertex(x,y,z){
                color[0] = r;
                color[1] = g;
                color[2] = b;
            }

            bool operator==(const Vertex& other) const{
                return 
                    position[0] == other.position[0] &&
                    position[1] == other.position[1] &&
                    position[2] == other.position[2];
            }
            

        };

        struct Edge{
            std::array<Vertex,2> vertices;

            Edge(){

            }

            Edge(Vertex v1, Vertex v2): vertices{v1, v2} {
            }

        };
                
        struct Face{
            std::array<Edge,4> edges;

            Face(){}

            std::array<Vertex, 4> getVertices() const {
                std::array<Vertex, 4> result;
                result[0] = edges[0].vertices[0];
                result[1] = edges[0].vertices[1];
                result[2] = edges[1].vertices[0];
                result[3] = edges[1].vertices[1];
                return result;
            }


        };
    

        void instantiate();

        void rotateGPU(float angle);

        void translateGPU(const float vec[3]);

        void scaleGPU(float scale);

        void rotateCPU(float angle);

        void translateCPU(const float vec[3]);

        void scaleCPU(float scale);

        unsigned int VBO;

        unsigned int VAO;

        unsigned int EBO;

        std::array<Face,6> faces;
    
    private:
        
        std::vector<Vertex> vertexData;
        
        std::vector<float> getVertexBufferData() const {
            std::vector<float> data;
            for (const auto& vertex : vertexData) {
                data.push_back(vertex.position[0]);
                data.push_back(vertex.position[1]);
                data.push_back(vertex.position[2]);
                data.push_back(vertex.color[0]);
                data.push_back(vertex.color[1]);
                data.push_back(vertex.color[2]);
            }
            return data;
    }
        

};
#endif