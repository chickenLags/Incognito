#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//close on escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// shaders
static const char* vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main(){\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0); }\n";

static const char* fragment_shader =
"#version 330 core\n"
"out vec4 color; \n"
"void main(){\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f); } \n";

static const char* second_fragment_shader =
"#version 330 core\n"
"out vec4 color; \n"
"void main(){\n"
"color = vec4(0.2f, 0.9f, 0.3f, 1.0f);} \n";

// make vertices for triangle
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

GLfloat square_vertices[] = {
	 0.5f,  0.5f, 0.0f, // Top Right
	 0.5f, -0.5f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f, // Bottom Left
	-0.5f,  0.5f, 0.0f	// top left

};

GLfloat first_vertices[] = {
	-0.5f, 0.5f, 0.0f,	// left up
	-0.5f, -0.5f, 0.0f, // left bottom
	0.0f, -0.5f, 0.0f	// final corner in middle of screen
};

GLfloat second_vertices[] = {
	0.0f, 0.5f, 0.0f,	// left up
	0.0f, -0.5f, 0.0f, // left bottom
	0.5f, -0.5f, 0.0f	// final corner in middle of screen
};

GLuint indices[] = {
	0, 3, 1,	// first triangle
	1, 2, 3		// second triangle
};

GLuint makeShader(GLenum type, const char* shader) {

	// compile shaders 
	GLuint vertexShader;
	GLint succes;
	GLchar infoLog[512];

	vertexShader = glCreateShader(type);
	glShaderSource(vertexShader, 1, (const GLchar**)&shader, NULL);
	glCompileShader(vertexShader);

	// check whether compilation was successful
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" <<
			infoLog << std::endl;
	}

	return vertexShader;
}

GLuint makeProgram(GLuint vertexShader, GLuint fragmentShader){
	// create shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	GLint succes;
	GLchar infoLog[512];

	//check for errors:
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succes);
	if (!succes) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" <<
			infoLog << std::endl;
	}

	// remove compiled shaders;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;

}

GLFWwindow* init() {

	// init code
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//create window, error check, and then bind to window to glfw
	GLFWwindow* window = glfwCreateWindow(800, 600, "hello world", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//  initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize glew" << std::endl;
		return nullptr;
	}

	// set some settings (viewport, input callback
	glViewport(0, 0, 800, 600);
	glfwSetKeyCallback(window, key_callback);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	return window;
}

GLuint makeMesh(GLfloat vertices[] ) {

	GLuint VAO;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return VAO;

}


int main() {
	
	GLFWwindow * window = init();

	// compile and create program (and delete shaders afterwards)
	GLuint shaderProgram = makeProgram(makeShader(GL_VERTEX_SHADER, vertex_shader), makeShader(GL_FRAGMENT_SHADER, fragment_shader));
	GLuint second_shader_program = makeProgram(makeShader(GL_VERTEX_SHADER, vertex_shader), makeShader(GL_FRAGMENT_SHADER, second_fragment_shader));

	//GLuint VAO = makeMesh(vertices);

	GLuint VAO;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// set indices in case of EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		// use shaderprogram and bind VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;

	

}