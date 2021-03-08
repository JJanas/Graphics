#include "ParticleSystem.h"

// add
void ParticleSystem::add() {
	Particle* p = new Particle();
	p->next = particle_head;
	particle_head = p;
}

// update
void ParticleSystem::update(float dt) {
	Particle* p = particle_head;
	while (p) {
		// decrease lifespan
		p->life -= dt;
		// apply gravity
		p->direction[0] += 9.81f * dt;
		// modify position
		p->position[0] += dt * p->direction[0];
		p->position[1] += dt * p->direction[1];
		p->position[2] += dt * p->direction[2];
		// goto next particle
		p = p->next;
	}
}

// remove
void ParticleSystem::remove() {
	Particle* curr = particle_head;
	Particle* prev = 0;
	while (curr) {
		if (curr->life < 0) {
			if (prev) {
				prev->next = curr->next;
			}
			else {
				particle_head = curr->next;
			}
			Particle* temp = curr;
			curr = curr->next;
			delete temp;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
}