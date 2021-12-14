#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#define MAX_PARTICLE_NUM 10000

#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

struct Particle
{
    glm::fvec3 position; // �����ӵ�offset����ʾ���ӵ�λ��
    glm::fvec3 velocity;
    glm::fvec4 color;
    float size;
    float life;

    Particle()
        : position(glm::fvec3(0.0f)), velocity(glm::fvec3(0.0f)), color(glm::fvec4(0.0f)), life(-1.0f), size(0.0f) { }
};

class ParticleSystem
{
public:
    ParticleSystem() {};

    ParticleSystem(int max_particle);

    ~ParticleSystem();

    void initTrail(Particle& base_particle);

    void initExplode(Particle& base_particle, float v_abs);

    void updatePos(const float*);

    void updateColor(const float*);

    void draw(Shader&);

    int trail(float delta_time);

    int explode(float delta_time);

    bool isDied();

    glm::fvec3 posTrail();

    static void setTexture(const char* file_name);
    static void deleteTexture();

private:
    // ʹ�����ĸ�VBO��
    // ���е�0�������������ӵĶ������꣬��1�������������ӵ�����λ��offset����2�������������ӵ���ɫ��Ϣ����3���������ӵĴ�С
    GLuint vbo[3], vao;
    Particle particles[MAX_PARTICLE_NUM];
    int particles_num, max_particles_num;
    int layer; // ��β�ĳ��ȣ������Ӳ�������ʾ

    float pos_size_data[MAX_PARTICLE_NUM * 4];
    float color_data[MAX_PARTICLE_NUM * 4];
    static float vertex[3];
    // �㾫��
    static GLuint texture;
    static int system_cnt;
};

#endif