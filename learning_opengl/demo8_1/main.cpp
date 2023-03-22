#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include "include/shader.h"
#include "include/camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const float SCR_WIDTH = 800;
const float SCR_HEIGHT = 600;
const char* vertexShaderPath = "../shader/shader.vs";
const char* fragmentShaderPath = "../shader/shader.fs";
const char* img1Path = "../img/container.jpg";
const char* img2Path = "../img/awesomeface.png";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// shader
Shader* shader = NULL;
// camera
Camera *camera = NULL;

// frame time
float deltaTime = 0;
float lastFrame = 0;


unsigned int load_texture2D(const char* imgPath);
float vertices[] = {
	// location           texture coord
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};



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

	glfwSetCursorPosCallback(window, mouse_callback); // set mouse callback
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide mouse cursor

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glViewport(0, 0, 800, 600);

	// configure global opengl state
	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// create shader
	shader = new Shader(vertexShaderPath, fragmentShaderPath);
	// create camera
	camera = new Camera();

	// put some vertices into vbo
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// location attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coords attrib
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
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
		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT
				);
		
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
	
		
		// select shaders
		shader->use();	
		
		glm::mat4 view=glm::mat4(1.0f);
		// create transformations
		view = camera->getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), SCR_WIDTH / SCR_HEIGHT, 1.0f, 100.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		float theta = (float)glfwGetTime()*10;
		// trans = glm::rotate(trans, glm::radians(theta), glm::vec3(0.5f, 1.0f, 1.0f));
		// trans = glm::scale(trans, glm::vec3(abs(sin(glfwGetTime())), abs(sin(glfwGetTime())), abs(sin(glfwGetTime()))));
		// get transform uniform
		shader->uniformSet("view", view);
		shader->uniformSet("projection", projection);
        shader->uniformSet("trans", trans);
		
		// render container
		glBindVertexArray(VAO);
		for(int i=0; i<10; i++){
			glm::mat4 model=glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f*i;
			if(i % 3 == 0){
				angle = glfwGetTime() * 25.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			shader->uniformSet("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);	
		}	

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	delete shader;
	delete camera;
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
	
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float cameraSpeed = 2.5*deltaTime; // adjust accordingly
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera->processKeyboard(FORWARD, deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		camera->processKeyboard(BACKWARD, deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera->processKeyboard(LEFT, deltaTime);
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		camera->processKeyboard(RIGHT, deltaTime);
	}
}

float lastX=0.0f, lastY=0.0f;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
	
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	
	static bool firstMouse=true;
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse=false;
	}
	float xoffset = xpos -lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera->processMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->processMouseScroll(yoffset);
}

