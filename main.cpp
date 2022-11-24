#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <raylib.h>

const int map[10][10] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

const int TileSize = 40;


bool IsHit(const int map[10][10], Vector2 point, float size) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (j < point.x + size &&
				j + size > point.x &&
				i < point.y + size &&
				i + size > point.y && map[i][j] == 1) {
				return true;
			}
		}
	}

	return false;
}

void StepRay(const Vector2 position, Vector2 forward, const int stepCount, const int stepLength, int* incr, Color color, Vector2* pHit) {

	Vector2 start = Vector2{
		position.x,
		position.y
	};

	Vector2 end = Vector2{
		(position.x + (forward.x / stepLength)),
		(position.y + (forward.y / stepLength))
	};

	pHit->x = end.x;
	pHit->y = end.y;

	DrawLine(start.x * TileSize + TileSize / 2, start.y * TileSize + TileSize / 2, end.x * TileSize + TileSize / 2, end.y * TileSize + TileSize / 2, GRAY);

	if (!IsHit(map, end, 0.5) && *incr < stepCount) {
		*incr += 1;
		StepRay(end, forward, stepCount, stepLength, incr, color, pHit);
	}
	else {
		*incr = 0;
	}
}

float Dist(const Vector2 v1, const Vector2 v2) {
	return powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2);
}

void Render3DMap(Vector2 cameraPosition, float cameraRotation, int lineThickness, int fov) {
	for (int i = -fov / 2; i < fov / 2; i++) {
		
		int c = 0;
		
		Vector2 hit;

		Vector2 dir = Vector2{ (float)sin((cameraRotation + i) * (PI / 180)), (float)cos((cameraRotation + i) * (PI / 180)) };

		StepRay(cameraPosition, dir, 1000, 100, &c, GRAY, &hit);

	}
}

void Render2DMap(const int map[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Color color = BLACK;
			if (map[i][j] == 1) {
				color = BROWN;
			}

			DrawRectangle(j * TileSize, i * TileSize, TileSize, TileSize, color);
		}
	}
}

void Render2DPlayer(const Vector2 position) {
	DrawCircle(position.x * TileSize + TileSize / 2, position.y * TileSize + TileSize / 2, 6, ORANGE);
}

Vector2 Update2DPlayer(Vector2* position, int* rotation) {
	Vector2 forward = Vector2{ (float)sin(*rotation * (PI / 180)), (float)cos(*rotation * (PI / 180)) };

	Vector2 velocity = Vector2{ 0, 0 };

	if (IsKeyDown(KEY_W)) {
		velocity = Vector2{
			0.01f * forward.x,
			0.01f * forward.y
		};
	}

	if (IsKeyDown(KEY_S)) {
		velocity = Vector2{
			-0.01f * forward.x,
			-0.01f * forward.y
		};
	}

	if (IsKeyDown(KEY_Q)) {
		(*rotation)--;
	}

	if (IsKeyDown(KEY_E)) {
		(*rotation)++;
	}

	if (!IsHit(map, Vector2{ position->x + velocity.x, position->y + velocity.y }, 0.5)) {
		position->x += velocity.x;
		position->y += velocity.y;
	}
	else {

	}

	return forward;
}



struct Player
{
	float x;
	float y;
	int radius;
};


void main()
{
	const int windowWidth = 800;
	const int windowHeight = 450;

	InitWindow(windowWidth, windowHeight, "Algebra EJ4");

	Player player = { GetScreenWidth() / 2 , GetScreenHeight() / 2 ,  10 };

	Vector2 playerPosition = Vector2{ 1, 1 };
	Vector2 playerForward = {};
	int playerRotation = 0;

	int lineWidth = 10;

	Vector2 mousePosition;
	float vectorModule;
	Vector2 normalizeDirect;

	double lastTime = GetTime();
	while (!WindowShouldClose())
	{
		double currentTime = GetTime();
		mousePosition = GetMousePosition();

		Vector2 vectorDirection = { mousePosition.x - GetScreenWidth() / 2,  mousePosition.y - GetScreenHeight() / 2 };
		float arcTan = atan2(vectorDirection.y, vectorDirection.x);
		float angle = arcTan * 180 / PI;

		vectorModule = static_cast<float>(sqrt(pow(vectorDirection.x, 2) + pow(vectorDirection.y, 2)));
		normalizeDirect = { vectorDirection.x / vectorModule, vectorDirection.y / vectorModule };

		if (currentTime - lastTime > 1.0f / 150.0f)
		{
			lastTime = currentTime;
			playerForward = Update2DPlayer(&playerPosition, &playerRotation);
		}

		BeginDrawing();
		ClearBackground(BLACK);
		Render2DMap(map);
		Render2DPlayer(playerPosition);
		Render3DMap(playerPosition, playerRotation, lineWidth, 60);


		EndDrawing();

	}
	CloseWindow();

}