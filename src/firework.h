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
	int max_trail; // 轨迹中包含的最多粒子数
	int min_trail; // 轨迹中包含的最少粒子数
};

struct fireworkParam {
	int trails_num;  // 烟花爆炸之后的轨迹数
	int explode_num; // 发生二次爆炸的粒子团数
	trailParam tp;   // 每一个拖尾的参数
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
	std::vector<ptr> trails;	// 产生拖尾的粒子团，第0个是释放时的主粒子
	std::vector<ptr> explosions; // 产生爆炸效果的粒子团
	float time_cnt, explode_time; // 计时器以及要爆炸的时间
	fireworkParam fwp; // 烟花的参数
	bool exploded;
	bool canExplodeTwice;
};


#endif