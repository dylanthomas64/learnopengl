// base class for platonic solids (without textures)

#ifndef PLATONIC_SOLID_H
#define PLATONIC_SOLID_H

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
};

enum class PolygonType {
    Tetrahedron,
    Cube,
    Octahedron,
};


class PlatonicSolid {
public:

    PolygonType type;
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    // constructor
    PlatonicSolid(PolygonType type);

    //copy constructor
    PlatonicSolid(PlatonicSolid& platonicsolid);
    //destructor
    //~PlatonicSolid();

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};


// helper functions
std::vector<Vertex> get_cube_vertices();
std::vector<Vertex> get_tetra_vertices();
std::vector<Vertex> get_octa_vertices();

#endif