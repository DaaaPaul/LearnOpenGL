#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"
#include <iostream>

const unsigned WINDOW_HEIGHT = 600;
const unsigned WINDOW_WIDTH = 800;
bool lineMode = false;
bool stopper = false;

void windowSizeAdjustCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mapInputToGlfwState(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (lineMode && !stopper) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			lineMode = false;
			stopper = true;
		} else if (!lineMode && !stopper) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			lineMode = true;
			stopper = true;
		}
	} else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
		stopper = false;
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "GLFWwindow object creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD setup failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, windowSizeAdjustCallback);

	Shader shaderProgram("src/shaders/vertex.txt", "src/shaders/fragment.txt");

	const float vboData[] = {
		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 
	    -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 
		 0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 
		 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 
	};
	const unsigned eboData[] = {
		0, 1, 2,
		3, 2, 1,
	};

	unsigned vao = 0;
	glGenVertexArrays(1, &vao);
	unsigned vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);
	unsigned ebo = 0;
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboData), eboData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	stbi_set_flip_vertically_on_load(true);
	unsigned texture0 = 0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width0 = 0, height0 = 0, channels0 = 0;
	unsigned char* imageData0 = stbi_load("src/textures/sion.jpg", &width0, &height0, &channels0, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);

	unsigned texture1 = 0;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width1 = 0, height1 = 0, channels1 = 0;
	unsigned char* imageData1 = stbi_load("src/textures/awsome.png", &width1, &height1, &channels1, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData1);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);

	stbi_image_free(imageData0);
	stbi_image_free(imageData1);
	
	shaderProgram.use();
	shaderProgram.setUniform("text0", 0);
	shaderProgram.setUniform("text1", 1);
	
	double nextSecond = 1.0;
	uint32_t framesInSecond = 0;
	while (!glfwWindowShouldClose(window)) {

		if(glfwGetTime() > nextSecond) {
			nextSecond++;
			std::cout << "Frames last second: " << framesInSecond << '\n';
			framesInSecond = 0;
		} else {
			framesInSecond++;
		}

		glfwPollEvents();
		mapInputToGlfwState(window);

		glClearColor(0.3f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void*>(0));
		glBindVertexArray(NULL);
		glUseProgram(NULL);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	shaderProgram.kill();
	glfwTerminate();
	return 0;
}