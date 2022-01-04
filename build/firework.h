#pragma once
#ifndef FIREWORK_H
#define FIREWORK_H

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

#include <irrKlang/irrKlang.h>

typedef std::shared_ptr<ParticleSystem> ptr;


enum MODE{NORMAL_FIREWORK, BIG_FUCKING_FIREWORK, RANDOM_LITTLE_FIREWORK};


struct trailParam {
	int max_trail; // �켣�а��������������
	int min_trail; // �켣�а���������������
};

struct fireworkParam {
	int trails_num;  // �̻���ը֮��Ĺ켣��
	int explode_num; // �������α�ը����������
	trailParam tp;   // ÿһ����β�Ĳ���
};

class Firework {
private:
	virtual void genTrails();
public:
	Firework();

	Firework(float);

	~Firework();

	virtual bool isAlive();

	virtual void destroy();

	virtual void init(fireworkParam fwp);

	virtual void light(Shader& shder, float delta_time);

private:
	std::vector<ptr> trails;	// ������β�������ţ���0�����ͷ�ʱ��������
	std::vector<ptr> explosions; // ������ըЧ����������
	float time_cnt, explode_time; // ��ʱ���Լ�Ҫ��ը��ʱ��
	fireworkParam fwp; // �̻��Ĳ���
	bool exploded;
	bool canExplodeTwice;
};


#endif