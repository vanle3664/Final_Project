#include "targetver.h"
#include "Shaders.h"
#include "Application.h"
#include "utilities.h" 
//#include "vld.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

GLint screenWidth = 480;
GLint screenHeight = 320;
GLint xSpeed = 100;
GLint ySpeed = 100;
GLint gravity = 9;
GLint score = 0;
GLint highScore[3];
bool isMute = false;

void LoadHighScore()
{
	printf("load \n");

	
	fstream f;
	f.open("highscore.txt", ios::in);

	string data;
	int i = 0;
	string line;
	while (!f.eof())          // (1)
	{
		getline(f, line); // (2)
		data += line + " ";
	}

	f.close();
	while (data.find(" "))
	{
		highScore[i++] = std::stoi(data.substr(0, data.find(" ")));
		data = data.substr(data.find(" ") + 1, data.length());
	}

	for (int i = 0; i < 3; i++)
	{
		printf("%d ", highScore[i]);
	}
}


GLint Init(ESContext* esContext)
{	
	//LoadHighScore();
	Application::GetInstance()->Init();
	return 0;
}

void Draw(ESContext* esContext)
{
	Application::GetInstance()->Render();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, GLfloat deltaTime)
{
	Application::GetInstance()->Update(deltaTime);
}

void Key(ESContext* esContext, unsigned char key, bool bbIsPresseded)
{
	Application::GetInstance()->HandleKeyEvent(key, bbIsPresseded);
}

void Mouse(ESContext* esContext, GLint x, GLint y, bool bbIsPresseded)
{
	Application::GetInstance()->HandleTouchEvent(x, y, bbIsPresseded);
}

void CleanUp()
{
	Application::GetInstance()->Exit();
	Application::FreeInstance();
}

GLint _tmain(GLint argc, _TCHAR* argv[])
{


	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, "Jungle Conquer", screenWidth, screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);
	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	//MemoryDump();
	//printf("Press any key...\n");
	//_getch();

	return 0;
}

//#endif