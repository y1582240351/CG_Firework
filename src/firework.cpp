#include "firework.h"

// sound
extern irrklang::ISoundEngine* SoundEngine;

Firework::Firework() : time_cnt(0.0f), exploded(false) {

}

Firework::~Firework() {

}

Firework::Firework(float explode_time) : time_cnt(0.0f), explode_time(explode_time), exploded(false) {
	ptr p = std::make_shared<ParticleSystem>(5000);
	Particle base;
	base.position = glm::fvec3(0.0f, -1.0f, 0.0f);
	base.color = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f);
	base.velocity = glm::fvec3(0.0f, 0.450f, 0.0f);
	base.size = 10.0f;
	base.life = 4.0f;
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
void Firework::init(fireworkParam base_fwp) {
	fwp = base_fwp;
	// ...其他要用到的参数
}


/// <summary>
/// 燃放烟花时的更新函数
/// </summary>
/// <param name="shader">渲染所用的着色器</param>
/// <param name="delta_time">每帧之间的间隔时间</param>
void Firework::light(Shader& shader, float delta_time) {
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
		if (!exploded) { // 烟花第一次爆炸
			exploded = true;
			genTrails();
			SoundEngine->play2D("./explosion.wav", GL_FALSE);
		}
		for (int i = 0; i < fwp.trails_num; ++i) {
			if (trails[i]->isDied()) // 不再渲染死亡的粒子团
				continue;
			trails[i]->trailGen(delta_time, 0.7, 0.07);
			trails[i]->draw(shader);
		}
	}
}


/// <summary>
/// 生成烟花炸开时产生的流苏
/// </summary>
void Firework::genTrails() {
	glm::fvec3 explode_pos = trails[0]->posTrail();
	trails.resize(fwp.trails_num);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	for (int i = 0; i < fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10000);
		// 初始化生成的节点，参数可以改
		Particle base;
		base.position = explode_pos;
		//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
		base.color = glm::fvec4(ColorRandom(), 1.0f);
		base.velocity = 0.15f * sphereRandom();
		base.size = 6.0f;
		base.life = 5.0f;
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