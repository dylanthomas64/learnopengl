#include "Rectangle.h"
#include <stb_image.h>

Rectangle::Rectangle() {
	setupMesh();
}

void Rectangle::setupMesh() {

	// create rectangel vertex data

	Vertex vertex;
	// left top
	vertex.Position = glm::vec3(-1.0, 1.0, 0.0);
	vertex.Normal = glm::vec3(0.0, 0.0, -1.0);
	vertex.TexCoords = glm::vec2(-1.0, 1.0);
	this->vertices.push_back(vertex);
	// left bottom
	vertex.Position = glm::vec3(-1.0, -1.0, 0.0);
	vertex.Normal = glm::vec3(0.0, 0.0, -1.0);
	vertex.TexCoords = glm::vec2(-1.0, -1.0);
	this->vertices.push_back(vertex);
	// right bottom
	vertex.Position = glm::vec3(1.0, -1.0, 0.0);
	vertex.Normal = glm::vec3(0.0, 0.0, -1.0);
	vertex.TexCoords = glm::vec2(1.0, -1.0);
	this->vertices.push_back(vertex);
	// right top
	vertex.Position = glm::vec3(1.0, 1.0, 0.0);
	vertex.Normal = glm::vec3(0.0, 0.0, -1.0);
	vertex.TexCoords = glm::vec2(1.0, 1.0);
	this->vertices.push_back(vertex);

    //create indices data
    // 0, 1, 2, 0, 2, 3
    indices = std::vector<unsigned int>{1, 2, 3};


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
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}
void Rectangle::loadTexture(std::string path) {

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    this->textureID = textureID;
}

void Rectangle::Draw(Shader& shader)
{
    // draw mesh
    glBindVertexArray(VAO);

    if (!indices.empty())
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}