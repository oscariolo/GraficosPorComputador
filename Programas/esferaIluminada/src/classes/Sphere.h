#ifndef SPHERE_H
#define SPHERE_H
#include <vector>
#include "Polyhedron.h"

class Sphere : public Polyhedron {
    // Class definition
    public:
    void generateSphere(unsigned int numSubdivisions = 5);

};


#endif // SPHERE_H