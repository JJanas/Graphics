#include "Keyboard.h"

Keyboard::Keyboard() {
	//generic constructor: do nothing
}

Keyboard::~Keyboard() {
	//generic destructor
}

//read the input from the players
void Keyboard::readKeyboard(unsigned char key, int x, int y, bool press) {

	if (key >= 'A' && key <= 'Z') key += 32;
	keys[key] = press;
}

//return the keys array
unsigned char* Keyboard::getKeys() {
	return this->keys;
}