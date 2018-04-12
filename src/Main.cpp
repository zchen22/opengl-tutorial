#include "HelloWindow.h"
#include "HelloTriangle.h"
#include "HelloTexture.h"
#include "HelloTransformations.h"

int main() {
	HelloWindow win;
	win.Main();
	HelloTriangle tri;
	tri.Main();
	HelloTexture tex;
	tex.Main();
	HelloTransformations trans;
	trans.Main();
	return 0;
}