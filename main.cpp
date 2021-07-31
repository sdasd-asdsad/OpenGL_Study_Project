#include "EngineCore.h"

int main(int argc, char** argv) {
	EngineCore* core;
	core = new EngineCore();

	core->run(&argc, argv);

	return 0;
}