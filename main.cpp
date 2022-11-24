#include <math.h>

#include <raylib.h>



const int map[10][20] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

const int TileSize = 40;


bool IsHit(const int map[10][20], Vector2 point, float size)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (j < point.x + size &&
				j + size > point.x &&
				i < point.y + size &&
				i + size > point.y && map[i][j] == 1)
			{
				return true;
			}
		}
	}

	return false;
}

//Usando recursividad se logra dibujar los ray
void StepRay(const Vector2 position, Vector2 forward, const int stepCount, const int stepLength, int* incr, Color color, Vector2* pHit)
{

	Vector2 start = Vector2{ position.x,position.y };

	Vector2 end = Vector2{ (position.x + (forward.x / stepLength)), (position.y + (forward.y / stepLength)) };

	pHit->x = end.x;
	pHit->y = end.y;

	DrawLine(start.x * TileSize + TileSize / 2, start.y * TileSize + TileSize / 2, end.x * TileSize + TileSize / 2, end.y * TileSize + TileSize / 2, WHITE);

	if (!IsHit(map, end, 0.5) && *incr < stepCount)
	{
		*incr += 1;
		StepRay(end, forward, stepCount, stepLength, incr, color, pHit);
	}
	else
	{
		*incr = 0;
	}
}


void Render3DMap(Vector2 cameraPosition, float cameraRotation, int lineThickness, int fov)
{
	for (int i = -fov / 2; i < fov / 2; i++)
	{
		int c = 0;

		Vector2 hit;

		Vector2 dir = Vector2{ (float)sin((cameraRotation + i) * (PI / 180)), (float)cos((cameraRotation + i) * (PI / 180)) }; //seno de la rotacion de la camara(player) en radianes pasada a grados 
																															   //coseno'' ''   ''    '' ''   ''   ''    ''      ''   '' ''
		StepRay(cameraPosition, dir, 2000, 100, &c, GRAY, &hit);

	}
}

void Render2DMap(const int map[10][20])
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			Color color = BROWN;
			if (map[i][j] == 1) {
				color = DARKGREEN;
			}

			DrawRectangle(j * TileSize, i * TileSize, TileSize, TileSize, color);
		}
	}
}

void Render2DPlayer(const Vector2 position) 
{
	DrawCircle(position.x * TileSize + TileSize / 2, position.y * TileSize + TileSize / 2, 6, PURPLE);
}

Vector2 Update2DPlayer(Vector2* playerPosition, int* rotation)
{
	Vector2 forward = Vector2{ (float)sin(*rotation * (PI / 180)), (float)cos(*rotation * (PI / 180)) };

	Vector2 velocity = Vector2{ 0, 0 };

	if (IsKeyDown(KEY_W))
		velocity = Vector2{ 0.01f * forward.x, 0.01f * forward.y };


	if (IsKeyDown(KEY_S))
		velocity = Vector2{ -0.01f * forward.x, -0.01f * forward.y };


	if (IsKeyDown(KEY_E))
		(*rotation)--;


	if (IsKeyDown(KEY_Q))
		(*rotation)++;


	if (!IsHit(map, Vector2{ playerPosition->x + velocity.x, playerPosition->y + velocity.y }, 0.5))
	{
		playerPosition->x += velocity.x * GetFrameTime() * 200;
		playerPosition->y += velocity.y * GetFrameTime() * 200;
	}

	return forward;
}



void main()
{
	const int windowWidth = 800;
	const int windowHeight = 450;

	InitWindow(windowWidth, windowHeight, "Algebra EJ4");


	Vector2 playerPosition = Vector2{ 2, 2 };
	Vector2 playerForward = {};
	int playerRotation = 0;

	int lineWidth = 10;

	Vector2 mousePosition;
	float vectorModule;
	Vector2 normalizeDirect;


	while (!WindowShouldClose())
	{

		/*mousePosition = GetMousePosition();

		Vector2 vectorDirection = { mousePosition.x - GetScreenWidth() / 2,  mousePosition.y - GetScreenHeight() / 2 };
		float arcTan = atan2(vectorDirection.y, vectorDirection.x);
		float angle = arcTan;

		vectorModule = static_cast<float>(sqrt(pow(vectorDirection.x, 2) + pow(vectorDirection.y, 2)));
		normalizeDirect = { vectorDirection.x / vectorModule, vectorDirection.y / vectorModule };*/

		playerForward = Update2DPlayer(&playerPosition, &playerRotation);


		BeginDrawing();
		ClearBackground(BLACK);
		Render2DMap(map);
		Render2DPlayer(playerPosition);
		Render3DMap(playerPosition, playerRotation, lineWidth, 60);


		EndDrawing();

	}
	CloseWindow();

}