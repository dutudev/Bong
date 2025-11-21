#include "raylib.h"
#include <iostream>

using namespace std;

const Vector2 paddleSize = { 10, 100 };
const float speed = 400.0;
const int netNumber = 12;
bool ok = true;



void DrawNet() {
	for (int i = 0; i < netNumber; i++) {
		DrawRectangle(390, 600 / (float)(netNumber - 1) * i - 5, 10, 10, {255, 255, 255, 100});
	}
}

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

		
		void Move(float speed) {
			if (!(position.y <= 0 && speed <= 0) && !(position.y >= 500 && speed >= 0)) {
				position.y += speed * GetFrameTime();
			}
			paddleRect = { position.x, position.y, paddleSize.x, paddleSize.y };
		}

		void Draw(){
			DrawRectangle(position.x, position.y, paddleSize.x, paddleSize.y, WHITE);
		}

		Vector2 GetPaddlePos() {
			return position;
		}
};

class Ball {
	private:
		Vector2 position;
		Vector2 speed;
		float radius;
	public:
		Ball(Vector2 startSpeed){
			radius = 8;
			speed = startSpeed;
			position = { 400 - (float)radius/2, 300 - (float)radius/2};
		}
		
		void Draw() {
			DrawCircle(position.x, position.y, radius, WHITE);
		}

		void Move(Vector2 player, Vector2 enemy) {
			if ((position.y + radius >= 600 && speed.y >= 0) || (position.y - radius <= 0 && speed.y <= 0)) {
				speed.y = -speed.y;
			}

			if (CheckCollisionCircleRec(position, radius, { player.x, player.y, paddleSize.x, paddleSize.y }) && speed.x <= 0) {
				speed.x = -speed.x;
			}
			if (CheckCollisionCircleRec(position, radius, { enemy.x, enemy.y, paddleSize.x, paddleSize.y }) && speed.x >= 0) {
				speed.x = -speed.x;
			}
			position =  Vector2{ position.x + speed.x * GetFrameTime(), position.y + speed.y * GetFrameTime()};
		}

		Vector2 GetBallPos() {
			return position;
		}
};

int main() {

	SetTargetFPS(60);
	InitWindow(800, 600, "Pong");
	//Vector2 playerPos = { 400, 300 };
	Vector2 score = { 0, 0 };
	Paddle player = Paddle({ 25 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
	Paddle enemy = Paddle({ 775 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
	Ball ball = Ball({ 200, 150 });
	while (!WindowShouldClose()) {


		//Logic
		int dir = 0;
		if (IsKeyDown(KEY_W)) {
			dir--;
		}
		if (IsKeyDown(KEY_S)) {
			dir++;
		}
		player.Move(dir * speed);

		if (ball.GetBallPos().y > enemy.GetPaddlePos().y + paddleSize.y / 2) {
			enemy.Move(180);
		}
		if (ball.GetBallPos().y < enemy.GetPaddlePos().y + paddleSize.y / 2) {
			enemy.Move(-180);
		}

		ball.Move(player.GetPaddlePos(), enemy.GetPaddlePos());

		//Drawing
		BeginDrawing();
		ClearBackground(BLACK);

		DrawNet();
		ball.Draw();
		player.Draw();
		enemy.Draw();

		EndDrawing();

	}

	CloseWindow();

	return 0;
}