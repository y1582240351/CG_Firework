#include "firework.h"


Firework::Firework() : time_cnt(0.0f), exploded(false){

}

Firework::~Firework() {

}

Firework::Firework(float explode_time) : time_cnt(0.0f), explode_time(explode_time), exploded(false){
	ptr p = std::make_shared<ParticleSystem>(10);
	Particle base;
	base.position = glm::fvec3(0.0f, -1.0f, 0.0f);
	base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
	base.velocity = glm::fvec3(0.0f, 0.1f, 0.0f);
	base.size = 4.0f;
	base.life = 10.0f;
	p->initTrail(base);
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
void Firework::light(Shader &shader, float delta_time) {
	time_cnt += delta_time;
	if (time_cnt < explode_time) {
		trails[0]->trail(delta_time);
		trails[0]->draw(shader);
	}
	else {
		if (!exploded) { // 烟花第一次爆炸
			exploded = true;
			genTrails();
		}
		for (int i = 0; i < fwp.trails_num; ++i) {
			if (trails[i]->isDied()) // 不再渲染死亡的粒子团
				continue;
			trails[i]->trail(delta_time);
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
	for (int i = 0; i < fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10);
		// 初始化生成的节点，参数可以改
		Particle base;
		base.position = explode_pos;
		base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
		base.velocity = 0.3f * sphereRandom();
		base.size = 4.0f;
		base.life = 3.0f;
		p->initTrail(base);
		trails[i] = p;
	}
}