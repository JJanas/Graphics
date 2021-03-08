#include "NewParticle.h"
#include <stdlib.h>

Particle::Particle() {
	position[0] = 50.0f;
	position[2] = 25.0f;
	position[1] = 2.0f;
	direction[0] = (10000 - rand() % 20000) / 10000.0f;
	direction[1] = (10000 - rand() % 20000) / 10000.0f;
	direction[2] = (10000 - rand() % 20000) / 10000.0f;
	life = rand() % 10000 / 10000.0f;
}