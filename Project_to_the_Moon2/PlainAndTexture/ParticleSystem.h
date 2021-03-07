#pragma once
#include "NewParticle.h"

//class for particle system
class ParticleSystem {

public:
	Particle* particle_head;

	//methods for particle system
	void add();
	void update(float dt);
	void remove();
};