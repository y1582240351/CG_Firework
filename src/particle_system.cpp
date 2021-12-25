#include "particle_system.h"
#include "utility_tool.h"
#include "stb_image.h"

enum vbo_index { VERTEX, POS, COLOR };

float ParticleSystem::vertex[3] = { 0.0f, 0.0f, 0.0f };
int ParticleSystem::system_cnt = 0;
GLuint ParticleSystem::texture = -1;


ParticleSystem::~ParticleSystem() {
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);

	--system_cnt;
	if (system_cnt == 0) {
		deleteTexture();
	}
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

	++system_cnt;
	/*particles.resize(MAX_PARTICLE_NUM);*/
}


/// <summary>
/// 更新粒子的颜色参数
/// </summary>
/// <param name="color_data">颜色参数的值</param>
void ParticleSystem::updateColor(const float* color_data) {
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
void ParticleSystem::draw(Shader& shader) {
	if (texture != -1) {
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	glEnable(GL_PROGRAM_POINT_SIZE);
	shader.use();
	if (texture != -1)
		glBindTexture(GL_TEXTURE_2D, texture);
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
	particles.resize(MAX_PARTICLE_NUM);
	particles[0] = base_particle;
	float size_atten = 0.01f;
	for (int i = 1; i < max_particles_num; ++i) {
		particles[i].position = base_particle.position;
		particles[i].velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
		particles[i].life = floatRandom(10.0f, base_particle.life);
		particles[i].color = base_particle.color;
		particles[i].size = (1.0f - size_atten) * base_particle.size;
	}
}

void ParticleSystem::initTrailGen(Particle& base_particle, GenParam& param) {
	particles.push_back(base_particle);
	gen_param = param;
	pos = particles[0].position;
	vel = particles[0].velocity;
	count = 0;
}

/// <summary>
/// 让粒子按一定的轨迹运动
/// </summary>
/// <param name="delta_time">时间间隔</param>
/// <returns>当前存活粒子的个数</returns>
int ParticleSystem::trail(float delta_time) {
	//static bool showed[MAX_PARTICLE_NUM] = { 0 };
	particles_num = 0;
	glm::fvec3 a(0.0f, -0.02f, 0.0f); // 加速度
	float size_atten;
	//bool tmp = false;

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
			//else if (!showed[i] && !tmp) {
			//	//glm::fvec3 tmp = particles[i].position;
			//	particles[i].position = pre_pos;
			//	//pre_pos = tmp;
			//	showed[i] = true;
			//	tmp = true;
			//}
			else {
				glm::fvec3 tmp = particles[i].position;
				particles[i].position = pre_pos;
				pre_pos = tmp;
				/*showed[i] = true;
				tmp = true;*/
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
		else {
			//showed[i] = false;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}


int ParticleSystem::trailGen(float delta_time) {
	glm::fvec3 a(0.0f, -0.02f, 0.0f); // 加速度
	genParticles(delta_time);
	particles_num = 0;
	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i].life > 0.0f) {

			particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
			particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);

			particles[i].life -= delta_time;

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
		else {
			particles[i] = particles[particles.size() - 1];
			particles.pop_back();
			--i;
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
	particles.resize(MAX_PARTICLE_NUM);
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
	glm::fvec3 a(0.0f, 0.00f, 0.0f); // 加速度
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


/// <summary>
/// 为点精灵添加纹理
/// </summary>
/// <param name="file_name">纹理的文件路径</param>
void ParticleSystem::setTexture(const char* file_name) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}



/// <summary>
/// 删除纹理
/// </summary>
void ParticleSystem::deleteTexture() {
	glDeleteTextures(1, &texture);
}


/// <summary>
/// 生成粒子
/// </summary>
void ParticleSystem::genParticles(float delta_time) {
	++count;
	if (count % gen_param.delay != 0) {
		return;
	}
	int n = delta_time * gen_param.gen_rate; // 设置生成粒子的个数
	for (int i = 0; i < n; ++i) {
		if (particles.size() >= max_particles_num)
			break;
		Particle p;
		float r = particles[0].size * gen_param.size_rate;
		p.color = particles[0].color;
		//p.velocity = particles[0].velocity * floatRandom(0.85, 0.90);
		// p.velocity = particles[0].velocity - glm::fvec3(0.0f, r / delta_time, 0.0f) * floatRandom(0.085, 0.090);
		float tmp = normalRandom();
		p.velocity = vel * gen_param.vel_base_rate * tmp + vel * gen_param.vel_random_rate * sphereRandom();
		p.position = posRandom(pos, r); // (0.01f * delta_time * particles[0].velocity).length()
		p.life = particles[0].life * floatRandom(gen_param.life_rate[1], gen_param.life_rate[1]);
		//p.life = particles[0].life * tmp;
		p.size = gen_param.size;
		particles.push_back(p);
	}
	pos = particles[0].position;
	vel = particles[0].velocity;
}