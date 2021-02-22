#include "mendelbrotset.h"

const char* path = "set.ppm";

int main() {
	set mendelbrotset;
	mendelbrotset.init(1000,1000,path);
	mendelbrotset.setparameters();
	mendelbrotset.draw();
	return 0;
}
