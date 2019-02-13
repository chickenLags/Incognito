#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h";
#include <src/SOIL.h>
#include "Include\glm\glm.hpp";
#include "Include\glm\gtc\matrix_transform.hpp";
#include "Include\glm\gtc\type_ptr.hpp";




//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//close on escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}


// make vertices for triangle
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

GLfloat vertices_color[] = {
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f
};

// SQUARE x, y, z,			r, g, b,			s, t
GLfloat vertices_color_texture[] = {
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f
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




int main() {
	
	GLFWwindow * window = init();

	// compile and create program (and delete shaders afterwards)
	Shader myShader("bookcase.vs", "bookcaseDuoTexture.frag");

	

	GLuint EBO;
	GLuint VAO;
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color_texture), vertices_color_texture, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// set data for color in vertexData
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// set data for texture in vertexData
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// set indices in case of EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// stop using VAO
	glBindVertexArray(0);


	//==============================================
	//	TEXTURES
	//==============================================

	// texture 1
	//===========
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // bound and settings for texture
	glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);

	//wrapping parameters & set filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture, and generate mipmap
	int width, height;
	unsigned char* image = SOIL_load_image("img\\container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// texture 2
	//==========

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);

	//wrapping parameters & set filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture, and generate mipmap
	image = SOIL_load_image("img\\awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	//==============================================
	//	Test spot
	//==============================================

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;


	
	//realTrans = glm::rotate(realTrans, 3.14159265358979323846264338327950288f / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));






	//==============================================
	//	GAME LOOP
	//==============================================

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		myShader.Use();

		// set offset uniform
		GLfloat timeValue = glfwGetTime();
		GLfloat offsetValue = (sin(timeValue) / 2);
		GLint vertexMyOffsetLocation = glGetUniformLocation(myShader.Program, "offset");
		glUniform3f(vertexMyOffsetLocation, offsetValue, 0.0f, 0.0f);

		// set texture uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(myShader.Program, "ourTexture2"), 1);

		// mad uniform translations!
		glm::mat4 realTrans(1.0f);
		realTrans = glm::scale(realTrans, glm::vec3(0.5f, 0.5f, 0.5f));
		realTrans = glm::translate(realTrans, glm::vec3(0.8f, 0.8f, 0.0f));
		realTrans = glm::rotate(realTrans, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));
		GLuint transformLoc = glGetUniformLocation(myShader.Program, "myMat");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(realTrans));


		
		//drawing
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;

	

}