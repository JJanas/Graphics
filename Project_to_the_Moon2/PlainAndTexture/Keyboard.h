#pragma once
#include <iostream>

class Keyboard {

	//public methods for getting the keyboard input
public:

	//generic constructor and destructor
	Keyboard();
	~Keyboard();

	//main keyboard method
	void readKeyboard(unsigned char key, int x, int y, bool press);

	//return the keys array
	unsigned char* getKeys();

private:
	unsigned char keys[256];
};