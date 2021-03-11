#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "GLDebugging/GLDebugMessageCallback.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) 
{
	std::ifstream stream(filepath); //open the file

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			//if we find the vertex tag, set the mode to vertex
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			//if we find the fragment tag, set the mode to vertex
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

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
		char* message = (char*)_malloca(length * sizeof(char));
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

	//Enable the debug function callback
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 640, "OpenGL Learning Project | C++", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current.
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Initialize glew and make sure it is okay.
	if (glewInit() != GLEW_OK)
	{
		printf("glew did not return 'OK' status.");
		return -1;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDebugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION)); //print opengl version + driver compat version


	//positions for the triangle
	float positions[8] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f, 
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	unsigned int indicies[6] = {
		0, 1, 2,
		2, 3, 0
	};

	//generate data for the Triangle => https://www.youtube.com/watch?v=0p9VxImr7Y0
	unsigned int buffer; //id of the buffer
	glGenBuffers(1, &buffer); //amount to create & buffer to output to.
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //select the buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW); //give the binded buffer the data for drawing the geometry

	//Declare the layout of our buffer data => https://www.youtube.com/watch?v=x0H--CL2tUI
	glEnableVertexAttribArray(0); //enable the vertex attribute pointer we defined
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //info about position attribute of the vertex in the data

	//create the index buffer => https://youtu.be/MXNMC1YAxVQ?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&t=567
	unsigned int indexBuffer; //id of the buffer
	glGenBuffers(1, &indexBuffer); //amount to create & buffer to output to.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); //select the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW); //give the binded buffer the data for drawing the geometry

	//Shader creation https://youtu.be/71BLZwRGUJE?t=1178
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); //create the shader from out sources
	glUseProgram(shader); //select the shader for use in our program

	int location = glGetUniformLocation(shader, "u_Color"); //get the uniform inside the shader

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//Uniform video https://youtu.be/DE6Xlx_kbo0?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
		glUniform4f(location, r, 0.7f, 0.8f, 1.0f); //modify the value of the uniform inside of teh shader
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0)
		{
			increment = 0.05f;
		}

		r += increment;
		g += increment /2.f;
		b += increment / 4.f;
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}