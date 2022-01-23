#include "bloom.h"

Bloom::Bloom(unsigned int height, unsigned int width) {
	// //////////////////////////////
	// 1、初始化默认帧缓冲
	// //////////////////////////////
	glGenFramebuffers(1, &defaultFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	
	// 为默认帧缓冲附加空白的纹理
	glGenTextures(2, defaultColorBuffer);
	for (int i = 0; i < 2; ++i) {
		glBindTexture(GL_TEXTURE_2D, defaultColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, defaultColorBuffer[i], 0);
	}

	// 绑定一个渲染缓冲对象。由于默认的帧缓冲不需要读取缓冲，因此使用RBO速度更快
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// 为默认FBO绑定颜色附件。0号用于渲染原图，1号附件用于提取需要模糊的亮度
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR IN bloom:: defaultFBO is not complete!" << std::endl;

	// 帧缓冲置空
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// //////////////////////////////
	// 2、初始化高斯滤波用的缓冲
	// //////////////////////////////

	// 两个帧缓冲，一个用来水平滤波，另一个用于垂直滤波
	glGenFramebuffers(2, GaussFBO);
	glGenTextures(2, GaussColorBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GaussFBO[i]);
		glBindTexture(GL_TEXTURE_2D, GaussColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GaussColorBuffer[i], 0);
		// 不需要深度信息，只需要使用一个颜色纹理用作滤波就可以了
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR IN bloom:: GaussFBO [" << i << "] not complete!" << std::endl;
	}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 渲染屏幕显示的四边形
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}


Bloom::~Bloom() {
	glDeleteFramebuffers(1, &defaultFBO);
	glDeleteTextures(2, defaultColorBuffer);
	glDeleteRenderbuffers(1, &RBO);

	glDeleteFramebuffers(2, GaussFBO);
	glDeleteTextures(2, GaussColorBuffer);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void Bloom::bindDefaultFBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Bloom::GaussBlur(Shader& blurShader) {
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	blurShader.use();
	glActiveTexture(GL_TEXTURE0);

	for (unsigned int i = 0; i < amount; i++)
	{
		// 每次绑定不同的FBO作为滤波输出，分别对应水平滤波和垂直滤波
		glBindFramebuffer(GL_FRAMEBUFFER, GaussFBO[horizontal]);
		blurShader.setInt("horizontal", horizontal);
		// 第一次滤波的时候以默认FBO中的输出作为输入。
		// 之后水平滤波以之前垂直滤波的输出作为输入，垂直滤波以水平滤波作为输入
		glBindTexture(GL_TEXTURE_2D, first_iteration ? defaultColorBuffer[1] : GaussColorBuffer[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		blurShader.setInt("image", 0);

		// 将滤波的结果输出到当前FBO对应的颜色附件中
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Bloom::RanderToScreen(Shader& screenShader) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultColorBuffer[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GaussColorBuffer[0]);
	screenShader.setInt("screen", 0);
	screenShader.setInt("bloom", 1);
	// 输出到屏幕
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}