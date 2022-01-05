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
	GLuint configFBO; // ԭͼ���õ�֡����
	GLuint blurFBO[2]; // �˲�ʱ���õ�֡����
	GLuint color_buffer[2]; // �󶨵�ԭͼ����ɫ����
	GLuint blur_buffer[2]; // �˲�ʱ���õ���ɫ����
	GLuint rbo_depth; // ԭͼ��Ⱦʱ���õ���Ȼ���
	GLuint vao, vbo; // �˲����õ�֡�����Ӧ��vao
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	const unsigned int SCR_WIDTH;
	const unsigned int SCR_HEIGHT;

	void initQuad(); //  ��ʼ���˲����õ��ı��ο�

public:
	Bloom(int, int);  // ��ʼ��Ĭ�ϵ�֡���壨FrameBuffer��
	~Bloom();

	void initBlurFB(); // ��ʼ���˲����õ�֡����

	void activateConfigFB(); // ����Ĭ��֡����

	void blur(Shader& blur_shader); // ���и�˹�˲�

	void finalShade(Shader& final_shader); // �������յ���Ⱦ
};


#endif