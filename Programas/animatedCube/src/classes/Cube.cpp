#include "Cube.h"
#include <glad/glad.h>
#include <iostream>

void Cube::instantiate(){

    //generamos el cubo partiendo desde las caras, de ahi por cada cara sus aristas con sus vertices

    for(int i = 0; i < 6; i++){

        Face newFace;

        this->faces[i] = newFace;


    }

    

}