#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include "include/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const char* vertexShaderPath = "../shader/shader.vs";
const char* fragmentShaderPath = "../shader/shader.fs";
const char* img1Path = "../img/container.jpg";
const char* img2Path = "../img/awesomeface.png";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int load_texture2D(const char* imgPath);

int main(){

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);	
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glViewport(0, 0, 800, 600);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
	// location                      color         texture 
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,      1.0f, 1.0f,  // right up   
		0.5f, -0.5f, 0.0f,	  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // right bottom
		-0.5f, -0.5f, 0.0f,	  0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // left bottom
		-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // left up
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// create shader
	Shader* shader = new Shader(vertexShaderPath, fragmentShaderPath);
	

	// put some vertices into vbo
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// location attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coords attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	// load and create a texture
	unsigned int texture1 = load_texture2D(img1Path);
	unsigned int texture2 = load_texture2D(img2Path);
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	shader->use();
	shader->uniformSet("texture1", 0);
	shader->uniformSet("texture2", 1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	
	while(!glfwWindowShouldClose(window))
	{
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// create transformations
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		// select shaders
		shader->use();	
		// get matrix's uniform location and set matrix
		shader->uniformSet("transform", trans);

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(abs(sin(glfwGetTime())), abs(sin(glfwGetTime())), abs(sin(glfwGetTime()))));
		shader->uniformSet("transform", trans);

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	delete shader;
	glfwTerminate();
	return 0;

}

// return texture ID
unsigned int load_texture2D(const char* imgPath){
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the wrapping and filtering methods for the currently bound texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// load and generate textures
	int width, height, nrChannels;
	// OpenGL requires that the y-axis 0.0 coordinate be at the bottom of the image
	// but the y-axis 0.0 coordinate of the image is usually at the top. 
	// Reversing the image meets the requirements of OpenGL
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (data){
		if(nrChannels == 3){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);	
		}else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);	
		}
	} else {
		std::cout << "Failed to load texture at " << imgPath << std::endl;
	}	
	stbi_image_free(data); // free img memory
	return texture;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}
