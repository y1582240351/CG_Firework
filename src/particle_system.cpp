#include "particle_system.h"
#include "utility_tool.h"

enum vbo_index{VERTEX, POS, COLOR};

float ParticleSystem::vertex[3] = { 0.0f, 0.0f, 0.0f };

ParticleSystem::~ParticleSystem() {
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
}

/// <summary>
/// ����ϵͳ���캯��
/// </summary>
/// <param name="max_particle"> ���������� </param>
ParticleSystem::ParticleSystem(int max_particle = MAX_PARTICLE_NUM) : max_particles_num(max_particle) {
	glGenBuffers(3, vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// ��ʼ���������ӵ�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(VERTEX);

	// ��ʼ�����ӵ�����λ���Լ���С(����ά)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(POS, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(POS);

	// ��ʼ�����ӵ���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(COLOR);
}


/// <summary>
/// �������ӵ���ɫ����
/// </summary>
/// <param name="color_data">��ɫ������ֵ</param>
void ParticleSystem::updateColor(const float * color_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, color_data);
}



/// <summary>
/// �������ӵ�λ���Լ���С��Ϣ
/// </summary>
/// <param name="pos_data">λ���Լ���С������ֵ</param>
void ParticleSystem::updatePos(const float* pos_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, pos_data);
}


/// <summary>
/// ÿһ֡��Ⱦ
/// </summary>
/// <param name="shader">��Ⱦ���õ���ɫ������</param>
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
/// ��ʼ������ϵͳ�е�����
/// </summary>
/// <param name="base_particle">��ʼ�����õı�׼��������</param>
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
/// �����Ӱ�һ���Ĺ켣�˶�
/// </summary>
/// <param name="delta_time">ʱ����</param>
/// <returns>��ǰ������ӵĸ���</returns>
int ParticleSystem::trail(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, 0.05f, 0.0f); // ���ٶ�
	float size_atten;
	
	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {
			if (i == 0) {
				// ������ͷ���ӵ�λ�����ٶ�
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

			// ̫С������ֱ����ʧ
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
/// ��ʼ��Ҫ��ը��������
/// </summary>
/// <param name="base_particle">ÿ�����ӵĻ������ԣ�ע��������ٶȲ�û��ʹ��</param>
/// <param name="v_abs">Ҫָ�����ٶȴ�С</param>
void ParticleSystem::initExplode(Particle& base_particle, float v_abs) {
	for (int i = 0; i < max_particles_num; ++i) {
		particles[i] = base_particle;
		particles[i].velocity = v_abs * sphereRandom();
	}
}


/// <summary>
/// ���ӱ�ը
/// </summary>
/// <param name="delta_time">ʱ����</param>
/// <returns>������ӵĸ���</returns>
int ParticleSystem::explode(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, -0.0f, 0.0f); // ���ٶ�
	float size_atten;

	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {

			particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
			particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);

			// ̫С������ֱ����ʧ
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
/// �������Ƿ��Ѿ�����
/// </summary>
/// <returns>true: ��������</returns>
bool ParticleSystem::isDied() {
	// ����ͷ�������������������ž�����
	return particles[0].life <= 0.0f;
}



/// <summary>
/// ������ͷ���ӵ�λ��
/// </summary>
/// <returns>λ��</returns>
glm::fvec3 ParticleSystem::posTrail() {
	return particles[0].position;
}


