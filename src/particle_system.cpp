#include "particle_system.h"
#include "utility_tool.h"

enum vbo_index{VERTEX, POS, COLOR};

float ParticleSystem::vertex[3] = { 0.0f, 0.0f, 0.0f };

ParticleSystem::~ParticleSystem() {
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
}

/// <summary>
/// 粒子系统构造函数
/// </summary>
/// <param name="max_particle"> 最大的粒子数 </param>
ParticleSystem::ParticleSystem(int max_particle = MAX_PARTICLE_NUM) : max_particles_num(max_particle) {
	glGenBuffers(3, vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// 初始化生成粒子的位置
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]);
	// 初始化为空数组
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(VERTEX);

	// 初始化粒子的中心位置以及大小(第四维)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	// 初始化为空数组
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(POS, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(POS);

	// 初始化粒子的颜色
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	// 初始化为空数组
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(COLOR);
}


/// <summary>
/// 更新粒子的颜色参数
/// </summary>
/// <param name="color_data">颜色参数的值</param>
void ParticleSystem::updateColor(const float * color_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, color_data);
}



/// <summary>
/// 更新粒子的位置以及大小信息
/// </summary>
/// <param name="pos_data">位置以及大小参数的值</param>
void ParticleSystem::updatePos(const float* pos_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, pos_data);
}


/// <summary>
/// 每一帧渲染
/// </summary>
/// <param name="shader">渲染所用的着色器程序</param>
void ParticleSystem::draw(Shader &shader) {
	shader.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribDivisor(VERTEX, 0);
	glVertexAttribDivisor(POS, 1);
	glVertexAttribDivisor(COLOR, 1);
	//glVertexAttribDivisor(SIZE, 1);
	glDrawArraysInstanced(GL_POINTS, 0, 1, particles_num); 

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


/// <summary>
/// 初始化粒子系统中的粒子
/// </summary>
/// <param name="base_particle">初始化所用的标准参照粒子</param>
void ParticleSystem::initTrail(Particle& base_particle) {
	particles[0] = base_particle;
	float size_atten = 0.01f;
	for (int i = 1; i < max_particles_num; ++i) {
		particles[i].position = base_particle.position;
		particles[i].velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
		particles[i].life = floatRandom(1.0f, base_particle.life);
		particles[i].color = base_particle.color;
		particles[i].size = (1.0f - size_atten) * base_particle.size;
	}
}


/// <summary>
/// 让粒子按一定的轨迹运动
/// </summary>
/// <param name="delta_time">时间间隔</param>
/// <returns>当前存活粒子的个数</returns>
int ParticleSystem::trail(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, 0.05f, 0.0f); // 加速度
	float size_atten;
	
	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {
			if (i == 0) {
				// 更新领头粒子的位置与速度
				pre_pos = particles[i].position;
				particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
				particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);
				/*particles[i].size *= 0.999f;*/
			}
			else {
				glm::fvec3 tmp = particles[i].position;
				particles[i].position = pre_pos;
				pre_pos = tmp;
			}
			
			/*size_atten = floatRandom(0.001 * static_cast<float>(i), 0.01 * static_cast<float>(i));
			particles[i].size *= (1 - size_atten);*/

			particles[i].life -= delta_time;

			// 太小的粒子直接消失
			if (particles[i].size < 0.001) {
				particles[i].life = -1.0f;
				continue;
			}

			pos_size_data[4 * particles_num + 0] = particles[i].position.x;
			pos_size_data[4 * particles_num + 1] = particles[i].position.y;
			pos_size_data[4 * particles_num + 2] = particles[i].position.z;
			pos_size_data[4 * particles_num + 3] = particles[i].size;

			color_data[4 * particles_num + 0] = particles[i].color.r;
			color_data[4 * particles_num + 1] = particles[i].color.g;
			color_data[4 * particles_num + 2] = particles[i].color.b;
			color_data[4 * particles_num + 3] = particles[i].color.a;

			particles_num++;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}


/// <summary>
/// 初始化要爆炸的粒子团
/// </summary>
/// <param name="base_particle">每个粒子的基本属性，注意这里的速度并没有使用</param>
/// <param name="v_abs">要指定的速度大小</param>
void ParticleSystem::initExplode(Particle& base_particle, float v_abs) {
	for (int i = 0; i < max_particles_num; ++i) {
		particles[i] = base_particle;
		particles[i].velocity = v_abs * sphereRandom();
	}
}


/// <summary>
/// 粒子爆炸
/// </summary>
/// <param name="delta_time">时间间隔</param>
/// <returns>存活粒子的个数</returns>
int ParticleSystem::explode(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, -0.0f, 0.0f); // 加速度
	float size_atten;

	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {

			particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
			particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);

			// 太小的粒子直接消失
			if (particles[i].size < 0.01) {
				particles[i].life = -1.0f;
				continue;
			}

			pos_size_data[4 * particles_num + 0] = particles[i].position.x;
			pos_size_data[4 * particles_num + 1] = particles[i].position.y;
			pos_size_data[4 * particles_num + 2] = particles[i].position.z;
			pos_size_data[4 * particles_num + 3] = particles[i].size;

			color_data[4 * particles_num + 0] = particles[i].color.r;
			color_data[4 * particles_num + 1] = particles[i].color.g;
			color_data[4 * particles_num + 2] = particles[i].color.b;
			color_data[4 * particles_num + 3] = particles[i].color.a;

			particles_num++;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}



/// <summary>
/// 粒子团是否已经死亡
/// </summary>
/// <returns>true: 粒子死亡</returns>
bool ParticleSystem::isDied() {
	// 若领头粒子死亡，整个粒子团就死亡
	return particles[0].life <= 0.0f;
}



/// <summary>
/// 返回领头粒子的位置
/// </summary>
/// <returns>位置</returns>
glm::fvec3 ParticleSystem::posTrail() {
	return particles[0].position;
}


