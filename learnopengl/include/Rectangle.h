// base class for platonic solids (without textures)

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader_s.h>

#include <string>
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};


class Rectangle {
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int textureID;

  
    unsigned int VAO;

    // constructor
    Rectangle();

    //copy constructor
    //destructor
    //~PlatonicSolid();

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();

    void loadTexture(std::string path);
};

#endif