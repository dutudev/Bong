#include "raylib.h"

using namespace std;

int main() {

	InitWindow(800, 600, "Pong");

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();

	}

	CloseWindow();

	return 0;
}