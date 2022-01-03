#include "utility_tool.h"
#include <random>
#include <chrono>
#include <iostream>

glm::fvec3 move(glm::fvec3 init_pos, glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time) {
	// delta_s = vt + 0.5*a*t^2, s = s_init+ delta_s
	// return init_pos + velocity * delta_time + acceleration * delta_time * delta_time * 0.5f;
	return init_pos + velocity * delta_time;
}


glm::fvec3 velocityUpdate(glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time) {
	return velocity + acceleration * delta_time;
}


glm::fvec3 fvec3Random(float min, float max) {
	srand((unsigned)time(NULL));
	float r1 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	float r2 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	float r3 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	return glm::fvec3(r1, r2, r3);
}

int intRandom(int min, int max) {
	return min + rand() / (RAND_MAX / (max - min));
}

float floatRandom(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}


float timer() {
	static float last_frame = 0.0f;
	float current_frame = glfwGetTime();
	float ret = current_frame - last_frame;
	last_frame = current_frame;
	return ret;
}


glm::fvec3 sphereRandom() {
	float phi = floatRandom(0.0f, 2 * PI);
	//float costheta = floatRandom(-1, 1);
	//float theta = acos(costheta);
	float theta = floatRandom(0.0f, 2 * PI);
	return glm::fvec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}



glm::fvec3 posRandom(glm::fvec3& origin_pos, float radius) {
	glm::fvec3 pos = origin_pos;
	float r = floatRandom(0, 1) * radius;
	pos += r * sphereRandom();
	return pos;
}

float normalRandom(float mu, float sigma) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<float> distribution(mu, sigma);
	return distribution(generator);
}

glm::fvec3 ColorRandom()
{
	return glm::fvec3(intRandom(0, 255) * 1.0f / 255,
		intRandom(0, 255) * 1.0f / 255,
		intRandom(0, 255) * 1.0f / 255);
<<<<<<< HEAD
}
=======
}
>>>>>>> 9a62f39ad558c26963c51de33a9e7ad3d10adf8b
