#ifndef GRIDSPHERE_H
#define GRIDSPHERE_H
#include <vector>

class GridSphere{
    public:
        void setShape();
        void setUpBuffers();
        unsigned int VAO;
        unsigned int VBO;

        unsigned int EBO;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        static constexpr int NUM_LINES = 10;

        std::vector<unsigned int> polarIndices;
        std::vector<unsigned int> meridianIndices;
        std::vector<unsigned int> parallelIndices;

        unsigned int indexPolarStartOffset;
        unsigned int indexMeridianStartOffset;
        unsigned int indexParalelStartOffset;


    private:
        float center[3] = {0.0f,0.0f,0.0f};
        float radius = 1;

        static constexpr double PI = 3.14159265358979323846;
        void setUpIndices();



        

};
#endif