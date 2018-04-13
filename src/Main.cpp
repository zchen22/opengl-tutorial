#include "HelloWindow.h"
#include "HelloTriangle.h"
#include "HelloTexture.h"
#include "HelloTransformations.h"
#include "HelloCoordSystems.h"

int main() {
	HelloWindow win;
	win.Main();
	HelloTriangle tri;
	tri.Main();
	HelloTexture tex;
	tex.Main();
	HelloTransformations trans;
	trans.Main();
	HelloCoordSystems coords(800, 600);
	coords.Main();
	return 0;
}