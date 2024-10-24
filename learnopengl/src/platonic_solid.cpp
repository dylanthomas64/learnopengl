#include <PlatonicSolid.h>
#include <array>




// expects a type of enum
PlatonicSolid::PlatonicSolid(PolygonType type)
{
    this->type = type;
    switch (type)
    {
        case PolygonType::Cube:
        {
            this->vertices = get_cube_vertices();
            break;
        }
            
        case PolygonType::Tetrahedron:
        {
            this->vertices = get_tetra_vertices();
            break;
        }
        case PolygonType::Octahedron:
            this->vertices = get_octa_vertices();
            break;
            
        default:
        {
            this->vertices = get_cube_vertices();
            break;
        }

    }
    setupMesh();
}

void PlatonicSolid::Draw(Shader& shader)
{
    // draw mesh
    glBindVertexArray(VAO);

    if (!indices.empty())
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void PlatonicSolid::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (!indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }


    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
}

// helper functions
std::vector<Vertex> get_cube_vertices()
{
    // vertex data
    // cube cubeVertexData + normals
    float cubeVertexData[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    std::vector<Vertex> vertices;
    for (int i = 0; i <= (sizeof(cubeVertexData) / sizeof(float)); i += 6) {
        Vertex vertex;
        vertex.Position = glm::vec3(cubeVertexData[i], cubeVertexData[i + 1], cubeVertexData[i + 2]);
        vertex.Normal = glm::vec3(cubeVertexData[i + 3], cubeVertexData[i + 4], cubeVertexData[i + 5]);

        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<Vertex> get_tetra_vertices()
{
    // Vertices for tetrahedron (position and normals)
    float tetrahedronVertexData[] = {
        // Triangle 1 (v0, v1, v2)
        1.0f,  1.0f,  1.0f,   0.577f,  0.577f,  0.577f,  // v0 position, normal
       -1.0f, -1.0f,  1.0f,   0.577f,  0.577f,  0.577f,  // v1 position, normal
       -1.0f,  1.0f, -1.0f,   0.577f,  0.577f,  0.577f,  // v2 position, normal

       // Triangle 2 (v0, v1, v3)
       1.0f,  1.0f,  1.0f,   0.577f, -0.577f,  0.577f,  // v0 position, normal
      -1.0f, -1.0f,  1.0f,   0.577f, -0.577f,  0.577f,  // v1 position, normal
       1.0f, -1.0f, -1.0f,   0.577f, -0.577f,  0.577f,  // v3 position, normal

       // Triangle 3 (v0, v2, v3)
       1.0f,  1.0f,  1.0f,   0.577f,  0.577f, -0.577f,  // v0 position, normal
      -1.0f,  1.0f, -1.0f,   0.577f,  0.577f, -0.577f,  // v2 position, normal
       1.0f, -1.0f, -1.0f,   0.577f,  0.577f, -0.577f,  // v3 position, normal

       // Triangle 4 (v1, v2, v3)
      -1.0f, -1.0f,  1.0f,   -0.577f,  0.577f,  0.577f, // v1 position, normal
      -1.0f,  1.0f, -1.0f,   -0.577f,  0.577f,  0.577f, // v2 position, normal
       1.0f, -1.0f, -1.0f,   -0.577f,  0.577f,  0.577f, // v3 position, normal
    };

    std::vector<Vertex> vertices;
    for (int i = 0; i <= (sizeof(tetrahedronVertexData) / sizeof(float)); i += 6) {
        Vertex vertex;
        vertex.Position = glm::vec3(tetrahedronVertexData[i], tetrahedronVertexData[i + 1], tetrahedronVertexData[i + 2]);
        vertex.Normal = glm::vec3(tetrahedronVertexData[i + 3], tetrahedronVertexData[i + 4], tetrahedronVertexData[i + 5]);

        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<Vertex> get_octa_vertices()
{
    float octahedronVertices[] = {
        // Triangle 1 (v0, v2, v4)
        1.0f,  0.0f,  0.0f,   0.577f,  0.577f,  0.577f,  // v0 position, normal
        0.0f,  1.0f,  0.0f,   0.577f,  0.577f,  0.577f,  // v2 position, normal
        0.0f,  0.0f,  1.0f,   0.577f,  0.577f,  0.577f,  // v4 position, normal

        // Triangle 2 (v0, v3, v4)
        1.0f,  0.0f,  0.0f,   0.577f, -0.577f,  0.577f,  // v0 position, normal
        0.0f, -1.0f,  0.0f,   0.577f, -0.577f,  0.577f,  // v3 position, normal
        0.0f,  0.0f,  1.0f,   0.577f, -0.577f,  0.577f,  // v4 position, normal

        // Triangle 3 (v1, v2, v4)
       -1.0f,  0.0f,  0.0f,  -0.577f,  0.577f,  0.577f,  // v1 position, normal
        0.0f,  1.0f,  0.0f,  -0.577f,  0.577f,  0.577f,  // v2 position, normal
        0.0f,  0.0f,  1.0f,  -0.577f,  0.577f,  0.577f,  // v4 position, normal

        // Triangle 4 (v1, v3, v4)
       -1.0f,  0.0f,  0.0f,  -0.577f, -0.577f,  0.577f,  // v1 position, normal
        0.0f, -1.0f,  0.0f,  -0.577f, -0.577f,  0.577f,  // v3 position, normal
        0.0f,  0.0f,  1.0f,  -0.577f, -0.577f,  0.577f,  // v4 position, normal

        // Triangle 5 (v0, v2, v5)
        1.0f,  0.0f,  0.0f,   0.577f,  0.577f, -0.577f,  // v0 position, normal
        0.0f,  1.0f,  0.0f,   0.577f,  0.577f, -0.577f,  // v2 position, normal
        0.0f,  0.0f, -1.0f,   0.577f,  0.577f, -0.577f,  // v5 position, normal

        // Triangle 6 (v0, v3, v5)
        1.0f,  0.0f,  0.0f,   0.577f, -0.577f, -0.577f,  // v0 position, normal
        0.0f, -1.0f,  0.0f,   0.577f, -0.577f, -0.577f,  // v3 position, normal
        0.0f,  0.0f, -1.0f,   0.577f, -0.577f, -0.577f,  // v5 position, normal

        // Triangle 7 (v1, v2, v5)
       -1.0f,  0.0f,  0.0f,  -0.577f,  0.577f, -0.577f,  // v1 position, normal
        0.0f,  1.0f,  0.0f,  -0.577f,  0.577f, -0.577f,  // v2 position, normal
        0.0f,  0.0f, -1.0f,  -0.577f,  0.577f, -0.577f,  // v5 position, normal

        // Triangle 8 (v1, v3, v5)
       -1.0f,  0.0f,  0.0f,  -0.577f, -0.577f, -0.577f,  // v1 position, normal
        0.0f, -1.0f,  0.0f,  -0.577f, -0.577f, -0.577f,  // v3 position, normal
        0.0f,  0.0f, -1.0f,  -0.577f, -0.577f, -0.577f,  // v5 position, normal
    };

    std::vector<Vertex> vertices;
    for (int i = 0; i <= sizeof(octahedronVertices)/sizeof(float); i += 6) {
        Vertex vertex;
        vertex.Position = glm::vec3(octahedronVertices[i], octahedronVertices[i + 1], octahedronVertices[i + 2]);
        vertex.Normal = glm::vec3(octahedronVertices[i + 3], octahedronVertices[i + 4], octahedronVertices[i + 5]);

        vertices.push_back(vertex);
    }
    return vertices;
}