#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdlib.h>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "stb_image.h"
#include "shader.hpp"

#define VERTEX_NUM 108


class SkyBox {
public:
	SkyBox();
	~SkyBox();
	void loadMap(std::vector<std::string>& file_names);
	void draw(Shader& shader);
private:
	GLuint texture, vao, vbo;
	static float vertices[VERTEX_NUM];
};


#endif
