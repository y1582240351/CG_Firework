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

class Bloom {
private:
	GLuint configFBO; // 原图所用的帧缓冲
	GLuint blurFBO[2]; // 滤波时所用的帧缓冲
	GLuint color_buffer[2]; // 绑定到原图的颜色附件
	GLuint blur_buffer[2]; // 滤波时所用的颜色附件
	GLuint rbo_depth; // 原图渲染时所用的深度缓冲
	GLuint vao, vbo; // 滤波所用的帧缓冲对应的vao
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	const unsigned int SCR_WIDTH;
	const unsigned int SCR_HEIGHT;

	void initQuad(); //  初始化滤波所用的四边形框

public:
	Bloom(int, int);  // 初始化默认的帧缓冲（FrameBuffer）
	~Bloom();

	void initBlurFB(); // 初始化滤波所用的帧缓冲

	void activateConfigFB(); // 激活默认帧缓冲

	void blur(Shader& blur_shader); // 进行高斯滤波

	void finalShade(Shader& final_shader); // 调用最终的渲染
};


#endif