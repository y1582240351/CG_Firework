#ifndef INNERBURSTFIREWORK_H
#define INNERBURSTFIREWORK_H

#include "firework.h"

class innerburstfirework:public Firework 
{
private:
	void genTrails();
public:
	innerburstfirework();

	innerburstfirework(float);

	~innerburstfirework();

	bool isAlive();

	void destroy();

	void init(fireworkParam fwp);

	void light(Shader& shder, float delta_time);

private:
	std::vector<ptr> trails;	// ������β�������ţ���0�����ͷ�ʱ��������
	std::vector<ptr> explosions; // ������ըЧ����������
	float time_cnt, explode_time; // ��ʱ���Լ�Ҫ��ը��ʱ��
	fireworkParam fwp; // �̻��Ĳ���
	bool explodedOnce, explodedTwice;
	glm::fvec3 explodePosition;
	bool canExplodeTwice;
};


#endif