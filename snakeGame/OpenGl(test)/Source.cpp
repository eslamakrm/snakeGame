#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include <time.h>
#include <random>
#include <fstream>
#include <string>
#define SNAKEHEADX vertex[0]
#define SNAKEHEADY vertex[1]
#define BALLX vertex[4000]
#define BALLY vertex[4001]
#define BORDERX 0.5f
#define BORDERY 0.5f
#define SNAKE_SPEED 20
#define BIGBALL_START 10002
using namespace std;
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
enum DIRECTIONS {
	UP, DOWN, LEFT, RIGHT
};
int snakedirection = LEFT;
//////////////////////////
void updateSnakeBody(int, float, float);

////////////////////////////
int ReadHighScore();
bool UpdateHighScore(int);
void removeBigBall(int);
void updateBigBall(int,bool,float,float);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void windowSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int snakeGame();
unsigned int vbo, vao;
int vertexuniformLocation;
GLFWwindow* window;

int SCRwidth = SCR_WIDTH, SCRheight = SCR_HEIGHT;


GLfloat vertex[20000000];

int main()
{
	

#pragma region WindowInit



	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	///////////making buffer into GPU to save vertices to it///////////////////////
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	///////////////////////////////////////////////////////////////
#pragma endregion

	char restart;
	for (int i = 0; i < 20000000; i++)
	{
		vertex[i] = 2.0;
	}
	//bool usingPoints = false; //replace with true to use points instead of lines
	//gradiantTriangle();
	//colorChangingTriangle();
	//DrawSquare();
	//MouseLine(usingPoints);
	int currentScore;
	currentScore = snakeGame();
	cout << "##############################################################"<<endl;
	int highScore = ReadHighScore();
	if (highScore == -1)
		cout << "Unable to read High score !!!" << endl;
	else
		cout << "The Recorded High Score = " << highScore << endl;

	cout << "your Score = ";

	cout << currentScore << endl;
	if (highScore < currentScore)
	{
		cout << "************* New High Score *************" << endl;
		UpdateHighScore(currentScore);
	}

	cout << "Try again ? (y,n)" << endl;
	cin>>restart;
	 if (restart == 'Y' || restart == 'y')
		 main();
		 
	
		return 0;
}





int snakeGame()
{
	unsigned int VBO, VAO,EBO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, vertex, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	vertex[2000] = BORDERX;
	vertex[2001] = BORDERY;

	vertex[2002] = BORDERX;
	vertex[2003] = -BORDERY;

	vertex[2004] = -BORDERX;
	vertex[2005] = -BORDERY;

	vertex[2006] = -BORDERX;
	vertex[2007] = BORDERY;


	
	unsigned int indices[] = { 
		 1000,1001,
		 1002,1003,
		 1000,1003,
		 1001,1002
		
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(5);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	//glVertexAttribI2i(vbo, 100, 200);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	float xPos = 0.0f;
	float yPos = 0.0f;
	int snakelen = 0;
	vertex[snakelen++] = xPos;
	vertex[snakelen++] = yPos;
	vertex[snakelen++] = 0.01;
	vertex[snakelen++] = yPos;

	long bigBallStart;
	long bigBallUpdate;

	float BallXpos;
	float BallYpos;

	BallXpos = 0.1f;
	BallYpos = 0.1f;

	float bigBallX;
	float bigBallY;
	BALLX = 0.1f;
	BALLY = 0.1f;
	bool updated = false;
	bool flag = true;
	srand(time(NULL));

	int score = 0;
	int bigballscore=0;
	bool bigBallExist = false;
	long startTime = clock(); //return processor time consumed by the pre-code excution
	long BallStart = clock();

	while (!glfwWindowShouldClose(window))
	{
		
		
		processInput(window);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// draw our first triangle
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
								//glLineWidth(100);
	


		long endTime = clock();
		if (endTime - startTime >= SNAKE_SPEED)
		{
			switch (snakedirection)
			{
			case UP:
				yPos = yPos + 0.01f;
				break;
			case DOWN:
				yPos = yPos - 0.01f;
				break;
			case LEFT:
				xPos = xPos - 0.01f;
				break;
			case RIGHT:
				xPos = xPos + 0.01f;
				break;
			default:
				break;
			}
			updateSnakeBody(snakelen, xPos, yPos);
			
			startTime = clock();
		}
		//glDrawArrays(GL_POINTS, 0, arrayindex);

		/*	switch (snakedirection)
		{
		case UP:

		default:
		break;
		}
		*/
		/////////////////////////////////// die conditions //////////////////////////////////////
		for (int ii = 2; ii < snakelen; ii += 2) {
			if (SNAKEHEADX == vertex[ii] && SNAKEHEADY == vertex[ii + 1]) {
				glfwSetWindowShouldClose(window, true);
			}
		}
		
		if (SNAKEHEADX >= BORDERX-0.001 || SNAKEHEADX <= -BORDERX-0.001 || SNAKEHEADY >= BORDERY-0.001 || SNAKEHEADY <= -BORDERY-0.001)
			glfwSetWindowShouldClose(window, true);

	 /////////////////////////////// eat ball  ////////////////////////////////////////////////////////////
		if ((SNAKEHEADX+0.015>= BallXpos && SNAKEHEADX - 0.015 <= BallXpos && SNAKEHEADY + 0.015 >= BallYpos && SNAKEHEADY - 0.015 <= BallYpos) )
		{

			float newPosX;
			float newPosY;
			float newPosX2;
			float newPosY2;
			//////////////// add new point to snake ////////////////////////
			if (vertex[snakelen - 1] == vertex[snakelen - 3]) //y changing
			{
				if (vertex[snakelen - 1] > vertex[snakelen - 3])
				{
					newPosX = vertex[snakelen - 2];
					newPosY = vertex[snakelen - 1] + 0.01f;
					newPosX2 = vertex[snakelen - 2];
					newPosY2 = vertex[snakelen - 1] + 0.02f;
				}
				else
				{
					newPosX = vertex[snakelen - 2];
					newPosY = vertex[snakelen - 1] - 0.01f;
					newPosX2 = vertex[snakelen - 2];
					newPosY2 = vertex[snakelen - 1] - 0.02f;
				}
			}
			else //x changing
			{
				if (vertex[snakelen - 1] > vertex[snakelen - 3])
				{
					newPosY = vertex[snakelen - 1];
					newPosX = vertex[snakelen - 2] + 0.01f;
					newPosY2 = vertex[snakelen - 2];
					newPosX2 = vertex[snakelen - 1] + 0.02f;
				}
				else
				{
					newPosY = vertex[snakelen - 1];
					newPosX = vertex[snakelen - 2] - 0.01f;
					newPosY2 = vertex[snakelen - 2];
					newPosX2 = vertex[snakelen - 1] - 0.02f;
				}
			}
		
			vertex[snakelen++] = newPosX;
			vertex[snakelen++] = newPosY;
			vertex[snakelen++] = newPosX;
			vertex[snakelen++] = newPosY;
			///////////////////// generate new ball //////////////////////////////
			BallXpos = ((float(rand()) / float(RAND_MAX)) * (2*(BORDERX-0.1))) - (BORDERX - 0.02);
			BallYpos = ((float(rand()) / float(RAND_MAX)) * (2 * (BORDERY - 0.1))) - (BORDERY - 0.02);
			////////////////////////////////////////////////////////////////////
			BALLX = BallXpos;
			BALLY = BallYpos;
			score++;
		}



		////////////////////// big ball ///////////////////////////////////
		if ((score+1) % (5 * (bigballscore+1)) == 0 && !bigBallExist) //form big ball
		{
			bigballscore++;
			bigBallStart = clock();
			bigBallUpdate = bigBallStart;
			bigBallExist = true;
			bigBallX = ((float(rand()) / float(RAND_MAX)) * (2 * (BORDERX - 0.1))) - (BORDERX-0.02);
			bigBallY = ((float(rand()) / float(RAND_MAX)) * (2 * (BORDERY - 0.1))) - (BORDERY-0.02);
			int i = BIGBALL_START; //start index of bigball
			vertex[i++] = bigBallX;
			vertex[i++] = bigBallY;

			vertex[i++] = bigBallX + 0.01;
			vertex[i++] = bigBallY + 0.01;

			vertex[i++] = bigBallX - 0.01;
			vertex[i++] = bigBallY - 0.01;

			vertex[i++] = bigBallX - 0.01;
			vertex[i++] = bigBallY + 0.01;

			vertex[i++] = bigBallX + 0.01;
			vertex[i++] = bigBallY - 0.01;

			vertex[i++] = bigBallX-0.01;
			vertex[i++] = bigBallY;

			vertex[i++] = bigBallX;
			vertex[i++] = bigBallY-0.01;

			vertex[i++] = bigBallX;
			vertex[i++] = bigBallY + 0.01;

			vertex[i++] = bigBallX + 0.01;
			vertex[i++] = bigBallY;

		}
		         // after 8 seconds remove bonus ball
		if (bigBallExist && ( (long)clock() - bigBallStart >= 8000))
		{
			removeBigBall(BIGBALL_START);
			bigBallExist = false;
		}

		///////// only to update extraball shape (expand and compress) ////////////////
		if (((long)clock() - bigBallUpdate) >= 100 && bigBallExist)
		{
			if (!updated)
			{
				updateBigBall(BIGBALL_START,false,bigBallX,bigBallY);
				bigBallUpdate = clock();
				updated = true;
			}
			else
			{
				updateBigBall(BIGBALL_START, true, bigBallX, bigBallY);
				bigBallUpdate = clock();
				updated = false;
			}

		}
		///// extraball eat condition
		if (bigBallExist && (SNAKEHEADX >= vertex[BIGBALL_START+4]-0.01 && SNAKEHEADX <= vertex[BIGBALL_START+2] + 0.01 && SNAKEHEADY >= vertex[BIGBALL_START+5] - 0.01 && SNAKEHEADY <= vertex[BIGBALL_START+7] + 0.01) )
		{
			bigballscore++;
			removeBigBall(BIGBALL_START);
			bigBallExist = false;
		}
		//////////////////////////////////////////////////////////////////
		glDrawArrays(GL_POINTS, 0, 20000);
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------


		/*	vertexuniformLocation = glGetUniformLocation(shaderProgram, "color");
		if (lineVertices.size()  < 6)
		continue;
		glDrawArrays(GL_LINES, 0, lineVertices.size()/3);*/


	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	int currentScore = score + (10 * bigballscore);

	
	
	glfwTerminate();
	return currentScore;


}


void updateSnakeBody(int len,float xpos,float ypos)
{
	for (int i = len+1; i >= 0; i--)
	{
		vertex[i + 2] = vertex[i];
		vertex[i + 1] = vertex[i - 1];
	}
	SNAKEHEADX = xpos;
	SNAKEHEADY = ypos;
	
}



void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	//{
	//	vertices[1] = 0.5f;
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertices into the buffer
	//}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 1.0f, 0.0f, 0.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 0.0f, 1.0f, 0.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 0.0f, 0.0f, 1.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && snakedirection != DOWN)
		snakedirection = UP;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && snakedirection != UP)
		snakedirection = DOWN;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && snakedirection != RIGHT)
		snakedirection = LEFT;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && snakedirection != LEFT)
		snakedirection = RIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



void windowSize_callback(GLFWwindow* window, int width, int height)
{
	SCRheight = height;
	SCRwidth = width;
}


void removeBigBall(int start_index)
{
	for (int i = start_index; i <= start_index + 16; i++)
		vertex[i] = 2.0;
}

void updateBigBall(int start_index,bool updated,float x,float y)
{
	if (updated)
	{
		for (int i = start_index; i <= start_index + 9; i++)
			vertex[i] = 2.0;
	}
	else
	{
		vertex[start_index++] = x;
		vertex[start_index++] = y;

		vertex[start_index++] = x + 0.01;
		vertex[start_index++] = y + 0.01;

		vertex[start_index++] = x - 0.01;
		vertex[start_index++] = y - 0.01;

		vertex[start_index++] = x - 0.01;
		vertex[start_index++] = y + 0.01;

		vertex[start_index++] = x + 0.01;
		vertex[start_index++] = y - 0.01;

		vertex[start_index++] = x - 0.01;
		vertex[start_index++] = y;

		vertex[start_index++] = x;
		vertex[start_index++] = y - 0.01;

		vertex[start_index++] = x;
		vertex[start_index++] = y + 0.01;

		vertex[start_index++] = x + 0.01;
		vertex[start_index++] = y;
	}
}


int ReadHighScore()
{
	ifstream highScoreFile;
	highScoreFile.open("HighScore.txt");
	if (!highScoreFile) {
		return -1;
	}
	string sLine;
	getline(highScoreFile, sLine);
	
	highScoreFile.close();
	return stoi(sLine);
}


bool UpdateHighScore(int score)
{
	ofstream highScoreFile;
	highScoreFile.open("HighScore.txt");
	if (!highScoreFile) {
		return false;
	}
	highScoreFile.clear();
	highScoreFile << score;
	return true;
}