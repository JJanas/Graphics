#pragma once

//structure for Particle
class Particle {

public:
	float position[3];
	float direction[3];
	float life;
	Particle* next;
	Particle();
};