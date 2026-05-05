#ifndef GRIDSPHERE_H
#define GRIDSPHERE_H
#include <vector>

class GridSphere{
    public:
        void setShape();
        void bindSphere();
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> vertices;
        unsigned int parallelCount;


    private:
        float center[3] = {0.0f,0.0f,0.0f};
        float radius = 0.7;
        static constexpr int NUM_LINES = 15;
        static constexpr int NUM_VERTICES = NUM_LINES * 2 * 3;
        static constexpr double PI = 3.14159265358979323846;


        

};
#endif