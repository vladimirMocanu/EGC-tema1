#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <vector> 
#include <iostream> 

#include <Core/Engine.h>


class Tema1 : public SimpleScene
{
public:

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;


	void OnKeyPress(int key, int mods) override;
	//void OnKeyRelease(int key, int mods) override;
	void Tema1::Pipe(int x, float scale, float translate);
//	void Tema1::Pipe2(int x, float scale, float translate);
	void Tema1::Score();
	void Tema1::Bird();

protected:
	glm::mat3 square;
	glm::mat3 triangle;
	glm::mat3 triangle2;
	glm::mat3 pipe1;
	glm::mat3 pipe2;
	glm::mat3 circle;

	float translateX, translateY, x, y, y1;
	float speedPipe;
	bool pause;
	int startSlide;
	int pauseScore;
	struct pipe
	{
		int x;
		float scale;
		int position;
		int slide;

	};
	pipe pipeList[6];
	float acceleration;
	float speed;
	int score;
	int level;

	float scaleX, scaleY;
	float angularStep;
};
