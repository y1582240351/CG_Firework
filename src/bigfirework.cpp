#include "bigfirework.h"

// sound
extern irrklang::ISoundEngine* SoundEngine;

bigfirework::bigfirework() : time_cnt(0.0f), exploded(false) {

}

bigfirework::~bigfirework() {

	std::cout << "revoke" << std::endl;

}

bigfirework::bigfirework(float explode_time, glm::fvec3 pos, bool option, bool _randomcolor) : time_cnt(0.0f), explode_time(explode_time), exploded(false) {

	this->exist = 1.0f;
	this->randomcolor = _randomcolor;
	

	ptr p = std::make_shared<ParticleSystem>(5000, true);
	Particle base;
	if (option) {
		base.position = pos;
	}
	else base.position = glm::fvec3(floatRandom(-0.75, 0.75), pos.y, -1.0f);;

	base.color = glm::fvec4(1.0f, 1.0f, 1.0f, 3.0f);
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



	Particle base1;
	base1.position = glm::fvec3(-1.5f, -1.5f, -1.5f);
	base1.color = glm::fvec4(1.0f, 1.0f, 1.0f, 3.0f);
	base1.velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
	base1.size = 1.0f;
	base1.life = explode_time;
	GenParam param1;
	param1.gen_rate = 300;
	param1.size = 1;
	param1.size_rate = 0.0015;
	param1.vel_base_rate = 1.0;
	param1.vel_random_rate = 0.005;
	param1.delay = 1;
	param1.life_rate[0] = 0.8;
	param1.life_rate[1] = 0.9;


	this->dummy = std::make_shared<ParticleSystem>(10, true);
	this->dummy->initTrailGen(base1, param1);


	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	trails.push_back(p);
	sound = false;
}


/// <summary>
/// 初始化烟花数据
/// </summary>
/// <param name="base_fwp">烟花初始数据</param>
void bigfirework::init(fireworkParam base_fwp) {
	fwp = base_fwp;
	// ...其他要用到的参数
}


void bigfirework::destroy()
{
	trails.clear();
}

bool bigfirework::isAlive()
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
void bigfirework::light(Shader& shader, float delta_time, int second_trails_num) {

	time_cnt += delta_time;
	if (time_cnt < explode_time) {
		
		trails[0]->trailGen(delta_time, 0.85, 0.06);
		trails[0]->draw(shader);
		if (!sound) {
			SoundEngine->play2D("../sound/rise.wav", GL_FALSE);
			sound = true;
		}

	}
	else {
		if (!exploded) { // 烟花第一次爆炸
			exploded = true;
			genTrails();
			SoundEngine->play2D("../sound/explosion.wav", GL_FALSE);
		}
		//fwp.trails_num
		for (int i = 0; i < trails.size(); ++i) {
			if (trails[i]->isDied()) // 不再渲染死亡的粒子团
			{
				if (trails[i]->haveAnotherChance() == true)
				{
					trails[i]->cancelAnotherChance();

					auto chance = floatRandom(0.0f, 1.0f);

					if (chance > 0.9)
					{
						for (int j = 0; j < second_trails_num/5; ++j) {
							ptr p = std::make_shared<ParticleSystem>(50, false);
							// 初始化生成的节点，参数可以改
							Particle base;
							base.position = trails[i]->getHeadParticlePos();
							//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
							if (this->randomcolor)
							{
								base.color = glm::fvec4(ColorRandom(), 3.0f);
							}
							else
							{
								auto chance = floatRandom(0.0, 1.0);
								if (chance > 0.0)
								{
									base.color = glm::fvec4(ColorRandom(), 3.0f);
								}
								else
								{
									base.color = glm::fvec4(0.98f, 0.98f, 0.82f, 1.0f);
								}
							}
							this->explode_color = this->explode_color * (this->exist / (this->exist + 1.0f)) + base.color * (1 / (this->exist + 1.0f));
							this->exist += 1.0f;
							base.velocity = 0.15f * sphereRandom();
							base.size = 2.0f;
							base.life = floatRandom(0.0, 2.0);
							GenParam param;
							param.gen_rate = 150;
							param.size = 1;
							param.size_rate = 0.001;
							param.vel_base_rate = 1.0;
							param.vel_random_rate = 0.0000;
							param.delay = 1;
							param.life_rate[0] = 0.3;
							param.life_rate[1] = 0.4;
							p->initTrailGen(base, param);
							trails.push_back(p);
						}
					}
					
					//trails[i]->youCanDie();
				}
				trails[i] = this->dummy;
				this->exist -= 1.0f;
				/*trails[i] = trails[trails.size() - 1];
				trails.pop_back();*/
				//trails[i]->youCanDie();
			}
			else
			{
				trails[i]->trailGen(delta_time, 0.7, 0.07);
				trails[i]->draw(shader);
			}
		}
	}
}


/// <summary>
/// 生成烟花炸开时产生的流苏
/// </summary>
void bigfirework::genTrails() {
	explode_color = trails[0]->get_color();
	glm::fvec3 explode_pos = trails[0]->posTrail();
	explode_position = explode_pos;
	trails.resize(fwp.trails_num);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	for (int i = 0; i < fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10000, true);
		// 初始化生成的节点，参数可以改
		Particle base;
		base.position = explode_pos;
		//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
		if (this->randomcolor)
		{
			base.color = glm::fvec4(ColorRandom(), 3.0f);
		}
		else
		{
			base.color = glm::fvec4(0.98f, 0.98f, 0.82f, 1.0f);
		}
		
		
		this->explode_color = this->explode_color * (this->exist / (this->exist + 1.0f)) + base.color * (1 / (this->exist + 1.0f));
		this->exist += 1.0f;
		base.velocity = 0.15f * sphereRandom();
		base.size = 3.0f;
		base.life = floatRandom(0.0, 6.0);
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

bool bigfirework::isExploded() {
	return exploded;
}

glm::fvec3 bigfirework::get_explode_position() {
	return explode_position;
}

glm::fvec4 bigfirework::get_explode_color() {

	explode_color[0] = std::floor(explode_color[0] * 255.f) / 255;
	explode_color[1] = std::floor(explode_color[1] * 255.f) / 255;
	explode_color[2] = std::floor(explode_color[2] * 255.f) / 255;
	explode_color[3] = 3.0f;
	return explode_color;
}