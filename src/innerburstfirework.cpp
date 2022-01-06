#include "innerburstfirework.h"

// sound
extern irrklang::ISoundEngine* SoundEngine;

innerburstfirework::innerburstfirework() : time_cnt(0.0f), explodedOnce(false), explodedTwice(false){

}

innerburstfirework::~innerburstfirework() {

	std::cout << "revoke" << std::endl;

}

innerburstfirework::innerburstfirework(float explode_time, glm::fvec3 pos) : time_cnt(0.0f), explode_time(explode_time), explodedOnce(false),  explodedTwice(false) {
	//1
	ptr p = std::make_shared<ParticleSystem>(5000, true);
	Particle base;
	base.position = pos;
	base.color = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f);
	base.velocity = glm::fvec3(0.0f, 0.450f, 0.0f);
	base.size = 10.0f;
	base.life = explode_time;
	GenParam param;
	param.gen_rate = 300;
	param.size = 1;
	param.size_rate = 0.0015;
	param.vel_base_rate = 1.0;
	param.vel_random_rate = 0.005;
	param.delay = 1;
	param.life_rate[0] = 0.8;
	param.life_rate[1] = 0.9;
	p->initTrailGen(base, param);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	trails.push_back(p);
}


/// <summary>
/// 初始化烟花数据
/// </summary>
/// <param name="base_fwp">烟花初始数据</param>
void innerburstfirework::init(fireworkParam base_fwp) {
	fwp = base_fwp;
	// ...其他要用到的参数
}


void innerburstfirework::destroy()
{
	trails.clear();
}

bool innerburstfirework::isAlive()
{
	bool flag = 0;
	for (int i = 0; i < trails.size(); ++i)
	{
		if (trails[i]->isDied() == false)
		{
			flag = 1;
			return true;
		}
	}
	return false;
}

/// <summary>
/// 燃放烟花时的更新函数
/// </summary>
/// <param name="shader">渲染所用的着色器</param>
/// <param name="delta_time">每帧之间的间隔时间</param>
void innerburstfirework::light(Shader& shader, float delta_time, int second_trails_num) {

	time_cnt += delta_time;
	if (time_cnt < explode_time) {
		static bool sound = false;
		trails[0]->trailGen(delta_time, 0.85, 0.06);
		trails[0]->draw(shader);
		if (!sound) {
			SoundEngine->play2D("./rise.wav", GL_FALSE);
			sound = true;
		}

	}
	else {
		if (!explodedOnce) { // 烟花第一次爆炸

			explodedOnce = true;
			explodePosition = trails[0]->getHeadParticlePos();
			genTrails();
			SoundEngine->play2D("./explosion.wav", GL_FALSE);
		}
		//fwp.trails_num
		for (int i = 0; i < trails.size(); ++i) {

			if (trails[1]->testLife(5.0f) == true && !explodedTwice)
			{
				std::cout << "gen" << std::endl;
				explodedTwice = true;
				for (int j = 0; j < second_trails_num; ++j) {
					ptr p = std::make_shared<ParticleSystem>(10000, false);
					// 初始化生成的节点，参数可以改
					Particle base;
					base.position = explodePosition;
					//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
					base.color = glm::fvec4(0.98f, 0.98f, 0.82f, 1.0f);
					base.velocity = 0.15f * sphereRandom();
					base.size = 1.0f;
					base.life = 2.0f;
					GenParam param;
					param.gen_rate = 150;
					param.size = 1;
					param.size_rate = 0.001;
					param.vel_base_rate = 1.0;
					param.vel_random_rate = 0.0000;
					param.delay = 1;
					param.life_rate[0] = 0.6;
					param.life_rate[1] = 0.7;
					p->initTrailGen(base, param);
					trails.push_back(p);
				}
			}
			if (trails[i]->isDied()) // 不再渲染死亡的粒子团
			{
				continue;
			}
			trails[i]->trailGen(delta_time, 0.7, 0.07);
			trails[i]->draw(shader);
		}
	}
}


/// <summary>
/// 生成烟花炸开时产生的流苏
/// </summary>
void innerburstfirework::genTrails() {
	explode_color = trails[0]->get_color();
	glm::fvec3 explode_pos = trails[0]->posTrail();
	trails.resize(fwp.trails_num+1);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	for (int i = 1; i <= fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10000, true);
		// 初始化生成的节点，参数可以改
		Particle base;
		base.position = explode_pos;
		//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
		base.color = glm::fvec4(ColorRandom(), 1.0f);
		base.velocity = 0.15f * sphereRandom();
		base.size = 3.0f;
		base.life = 6.0;
		GenParam param;
		param.gen_rate = 150;
		param.size = 1;
		param.size_rate = 0.001;
		param.vel_base_rate = 1.0;
		param.vel_random_rate = 0.0000;
		param.delay = 1;
		param.life_rate[0] = 0.6;
		param.life_rate[1] = 0.7;
		p->initTrailGen(base, param);
		trails[i] = p;
	}
}

bool innerburstfirework::isExploded() {
	return explodedOnce;
}

glm::fvec3 innerburstfirework::get_explode_position() {
	return explodePosition;
}

glm::fvec4 innerburstfirework::get_explode_color() {
	return explode_color;
}