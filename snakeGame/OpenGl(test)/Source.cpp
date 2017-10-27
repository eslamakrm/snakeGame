#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include <time.h>
#include <random>

#define SNAKEHEADX vertex[0]
#define SNAKEHEADY vertex[1]

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void windowSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


void snakeGame();
unsigned int vbo, vao;
int vertexuniformLocation;
GLFWwindow* window;

int SCRwidth = SCR_WIDTH, SCRheight = SCR_HEIGHT;


GLfloat vertex[20000000];

int main()
{
#pragma region Window



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

	for (int i = 0; i < 10000; i++)
	{
		vertex[i] = 2.0;
	}
	//bool usingPoints = false; //replace with true to use points instead of lines
	//gradiantTriangle();
	//colorChangingTriangle();
	//DrawSquare();
	//MouseLine(usingPoints);
	snakeGame();
	return 0;
}





void snakeGame()
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

	vertex[200] = 0.8f;
	vertex[201] = 0.8f;

	vertex[202] = 0.8f;
	vertex[203] = -0.8f;

	vertex[204] = -0.8f;
	vertex[205] = -0.8f;

	vertex[206] = -0.8f;
	vertex[207] = 0.8f;


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	unsigned int indices[] = { // note that we start from 0!
		 100,101,
		 102,103,
		 100,103,
		 101,102
		
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


	long startTime = clock();
	long BallStart = clock();
	float BallXpos;
	float BallYpos;

	BallXpos = 0.1f;
	BallYpos = 0.1f;
	vertex[100] = 0.1f;
	vertex[101] = 0.1f;
	
	bool flag = true;
	srand(time(NULL));
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
		if (endTime - startTime >= 50)
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
			
			startTime = endTime;
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
				return;
			}
		}
		
		if (SNAKEHEADX >= 0.799f || SNAKEHEADX <= -0.799f || SNAKEHEADY >= 0.799f || SNAKEHEADY <= -0.799f)
			return;

	 ///////////////////////////////////////////////////////////////////////////////////////////
		if ((SNAKEHEADX+0.01>= BallXpos && SNAKEHEADX - 0.01 <= BallXpos && SNAKEHEADY + 0.01 >= BallYpos && SNAKEHEADY - 0.01 <= BallYpos) )
		{
		
		
		
			float newPosX;
			float newPosY;
			float newPosX2;
			float newPosY2;
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
			BallXpos = ((float(rand()) / float(RAND_MAX)) * (1.5)) - 0.8;
			BallYpos = ((float(rand()) / float(RAND_MAX)) * (1.5)) - 0.8;
			vertex[100] = BallXpos;
			vertex[101] = BallYpos;
		}

		glDrawArrays(GL_POINTS, 0, 300);
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
	glfwTerminate();



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
