#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <array>

class Cube{

    public:

        class Vertex{
            std::array<float,3> position;
            std::array<float,3> color;
        };

        class Edge{
            std::array<Vertex,2> vertices;
        };
                
        class Face{
            std::array<Edge,4> edges;
        };
    

        void instantiate();

        void rotateGPU(float angle);

        void translateGPU(const float vec[3]);

        void scaleGPU(float scale);

        void rotateCPU(float angle);

        void translateCPU(const float vec[3]);

        void scaleCPU(float scale);
    
    private:
        
        std::array<Face,6> faces;
        

};
#endif