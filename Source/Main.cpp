#include "raylib.h"
#include "raymath.h"
#include "../headers/bongfont.h"
#include "../headers/BongLogo.h"
#include "../headers/selectSound.h"
#include "../headers/scoreSound.h"
#include "../headers/hitSound.h"
#include <iostream>
#include <string>

using namespace std;

const Vector2 paddleSize = { 10, 100 };
const float speed = 400.0;
const int netNumber = 12;
int currentMode = 0;
bool shouldExit = false;
Sound selectSound, scoreSound, hitSound;

void DrawNet() {
	for (int i = 0; i < netNumber; i++) {
		DrawRectangle(395, 600 / (float)(netNumber - 1) * i - 5, 10, 10, {255, 255, 255, 100});
	}
}

void DrawScore(Vector2 score, Font font) {
	int mText = MeasureTextEx(font, to_string((int)score.x).c_str(), 36, 2).x;
	/* debug center text
	DrawRectangle(249, 40, 36, 36, {255, 255, 255, 100});
	DrawRectangle(515, 40, 36, 36, { 255, 255, 255, 100 });*/
	DrawTextEx(font, to_string((int)score.x).c_str(), { 267.0f - mText / 2.0f, 40 }, 36, 2, WHITE);
	mText = MeasureTextEx(font, to_string((int)score.y).c_str(), 36, 2).x;
	DrawTextEx(font, to_string((int)score.y).c_str(), { 533.0f - mText / 2.0f, 40 }, 36, 2, WHITE);
}

void AddPoint(Vector2 &score, Vector2 scoreToAdd) {
	score = { score.x + scoreToAdd.x, score.y + scoreToAdd.y };
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

		void setPos(Vector2 pos) {
			position = pos;
		}

		Vector2 GetPaddlePos() {
			return position;
		}
};

class Ball {
	private:
		int MAXSPEED = 250;
		Vector2 position;
		Vector2 speed;
		float radius;
	public:
		Ball(Vector2 startSpeed){
			radius = 8;
			MAXSPEED = 250;
			speed = Vector2Normalize(startSpeed) * MAXSPEED;
			position = { 400, 300};
		}
		
		void Draw() {
			DrawCircle(position.x, position.y, radius, WHITE);
		}

		void Move(Vector2 player, Vector2 enemy) {
			if ((position.y + radius >= 600 && speed.y >= 0) || (position.y - radius <= 0 && speed.y <= 0)) {
				speed.y = -speed.y;
				PlaySound(hitSound);
			}

			if (CheckCollisionCircleRec(position, radius, { player.x, player.y, paddleSize.x, paddleSize.y }) && speed.x <= 0) {
				speed.y = ((float)(position.y + radius - (player.y + paddleSize.y / 2)) / 50 * MAXSPEED / 2);
				speed.x = -speed.x;
				MAXSPEED += 25;
				speed = Vector2Normalize(speed) * MAXSPEED;
				PlaySound(hitSound);
			}
			if (CheckCollisionCircleRec(position, radius, { enemy.x, enemy.y, paddleSize.x, paddleSize.y }) && speed.x >= 0) {
				speed.y = ((float)(position.y + radius - (enemy.y + paddleSize.y / 2)) / 50 * MAXSPEED / 2);
				speed.x = -speed.x;
				MAXSPEED += 25;
				speed = Vector2Normalize(speed) * MAXSPEED;
				PlaySound(hitSound);
			}

			
			position =  Vector2{ position.x + speed.x * GetFrameTime(), position.y + speed.y * GetFrameTime()};
		}
		
		void ResetBall() {
			MAXSPEED = 250;
			speed = { 0, 0 };
			position = position = { 400, 300 };
		}

		void SetSpeed(Vector2 startSpeed) {
			MAXSPEED = 250;
			speed = Vector2Normalize(startSpeed) * MAXSPEED;
			position = { 400 - (float)radius / 2, 300 - (float)radius / 2 };
		}

		Vector2 GetBallPos() {
			return position;
		}

		Vector2 GetBallSpeed() {
			return speed;
		}

		int GetBallRadius() {
			return radius;
		}
};

int main() {

	SetTargetFPS(60);
	SetExitKey(KEY_NULL);
	InitWindow(800, 600, "Pong");
	InitAudioDevice();
	Font robotoFont = LoadFont_bongFont();
	Image logoImg;
	logoImg.data = BONGLOGO_DATA;
	logoImg.height = BONGLOGO_HEIGHT;
	logoImg.width = BONGLOGO_WIDTH;
	logoImg.format = BONGLOGO_FORMAT;
	logoImg.mipmaps = 1;
	Texture logoTex = LoadTextureFromImage(logoImg);
	Wave currentWave;
	currentWave.frameCount = SELECT_FRAME_COUNT;
	currentWave.sampleRate = SELECT_SAMPLE_RATE;
	currentWave.sampleSize = SELECT_SAMPLE_SIZE;
	currentWave.channels = SELECT_CHANNELS;
	currentWave.data = SELECT_DATA;
	selectSound = LoadSoundFromWave(currentWave);
	currentWave.frameCount = SCORE_FRAME_COUNT;
	currentWave.sampleRate = SCORE_SAMPLE_RATE;
	currentWave.sampleSize = SCORE_SAMPLE_SIZE;
	currentWave.channels = SCORE_CHANNELS;
	currentWave.data = SCORE_DATA;
	scoreSound = LoadSoundFromWave(currentWave);
	currentWave.frameCount = HIT_FRAME_COUNT;
	currentWave.sampleRate = HIT_SAMPLE_RATE;
	currentWave.sampleSize = HIT_SAMPLE_SIZE;
	currentWave.channels = HIT_CHANNELS;
	currentWave.data = HIT_DATA;
	hitSound = LoadSoundFromWave(currentWave);
	//Wave select = LoadWave("score.wav");
	//ExportWaveAsCode(select, "score");
	//Image logo = LoadImage("BongLogo.png");
	//ExportImageAsCode(logo, ((string)GetWorkingDirectory() + "\\BongLogo.h").c_str());
	/*
	cout << GetWorkingDirectory() << "\n";
	if (f.texture.id == 0) {
		cout << "Failed to load\n";
	}
	string exportPath = (string)GetWorkingDirectory() + "bongFont.h";
	cout << exportPath << '\n';
	cout << ExportFontAsCode(LoadFont("RobotoMono-Bold.ttf"), exportPath.c_str()) << '\n';*/
	//Vector2 playerPos = { 400, 300 };
	if (IsAudioDeviceReady) {
		SetMasterVolume(0.5f);
	}
	Vector2 score = { 0, 0 };
	float timeToStart = 3.0;
	int lastPoint = 0;
	Paddle player = Paddle({ 25 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
	Paddle enemy = Paddle({ 775 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
	Ball ball = Ball({ 200, 150 });
	int currentScene = 0; // 0 - menu, 1 - game
	
	while (!shouldExit) {


		
		//Logic
		switch (currentScene) {
		case 0:
			if (IsKeyPressed(KEY_W)) {
				currentMode = (currentMode + 1) % 2;
				PlaySound(selectSound);
			}
			if (IsKeyPressed(KEY_SPACE)) {
				currentScene = 1;
				PlaySound(selectSound);
				// restart game
			}
			if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
				shouldExit = true;
			}
			break;
		case 1:
			if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
				player.setPos({ 25 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
				enemy.setPos({ 775 - paddleSize.x / 2, 300 - paddleSize.y / 2 });
				ball.ResetBall();
				ball.SetSpeed({ 200, 150 });
				score = { 0, 0 };
				currentScene = 0;
			}
			int dir = 0;
			if (IsKeyDown(KEY_W)) {
				dir--;
			}
			if (IsKeyDown(KEY_S)) {
				dir++;
			}
			player.Move(dir * speed);
			
			if (currentMode == 0) {
				if (ball.GetBallPos().y > enemy.GetPaddlePos().y + paddleSize.y / 2) {
					enemy.Move(180);
				}
				if (ball.GetBallPos().y < enemy.GetPaddlePos().y + paddleSize.y / 2) {
					enemy.Move(-180);
				}
			}
			else {
				dir = 0;
				if (IsKeyDown(KEY_UP)) {
					dir--;
				}
				else if (IsKeyDown(KEY_DOWN)) {
					dir++;
				}
				enemy.Move(dir * speed);
			}
			ball.Move(player.GetPaddlePos(), enemy.GetPaddlePos());

			//ResetMatch on point
			if (ball.GetBallPos().x <= 0 || ball.GetBallPos().x + 2 * ball.GetBallRadius() >= 800) {
				if (ball.GetBallPos().x <= 400) {
					AddPoint(score, { 0, 1 });
					lastPoint = -1;
				}
				else {
					AddPoint(score, { 1, 0 });
					lastPoint = 1;
				}
				PlaySound(scoreSound);
				timeToStart = 3.0;
				ball.ResetBall();
			}

			if (Vector2Length(ball.GetBallSpeed()) == 0 && timeToStart <= 0) {
				ball.SetSpeed({ (float)lastPoint * 200, 150 });
			}
			else if (timeToStart >= 0) {
				timeToStart -= GetFrameTime();
			}
			break;
		}
		


		//Drawing
		BeginDrawing();
		ClearBackground(BLACK);

		switch (currentScene) {
		case 0:
			DrawTextureEx(logoTex, { 400.0f - logoTex.width * 4 / 2, floor(80 + pow((float)sin(1.5f*GetTime()), 2.0f) * 12 )}, 0, 4, WHITE);
			DrawTextEx(robotoFont, "Press space to play", { 400 - MeasureTextEx(robotoFont, "Press space to play", 36, 0).x/2, 360 }, 36, 0, WHITE);
			//DrawTextEx(robotoFont, "Press space to pleeeeey", { 400 - MeasureTextEx(robotoFont, "Press space to play", 36, 0).x / 2, 360 }, 36, 0, WHITE);
			DrawTextEx(robotoFont, ("Current Mode (w) : " + to_string(currentMode + 1) + "p").c_str(), {400 - MeasureTextEx(robotoFont, ("Current Mode (w) : " + to_string(currentMode + 1) + "p").c_str() , 28, 0).x / 2, 400}, 28, 0, WHITE);
			DrawTextEx(robotoFont, "Made by dutudev", { 400 - MeasureTextEx(robotoFont, "Made by dutudev", 16, 0).x / 2, 580 }, 16, 0, {255, 255, 255, 200});
			break;
			
		case 1:
			

			DrawNet();
			ball.Draw();
			player.Draw();
			enemy.Draw();
			DrawScore(score, robotoFont);
			break;
		}

		EndDrawing();

	}

	UnloadSound(selectSound);
	UnloadSound(hitSound);
	UnloadSound(scoreSound);
	CloseAudioDevice();
	UnloadTexture(logoTex);
	CloseWindow();

	return 0;
}