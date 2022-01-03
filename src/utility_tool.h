#pragma once
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265358979323846

/// <summary>
/// 计算物体在delta_time内作匀加速运动后的位置
/// </summary>
/// <param name="init_pos">物体的初始位置</param>
/// <param name="velocity">物体的初始速度</param>
/// <param name="acceleration">物体所受的加速度</param>
/// <param name="delta_time">运动的持续时间</param>
/// <returns>物体最终的位置</returns>
glm::fvec3 move(glm::fvec3 init_pos, glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time);


/// <summary>
/// 计算物体在delta_time内作匀加速运动后的速度
/// </summary>
/// <param name="velocity">物体初始速度</param>
/// <param name="acceleration">物体加速度</param>
/// <param name="delta_time">运动持续时间</param>
/// <returns>物体最终速度</returns>
glm::fvec3 velocityUpdate(glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time);


/// <summary>
/// 返回一个随机三维向量
/// </summary>
/// <param name="min">下界</param>
/// <param name="max">上界</param>
/// <returns>随机三维向量</returns>
glm::fvec3 fvec3Random(float min, float max);


/// <summary>
/// 返回一个随机浮点数
/// </summary>
/// <param name="min">下界</param>
/// <param name="max">上界</param>
/// <returns>随机浮点数</returns>
float floatRandom(float min, float max);


/// <summary>
/// 计时器，用来计算渲染的两帧之间的delta_time
/// </summary>
/// <returns>相邻两帧之间的间隔时间</returns>
float timer();


/// <summary>
/// 返回指向单位圆圆面上随机一点的单位向量
/// </summary>
/// <returns>随机方向的单位向量</returns>
glm::fvec3 sphereRandom();


/// <summary>
/// 在以origin_pos为球心，radius为半径进行随机采样
/// </summary>
/// <param name="origin_pos"></param>
/// <param name="radius"></param>
/// <returns></returns>
glm::fvec3 posRandom(glm::fvec3& origin_pos, float radius);


/// <summary>
/// 返回服从高斯分布的随机数 (0.5,1)
/// </summary>
/// <returns></returns>
float normalRandom(float mu, float sigma);

/// <summary>
/// 返回服从高斯分布的随机数 (0.0, 1.0)
/// </summary>
/// <returns></returns>
glm::fvec3 ColorRandom();

#endif