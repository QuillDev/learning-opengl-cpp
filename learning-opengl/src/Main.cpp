#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
int main(void)
{
	GLFWwindow* window;

	//Initialize glfw & make sure it starts correctly.
	if (!glfwInit()) {
		printf("glfw did not initialize correctly\n");
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "OpenGL Learning Project | C++", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current.
	glfwMakeContextCurrent(window);

	//Initialize glew and make sure it is okay.
	if (glewInit() != GLEW_OK)
	{
		printf("glew did not return 'OK' status.");
		return -1;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION)); //print opengl version + driver compat version

	//positions for the triangle
	float positions[6] = {
	   -0.5f, -0.5f,
		0.0f,  0.5f,
		0.5f, -0.5f,
	};

	//generate data for the Triangle => https://www.youtube.com/watch?v=0p9VxImr7Y0
	unsigned int buffer; //id of the buffer
	glGenBuffers(1, &buffer); //amount to create & buffer to output to.
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //select the buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //give the binded buffer the data for drawing the geometry

	//Declare the layout of our buffer data => https://www.youtube.com/watch?v=x0H--CL2tUI
	glEnableVertexAttribArray(0); //enable the vertex attribute pointer we defined
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //info about position of the vertex

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}