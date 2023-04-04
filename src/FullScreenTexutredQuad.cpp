
#include "FullScreenTexutredQuad.h"


void FullScreenTexturedQuad::Settings(int windowWidth, int windowHeight) {
	glGenVertexArrays(1, &vao);	// create 1 vertex array object
	glBindVertexArray(vao);		// make it active

	unsigned int vbo;		// vertex buffer objects
	glGenBuffers(1, &vbo);	// Generate 1 vertex buffer objects

	// vertex coordinates: vbo0 -> Attrib Array 0 -> vertexPosition of the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // make it active, it is an array
	float vertexCoords[] = { -1, -1,  1, -1,  1, 1,  -1, 1 };	// two triangles forming a quad
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenTextures(1, &textureId);  				// id generation
	glBindTexture(GL_TEXTURE_2D, textureId);    // binding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // sampling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FullScreenTexturedQuad::LoadTexture(std::vector<Vec4f>& image) {
	glBindTexture(GL_TEXTURE_2D, textureId);    // binding
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, &image[0]); // To GPU
}

void FullScreenTexturedQuad::Draw() {
	glBindVertexArray(vao);	// make the vao and its vbos active playing the role of the data source
	int location = glGetUniformLocation(0, "textureUnit");
	const unsigned int textureUnit = 0;
	if (location >= 0) {
		glUniform1i(location, textureUnit);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);	// draw two triangles forming a quad
}
