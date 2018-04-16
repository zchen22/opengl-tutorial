#include <iostream>
#include <unordered_map>

#include "HelloWindow.h"
#include "HelloTriangle.h"
#include "HelloTexture.h"
#include "HelloTransformations.h"
#include "HelloCoordSystems.h"
#include "HelloCamera.h"

int PrintMenu(const std::unordered_map<int, std::string>& samples) {
	printf("\nMenu\n");
	for (const auto& kv : samples) {
		printf("\t%d\t%s\n", kv.first, kv.second.c_str());
	}
	return 0;
}

int main() {
	std::unordered_map<int, std::string> samples({{0, "Exit"},  {1, "HelloWindow"}, {2, "HelloWindow"}, {3, "HelloTexture"}, {4, "HelloTransformations"}, {5, "HelloCoordSystems",}, {6, "HelloCamera"} });
	int sampleId = 0;
	do {
		PrintMenu(samples);
		printf("\nEnter a sample ID: ");
		scanf("%d", &sampleId);
		if (sampleId < 0 || sampleId > 6) {
			printf("Invalid sample ID (%d)\n", sampleId);
			continue;
		}
		switch (sampleId) {
		case 1: {
			HelloWindow win;
			win.Main();
			break;
		}
		case 2: {
			HelloTriangle tri;
			tri.Main();
			break;
		}
		case 3: {
			HelloTexture tex;
			tex.Main();
			break;
		}
		case 4: {
			HelloTransformations trans;
			trans.Main();
			break;
		}
		case 5: {
			HelloCoordSystems coords(800, 600);
			coords.Main();
			break;
		}
		case 6: {
			HelloCamera camera(800, 600);
			camera.Main();
			break;
		}
		}
	} while (sampleId != 0);
	return 0;
}
