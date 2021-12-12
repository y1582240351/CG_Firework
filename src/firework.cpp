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
/// ��ʼ���̻�����
/// </summary>
/// <param name="base_fwp">�̻���ʼ����</param>
void Firework::init(fireworkParam base_fwp) {
	fwp = base_fwp;
	// ...����Ҫ�õ��Ĳ���
}


/// <summary>
/// ȼ���̻�ʱ�ĸ��º���
/// </summary>
/// <param name="shader">��Ⱦ���õ���ɫ��</param>
/// <param name="delta_time">ÿ֮֡��ļ��ʱ��</param>
void Firework::light(Shader &shader, float delta_time) {
	time_cnt += delta_time;
	if (time_cnt < explode_time) {
		trails[0]->trail(delta_time);
		trails[0]->draw(shader);
	}
	else {
		if (!exploded) { // �̻���һ�α�ը
			exploded = true;
			genTrails();
		}
		for (int i = 0; i < fwp.trails_num; ++i) {
			if (trails[i]->isDied()) // ������Ⱦ������������
				continue;
			trails[i]->trail(delta_time);
			trails[i]->draw(shader);
		}
	}
}


/// <summary>
/// �����̻�ը��ʱ����������
/// </summary>
void Firework::genTrails() {
	glm::fvec3 explode_pos = trails[0]->posTrail();
	trails.resize(fwp.trails_num);
	for (int i = 0; i < fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10);
		// ��ʼ�����ɵĽڵ㣬�������Ը�
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