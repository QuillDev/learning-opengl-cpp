#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source) //MAKE SURE THE SOURCE STRING STILL EXISTS WHEN THIS CODE IS COMPILED
{
	unsigned int id = glCreateShader(type); //create a vertex shader and get the id
	const char* src = source.c_str(); //get a raw string instead of a c++ one since opengl uses it
	glShaderSource(id, 1, &src, nullptr); // => reference http://docs.gl/gl4/glShaderSource
	glCompileShader(id); //compile the shader @ the id

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	//Check if our shader failed to compile
	if (result == GL_FALSE)
	{
		//get the length of the gl shader log length
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		//create room for our message by dynamically allocating enough size on the stack? => https://youtu.be/71BLZwRGUJE&t=1010
		char* message = (char*) _malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		printf("Failed to compile %s shader\n%s", (type == GL_VERTEX_SHADER) ? "vertex" : "fragment", message); //log compile failure messages

		glDeleteShader(id); //delete the shader because it failed.
		
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); //create the program for executing shaders
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //Create the vertex shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //Create the fragment shader

	//attach, link & validate the program.
	glAttachShader(program, vs); // => http://docs.gl/gl4/glAttachShader
	glAttachShader(program, fs);
	glLinkProgram(program); // => http://docs.gl/gl4/glLinkProgram
	glValidateProgram(program); // => http://docs.gl/gl4/glValidateProgram

	//delete our extra shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW); //give the binded buffer the data for drawing the geometry

	//Declare the layout of our buffer data => https://www.youtube.com/watch?v=x0H--CL2tUI
	glEnableVertexAttribArray(0); //enable the vertex attribute pointer we defined
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //info about position attribute of the vertex in the data

	//Shader creation https://youtu.be/71BLZwRGUJE?t=1178
	const std::string vertexShader = R"glsl(
		#version 330 core
		layout(location = 0) in vec4 position;
		void main()
		{
			gl_Position = position;
		}
		)glsl";

	const std::string fragmentShader = R"glsl(
		#version 330 core
		layout(location = 0) out vec4 color;
		void main()
		{
			color = vec4(1.0, 1.0, 0.0, 1.0);
		}
		)glsl";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); //draw the selected data as a triangle

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}