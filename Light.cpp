#include "Light.h"
Light::Light(glm::mat4 viewMatrix) {
	lightVertices = {
{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.,0.}},
{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.,0.}},
{{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.,0.}},
{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.,0.}},
{{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.,0.}},
{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.,0.}},
{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.,0.}},
{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.,0.}}
	};

	lightIndices = {
	  2, 3, 4,
	  8, 7, 6,
	  1, 5, 6,
	  2, 6, 7,
	  7, 8, 4,
	  1, 4, 8,
	  1, 2, 4,
	  5, 8, 6,
	  2, 1, 6,
	  3, 2, 7,
	  3, 7, 4,
	  5, 1, 8
	};

	for (unsigned int i = 0; i < lightIndices.size(); i++)
	{
		lightIndices[i] = lightIndices[i] - 1;
	}

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;
}

Light::~Light() {
	
}

void Light::Render() {

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), &lightVertices[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lightVertices), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glDrawElements(GL_TRIANGLES, sizeof(lightIndices), GL_UNSIGNED_INT, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ARRAY_BUFFER, IB);
	glBufferData(GL_ARRAY_BUFFER, lightIndices.size() * sizeof(unsigned int), &lightIndices[0], GL_STATIC_DRAW);

}
