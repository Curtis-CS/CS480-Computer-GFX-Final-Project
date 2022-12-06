#include "Sphere.h"

Sphere::Sphere()
{
    setupVertices();
    setupBuffers();
    setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);
}

Sphere::Sphere(glm::vec3 pivot)
{
    setupVertices();
    setupBuffers();
    setupModelMatrix(pivot, glm::radians(0.0f), 1.f);
}

Sphere::Sphere(glm::vec3 pivot, float angle)
{
    setupVertices();
    setupBuffers();
    setupModelMatrix(pivot, glm::radians(angle), 1.f);
}

Sphere::Sphere(glm::vec3 pivot, float angle, float scale, float sizeNew, const char* fileName)
{
    m_texture = new Texture(fileName);
    // load texture from file
    if (m_texture)
        hasTex = true;
    else
        hasTex = false;
    size = sizeNew;
    setupVertices();
    setupBuffers();
    setupModelMatrix(pivot, glm::radians(angle), scale);

}


void Sphere::Render(GLint positionAttribLoc, GLint normalAttribLoc, GLint texAttribLoc, GLint hasTexture)
{
    glBindVertexArray(vao);
    // Enable Vertext Attributes
    glEnableVertexAttribArray(positionAttribLoc);
    glEnableVertexAttribArray(normalAttribLoc);
    glEnableVertexAttribArray(texAttribLoc);

    // Bind your VBO buffer(s) and then setup vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(normalAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    // Bind your index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    if (m_texture != NULL)
    {

        glUniform1i(hasTexture, true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->getTextureID());
    }
    else
    {
        glUniform1i(hasTexture, false);
    }

    // Render
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Disable Vertex Attribuates
    glDisableVertexAttribArray(positionAttribLoc);
    glDisableVertexAttribArray(normalAttribLoc);
    glDisableVertexAttribArray(texAttribLoc);
}



void Sphere::setupVertices() {

    int numVertices = (size + 1) * (size + 1);
    int numIndices = size * size * 6;
    for (int i = 0; i < numVertices; i++)
    {
        Vertices.push_back({ {0,0,0},{0,0,0},{0, 0} });
    }
    for (int i = 0; i < numIndices; i++)
    {
        Indices.push_back(0);
    }

    for (int i = 0; i <= size; i++)
    {
        for (int j = 0; j <= size; j++)
        {
            float y = (float)cos(toRadians(180.f - i * 180.f / size));
            float x = -(float)cos(toRadians(j * 360.f / size)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.f / size)) * (float)abs(cos(asin(y)));
            Vertices[i * (size + 1) + j] = { glm::vec3(x, y, z), glm::vec3(x, y, z), glm::vec2(y, x)};
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Indices[6 * (i * size + j) + 0] = i * (size + 1) + j;
            Indices[6 * (i * size + j) + 1] = i * (size + 1) + j + 1;
            Indices[6 * (i * size + j) + 2] = (i + 1) * (size + 1) + j;
            Indices[6 * (i * size + j) + 3] = i * (size + 1) + j + 1;
            Indices[6 * (i * size + j) + 4] = (i + 1) * (size + 1) + j + 1;
            Indices[6 * (i * size + j) + 5] = (i + 1) * (size + 1) + j;
        }
    }

}

float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Sphere::setupBuffers() {
    // For OpenGL 3
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void Sphere::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
    pivotLocation = pivot;
    model = glm::translate(glm::mat4(1.0f), pivotLocation);
    model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
    model *= glm::scale(glm::vec3(scale, scale, scale));
}

void Sphere::Update(glm::mat4 matModel) {

    model = matModel;

}