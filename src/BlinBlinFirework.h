#ifndef BlinBlinFirework_H
#define BlinBlinFirework_H

#include "firework.h"

class BlinBlinFirework :public Firework
{
private:
	void genTrails();
public:
	BlinBlinFirework();

	BlinBlinFirework(float, glm::fvec3, bool);

	~BlinBlinFirework();

	bool isAlive();

	void destroy();

	void init(fireworkParam fwp);

	void light(Shader& shder, float delta_time, int second_trails_num);

	bool isExploded();

	glm::fvec3 get_explode_position();

	glm::fvec4 get_explode_color();

private:
	float exist;
	std::vector<glm::fvec4> store;
	std::vector<ptr> trails;	// 产生拖尾的粒子团，第0个是释放时的主粒子
	std::vector<ptr> explosions; // 产生爆炸效果的粒子团
	float time_cnt, explode_time; // 计时器以及要爆炸的时间
	fireworkParam fwp; // 烟花的参数
	bool explodedOnce, explodedTwice;
	glm::fvec3 explodePosition;
	bool canExplodeTwice;
	glm::fvec4 explode_color;
	bool sound;
};


#endif