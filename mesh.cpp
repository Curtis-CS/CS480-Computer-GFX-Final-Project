#include "mesh.h"

Mesh::Mesh()
{
	// Vertex Set Up
	// No mesh

	// Model Set Up
	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}

}

Mesh::Mesh(glm::vec3 pivot, const char* fname)
{
	// Vertex Set Up
	loadModelFromFile(fname);
	fileName = fname;

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	hasTex = false;
}

Mesh::Mesh(glm::vec3 pivot, const char* fname, const char* tname)
{
	// Vertex Set Up
	loadModelFromFile(fname);
	fileName = fname;

	// Model Set Up
	angle = 0.0f;
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	// load texture from file
	m_texture = new Texture(tname);
	if (m_texture)
		hasTex = true;
	else
		hasTex = false;
}


Mesh::~Mesh()
{
	Vertices.clear();
	Indices.clear();
}

void Mesh::Update(glm::mat4 inmodel)
{
	//OffsetRotation += .0001;
	model = inmodel;
	//model *= glm::rotate(glm::mat4(1.0f), 1 * (float)OffsetRotation, glm::vec3(0., 1., 0.));

}

glm::mat4 Mesh::GetModel()
{
	return model;
}

void Mesh::Render(GLint posAttribLoc, GLint colAttribLoc)
{

	glBindVertexArray(vao);

	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
}

void Mesh::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint hasTexture)
{
	glBindVertexArray(vao);
	// Enable Vertext Attributes
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);
	glEnableVertexAttribArray(tcAttribLoc);

	// Bind your VBO buffer(s) and then setup vertex attribute pointers
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(tcAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

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
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
	glDisableVertexAttribArray(tcAttribLoc);
}


bool Mesh::InitBuffers() {

	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

bool Mesh::loadModelFromFile(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene) {
		printf("couldn't open the .obj file. \n");
		return false;
	}

	const int ivertTotalSize = 2 * sizeof(aiVector3D);

	int iTotalVerts = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++) {
				aiVector3D& pTexCoord = mesh->mTextureCoords[i][j];
				// update here for each mesh's vertices to assign position, normal, and texture coordinates
				if (fileName == "assets\\Objects\\Ring.png")
				{
					Vertices.push_back(Vertex(glm::vec3(mesh->mVertices[face.mIndices[k]].x, mesh->mVertices[face.mIndices[k]].y, mesh->mVertices[face.mIndices[k]].z),
						glm::vec3(mesh->mNormals[face.mIndices[k]].x, mesh->mNormals[face.mIndices[k]].y, mesh->mNormals[face.mIndices[k]].z),
						glm::vec2(0, 0)));
				}
				else
				{
					Vertices.push_back(Vertex(glm::vec3(mesh->mVertices[face.mIndices[k]].x, mesh->mVertices[face.mIndices[k]].y, mesh->mVertices[face.mIndices[k]].z),
						glm::vec3(mesh->mNormals[face.mIndices[k]].x, mesh->mNormals[face.mIndices[k]].y, mesh->mNormals[face.mIndices[k]].z),
						glm::vec2(pTexCoord.x, pTexCoord.y)));
				}
			}

		}
		iTotalVerts += mesh->mNumVertices;
	}
	for (int i = 0; i < Vertices.size(); i++) {
		Indices.push_back(i);
	}
}
