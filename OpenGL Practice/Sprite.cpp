#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>

#include <GLFW\glfw3.h>

#include <SOIL.h>

#include <MLLS\Matrix4.h>
#include <MLLS\Vector2.h>

struct VertexData {
	Matrix4 Verticies;
	Matrix4 Color;
	Matrix4 UVs;
};

class Sprite {
public:
	Sprite() {
		for (int i = 0; i < 48; i++) {
			GLData[i] = 0;
		}
		glGenBuffers(1, &uiIBO);
		glGenBuffers(1, &uiVBO);
		UpdateVBO();
		UpdateIBO();
	}
	~Sprite() {
		glDeleteBuffers(1, &uiIBO);
	}

	void InitializeSprite(GLuint a_ShaderProgram, float a_x, float a_y, float a_width, float a_height) {
		x = a_x;
		y = a_y;
		width = a_width;
		height = a_height;
		ShaderProgram = a_ShaderProgram;
		//Data
		for (int i = 0; i < 4; i++) {
			SpriteData.Verticies.values[i][2] = 0;
			SpriteData.Verticies.values[i][3] = 1;
			SpriteData.Color.values[i][0] = 1;
			SpriteData.Color.values[i][1] = 1;
			SpriteData.Color.values[i][2] = 1;
			SpriteData.Color.values[i][3] = 1;
			SpriteData.UVs.values[i][2] = 0;
			SpriteData.UVs.values[i][3] = 0;
		}
		//Vertecies
		SpriteData.Verticies.values[0][0] = x;
		SpriteData.Verticies.values[0][1] = y;//bottom left
		SpriteData.Verticies.values[1][0] = x + width;
		SpriteData.Verticies.values[1][1] = y;//bottom right
		SpriteData.Verticies.values[2][0] = x;
		SpriteData.Verticies.values[2][1] = y + height;//top left
		SpriteData.Verticies.values[3][0] = x + width;
		SpriteData.Verticies.values[3][1] = y + height;//top right
		//UVs
		SpriteData.UVs.values[0][0] = 0;
		SpriteData.UVs.values[0][1] = 0;//bottom left
		SpriteData.UVs.values[1][0] = 1;
		SpriteData.UVs.values[1][1] = 0;//bottom right
		SpriteData.UVs.values[2][0] = 0;
		SpriteData.UVs.values[2][1] = 1;//top left
		SpriteData.UVs.values[3][0] = 1;
		SpriteData.UVs.values[3][1] = 1;//top right
	}
	void InputTexture(GLuint a_TextureID) {
		TextureID = a_TextureID;
	}

	void Draw(GLuint a_UniformMatrix, float* a_Ortho) {
		//glUseProgram(uiProgramFlat);
		glUseProgram(ShaderProgram);
		glUniformMatrix4fv(a_UniformMatrix, 1, GL_FALSE, a_Ortho);

		//binders
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		//enable the vertex array state, since we're sending in an array of vertices
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//specify where our vertex array is, how many components each vertex has, 
		//the data type of each component and whether the data is normalised or not
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)(sizeof(float)* 4));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)(sizeof(float)* 8));
		//draw to the screen
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void UpdateVBO() {
		
		//bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
		//allocate space for vertices on the graphics card
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLData),NULL , GL_STATIC_DRAW);
		//get pointer to allocated space on the graphics card
		GLvoid* vBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//copy data to graphics card
		memcpy(vBuffer, GLData, sizeof(GLData));
		//unmap and unbind buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void UpdateIBO() {
		
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiIBO);
		//allocate space for index info on the graphics card
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(char), NULL, GL_STATIC_DRAW);
		//get pointer to newly allocated space on the graphics card
		GLvoid* iBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		//specify the order we'd like to draw our vertices.
		//In this case they are in sequential order
		for (int i = 0; i < 4; i++)
		{
			((char*)iBuffer)[i] = i;
		}
		//unmap and unbind
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	

	void UpdateData() {
		GLData[0] = SpriteData.Verticies.values[0][0];
		GLData[1] = SpriteData.Verticies.values[0][1];
		GLData[2] = SpriteData.Verticies.values[0][2];
		GLData[3] = SpriteData.Verticies.values[0][3];

		GLData[4] = SpriteData.Verticies.values[1][0];
		GLData[5] = SpriteData.Verticies.values[1][1];
		GLData[6] = SpriteData.Verticies.values[1][2];
		GLData[7] = SpriteData.Verticies.values[1][3];

		GLData[8] = SpriteData.Verticies.values[2][0];
		GLData[9] = SpriteData.Verticies.values[2][1];
		GLData[10] = SpriteData.Verticies.values[2][2];
		GLData[11] = SpriteData.Verticies.values[2][3];

		GLData[12] = SpriteData.Verticies.values[3][0];
		GLData[13] = SpriteData.Verticies.values[3][1];
		GLData[14] = SpriteData.Verticies.values[3][2];
		GLData[15] = SpriteData.Verticies.values[3][3];

		GLData[16] = SpriteData.Color.values[0][0];
		GLData[17] = SpriteData.Color.values[0][1];
		GLData[18] = SpriteData.Color.values[0][2];
		GLData[19] = SpriteData.Color.values[0][3];

		GLData[20] = SpriteData.Color.values[1][0];
		GLData[21] = SpriteData.Color.values[1][1];
		GLData[22] = SpriteData.Color.values[1][2];
		GLData[23] = SpriteData.Color.values[1][3];
		
		GLData[24] = SpriteData.Color.values[2][0];
		GLData[25] = SpriteData.Color.values[2][1];
		GLData[26] = SpriteData.Color.values[2][2];
		GLData[27] = SpriteData.Color.values[2][3];
		
		GLData[28] = SpriteData.Color.values[3][0];
		GLData[29] = SpriteData.Color.values[3][1];
		GLData[30] = SpriteData.Color.values[3][2];
		GLData[31] = SpriteData.Color.values[3][3];
		
		GLData[32] = SpriteData.UVs.values[0][0];
		GLData[33] = SpriteData.UVs.values[0][1];

		GLData[34] = SpriteData.UVs.values[1][0];
		GLData[35] = SpriteData.UVs.values[1][1];

		GLData[36] = SpriteData.UVs.values[2][0];
		GLData[37] = SpriteData.UVs.values[2][1];

		GLData[38] = SpriteData.UVs.values[3][0];
		GLData[39] = SpriteData.UVs.values[3][1];

		UpdateVBO();
		UpdateIBO();
	}

	GLuint uiVBO;
	GLuint uiIBO;
	GLuint ShaderProgram;
	GLuint TextureID;
	VertexData SpriteData;
	float GLData[40];
	float width;
	float height;
	float x, y;
};