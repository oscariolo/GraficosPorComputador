#ifndef GRIDSPHERE_H
#define GRIDSPHERE_H
#include <vector>

class GridSphere{
    public:
        GridSphere();
        void setShape();
        void bindSphere();
        unsigned int VAO;
        unsigned int VBO;
        std::vector<float> meridianVertices;
        std::vector<float> paralelVertices;

    private:
        float center[3] = {0.0f,0.0f,0.0f};
        float radius = 0.4;
        static constexpr int VERTEX_SIZE = 10;
        static constexpr int NUM_LINES = 5;


        

};
#endif