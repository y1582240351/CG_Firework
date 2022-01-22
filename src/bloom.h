#pragma once
#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "particle_system.h"
#include "utility_tool.h"

#define DEFAULT 0
#define GAUSS 1

// ´°¿Ú´óÐ¡
//const unsigned int SCR_WIDTH = 1600;
//const unsigned int SCR_HEIGHT = 900;

class Bloom {
public:
	Bloom(unsigned int height, unsigned int width);
	~Bloom();

	void bindDefaultFBO();

	void GaussBlur(Shader &blurShader);

	void RanderToScreen(Shader& screenShader);

private:
	GLuint defaultFBO, GaussFBO[2];
	GLuint defaultColorBuffer[2], GaussColorBuffer[2];
	GLuint RBO;
	GLuint VAO, VBO, EBO;
};


#endif