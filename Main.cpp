#include "raylib.h"
#include <iostream>

using namespace std;

const Vector2 paddleSize = { 10, 100 };

//make classes here, maybe move them to a separate folder later
class Paddle {
	private:
		Vector2 position;
		Rectangle paddleRect;
	public:
		Paddle(Vector2 startPosition) {
			position = startPosition;
			paddleRect = { position.x, position.y, paddleSize.x, paddleSize.y };
		}

		/*
		void Move(float speed) {

		}*/

		void Draw(){
			DrawRectangle(position.x, position.y, paddleSize.x, paddleSize.y, WHITE);
		}
};

int main() {

	InitWindow(800, 600, "Pong");
	//Vector2 playerPos = { 400, 300 };
	Paddle player = Paddle({ 25 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
	Paddle enemy = Paddle({ 775 - paddleSize.x / 2, 300 - paddleSize.y / 2 });

	while (!WindowShouldClose()) {


		//Logic



		//Drawing
		BeginDrawing();
		ClearBackground(BLACK);

		player.Draw();
		enemy.Draw();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}