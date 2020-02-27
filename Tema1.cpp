// Mocanu-Prejma Vladimir-Georgian 335CB

#include "Tema1.h"

#include <iostream> 
#include <time.h>
#include <math.h>

#include <Core/Engine.h>
#include <GL/glu.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;
	srand((unsigned)time(NULL));

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initializam toate variabilele
	level = 1;
	speedPipe = 150 * level;
	acceleration = 9.8;
	speed = 8;
	score = 0;
	pauseScore = 0;
	startSlide = -1;
	pause = false;

	//initializam variabilele din structura pipeList
	for (int i = 0; i < 5; i++)
	{
		float rand1 = rand() % 15;
		rand1 = rand1 / 10;

		pipeList[i].x = 0;
		pipeList[i].position = i * 400;
		pipeList[i].scale = pow(-1, i) * rand1;
		pipeList[i].slide = -70;
	}

	// initializam scaleX, scaleY
	scaleX = 1;
	scaleY = 1;

	// initializa angularStep, x, y1, y
	angularStep = 0;
	y = 0;
	y1 = 0;
	x = 2;

	//crearea de obiecte
	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);

	Mesh* triangle = Object2D::CreateTriangle("triangle", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(triangle);

	Mesh* pipe2 = Object2D::CreateSquare("pipe2", corner, squareSide, glm::vec3(0, 1, 0), true);
	AddMeshToList(pipe2);

	Mesh* circle = Object2D::CreateCircle("circle", 10, glm::vec3(1, 0, 0), true);
	AddMeshToList(circle);
	circle->SetDrawMode(GL_TRIANGLE_FAN);

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();

	Bird();

	//scadere variabile pentru a creea ideea de gravitatie
	//daca este pauza, se opreste totul
	if (pause == false)
	{
		speed -= acceleration * deltaTimeSeconds;
		angularStep -= 1.5;
		y -= 0.6;
		y1 -= 1;
		x -= 0.3;
		translateY += speed * 150 * deltaTimeSeconds;
	}


	//obstacole
	for (int i = 0; i < 5; i++)
	{
		if (level > 0)
		{
			if (pipeList[i].slide == -70)
			{
				startSlide = 0;
			}
			else if (pipeList[i].slide == 70)
			{
				startSlide = 1;
			}
		}

		if (pipeList[i].x > resolution.x + 110 + pipeList[i].position)
		{
			pipeList[i].x = 0;
			pipeList[i].position = 500;

			pipeList[i].scale = pow(-1, rand() % 10) * (rand() % 20) / 10;
		}

		if (pause == false)
		{
			if (startSlide == 0)
			{
				pipeList[i].scale += 0.01;
				pipeList[i].slide += 1;
			}
			else if (startSlide == 1)
			{
				pipeList[i].scale -= 0.01;
				pipeList[i].slide -= 1;
			}
		}


		pipeList[i].x += speedPipe * deltaTimeSeconds;

		Pipe(resolution.x + pipeList[i].position, pipeList[i].scale, pipeList[i].x);
	}

	//score
	Score();

	//level
	if (score > 20 * level)
	{
		level += 1;
		speedPipe = 150 * level;
		cout << "\n\nLEVEL : " << level << "\n\n";
	}
}

//construire pasare
void Tema1::Bird()
{
	//corpul  = un patrat
	//aici il desenam si il facem sa se roteasca 
	square = glm::mat3(1);
	square *= Transform2D::Translate(150, 330);
	square *= Transform2D::Translate(3, translateY);

	square *= Transform2D::Translate(17, 17);
	square *= Transform2D::Rotate(RADIANS(angularStep));
	square *= Transform2D::Translate(-17, -17);

	square *= Transform2D::Scale(0.3, 0.3);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], square);

	//triunghi = cioc
	triangle = glm::mat3(1);
	triangle *= Transform2D::Translate(215, 344);
	triangle *= Transform2D::Translate(0, translateY);

	triangle *= Transform2D::Translate(x, y1);
	triangle *= Transform2D::Rotate(RADIANS(angularStep));

	triangle *= Transform2D::Scale(0.3, 0.3);
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], triangle);

	//cerc = cap
	circle = glm::mat3(1);
	circle *= Transform2D::Translate(197, 356);
	circle *= Transform2D::Translate(0, translateY);

	circle *= Transform2D::Translate(-5, y);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], circle);
}

//desenare obstacolelor
void Tema1::Pipe(int x, float scale, float translate)
{
	//generam un obstacol si il rotim, dupa il pozitionam sus
	pipe1 = glm::mat3(1);
	pipe1 *= Transform2D::Translate(x, 720);
	pipe1 *= Transform2D::Translate(50, 50);
	pipe1 *= Transform2D::Rotate(RADIANS(180));
	pipe1 *= Transform2D::Translate(-50, 50);

	pipe1 *= Transform2D::Translate(translate, 0);
	pipe1 *= Transform2D::Scale(1, 2.5 + (scale));
	RenderMesh2D(meshes["pipe2"], shaders["VertexColor"], pipe1);

	//generam al doilea obstacol, il plasam jos
	pipe2 = glm::mat3(1);
	pipe2 *= Transform2D::Translate(x, 0);
	pipe2 *= Transform2D::Translate(-translate, 0);
	pipe2 *= Transform2D::Scale(1, 2.5 + (-scale));

	RenderMesh2D(meshes["pipe2"], shaders["VertexColor"], pipe2);
}

/*
- calculare scor + afisare
- verificare terminare joc
- scorul creste in fiecare secunda
*/
void  Tema1::Score()
{
	int lastTime = glfwGetTime();

	if (score < lastTime - pauseScore)
	{
		//in caz de este pauza, creste scorul
		if (pause == true)
		{
			pauseScore = lastTime - score;

		}
		else {
			score = lastTime - pauseScore;

			//afisare scor din 2 in 2
			if (score % 2 == 0)
			{

				cout << "Score:" << score << "\n";
			}
		}
	}

	//terminare joc cand iesim din ecran
	if (translateY < -390 || translateY > 390)
	{
		cout << "\n\nGAME OVER!\n";
		cout << "FINAL SCORE: " << score << "\n";
		cout << translateY << "\n";
		Exit();
	}
}

/*
- daca apasam pe tasta S, creste viteza
- daca apasam pe SPACE, sare/zboara pasarea
- daca apasam pe P, punem pauza
*/
void Tema1::OnKeyPress(int key, int mods)
{

	if ((key == GLFW_KEY_S))
	{
		if (speedPipe < 200 + 150 * level) {
			speedPipe += 100;
		}
		else {
			speedPipe = 150 * level;
		}
	}

	if ((key == GLFW_KEY_SPACE))
	{
		angularStep = 60;
		y = 15;
		y1 = 30;
		x = 2;
		speed = 4;
	}

	if ((key == GLFW_KEY_P))
	{
		pause = true;

		if (speedPipe > 0) {
			speedPipe = 0;
			pause = true;
		}
		else {
			speedPipe = 150 * level;
			pause = false;
		}
	}

}
