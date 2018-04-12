#include "HelloWindow.h"
#include "HelloTriangle.h"
#include "HelloTexture.h"

int main() {
	HelloWindow win;
	win.Main();
	HelloTriangle tri;
	tri.Main();
	HelloTexture tex;
	tex.Main();
	return 0;
}