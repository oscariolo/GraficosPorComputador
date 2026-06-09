#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

class Cube{

    public:

        Cube(float baseSize = 1.0f, unsigned int shaderID = 0);

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
            Vertex(float x, float y, float z, std::array<float,3> rgb):Vertex(x,y,z){
                color[0] = rgb[0];
                color[1] = rgb[1];
                color[2] = rgb[2];
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

        //apply transform
        //0 para CPU, 1 para GPU
        void applyTransform(bool useGPU = true);

        //transform translation
        void transform(glm::vec3 translation);

        //scale transform
        void transform(float scaleX, float scaleY, float scaleZ);
        
        //rotation transform
        void transform(glm::vec3 axis, float angle);



        unsigned int VBO;

        unsigned int VAO;

        unsigned int EBO;

        unsigned int shaderID;

        std::array<Face,6> faces;

        glm::mat4 model = glm::mat4(1.0f);
    
    private:
        
        std::vector<Vertex> vertexData;
        std::vector<Vertex> originalVertexData;
        
        std::vector<float> getVertexBufferData() const;
        std::vector<float> getOriginalVertexBufferData() const;
        

};
#endif