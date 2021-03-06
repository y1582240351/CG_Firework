//#ifndef BIGFIREWORK_H
//#define BIGFIREWORK_H
//
//#include "firework.h"
//
//class bigfirework:public Firework 
//{
//private:
//	void genTrails();
//public:
//	bigfirework();
//
//	bigfirework(float);
//
//	~bigfirework();
//
//	bool isAlive();
//
//	void destroy();
//
//	void init(fireworkParam fwp);
//
//	void light(Shader& shder, float delta_time);
//
//private:
//	std::vector<ptr> trails;	// 产生拖尾的粒子团，第0个是释放时的主粒子
//	std::vector<ptr> explosions; // 产生爆炸效果的粒子团
//	float time_cnt, explode_time; // 计时器以及要爆炸的时间
//	fireworkParam fwp; // 烟花的参数
//	bool exploded;
//	bool canExplodeTwice;
//};
//
//
//#endif