#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"

#pragma region Camera Declare
//Instance Camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0)); //世界坐标系下：(相机位置，目标位置，世界up）
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(-180.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Light Declare
LightDirectional lightd(glm::vec3(1.0f,1.0f,-1.0f),glm::vec3(glm::radians(45.0f), glm::radians(45.0f),0), glm::vec3(1.0f, 1.0f, 1.0f));
LightPoint lightp0(glm::vec3(1.0f, 0, 0), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(1.0f, 1.0f, 1.0f));
LightPoint lightp1(glm::vec3(0, 0, -1.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(1.0f, 0, 0));
LightPoint lightp2(glm::vec3(0, 1.0f, 0), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(0, 1.0f, 0));
LightPoint lightp3(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0), glm::vec3(0, 0, 3.0f));
LightSpot lights(glm::vec3(0, 5.0f, 0), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(2.0f, 2.0f, 2.0f));

#pragma endregion

#pragma region Model Data
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion

#pragma region Input Declare
float lastX;
float lastY;
bool firstCalMouse = true;
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = -1.0f;
	}
	else {
		camera.speedZ = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.speedX = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.speedX = -1.0f;
	}
	else {
		camera.speedX = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.speedY = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.speedY = -1.0f;
	}
	else {
		camera.speedY = 0;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstCalMouse == true) {
		lastX = xPos;
		lastY = yPos;
		firstCalMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(deltaX, deltaY);
}
#pragma endregion

#pragma region load image to GPU
unsigned int LoadImageToGPU(const char* filename,GLint internalFormat,GLenum format,int textureSlot) {
	unsigned int TextBuffer;
	glGenTextures(1, &TextBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TextBuffer);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "load image failed!" << std::endl;
	}
	stbi_image_free(data);
	return TextBuffer;
}
#pragma endregion

int main() {
	#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//open GLFW window
	GLFWwindow* window = glfwCreateWindow(800,600,"my openGL game",NULL,NULL);
	if (window == NULL) {
		std::cout << "open openGL failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "failed to init GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	#pragma endregion

	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");

	Material* myMaterial = new Material(myShader,
		LoadImageToGPU("container2.png",GL_RGBA,GL_RGBA,Shader::DIFFUSE), //diffuse
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR),  //specular
		glm::vec3(1.0f, 1.0f, 1.0f),  //ambient
		32.0f);  //shininess

	#pragma region Init and load Models to VAO,VBO
	Mesh cube(vertices);
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//
	//unsigned int VBO;
	//glGenBuffers(1,&VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//
	////unsigned int EBO;
	////glGenBuffers(1, &EBO);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//
	//glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(7);
	//glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(8);
	//glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(9);
	#pragma endregion

	#pragma region Init and load Textures
	//unsigned int TextBufferA;
	//TextBufferA = LoadImageToGPU("container.jpg",GL_RGB,GL_RGB,0);
	//unsigned int TextBufferB;
	//TextBufferB = LoadImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 0);
	#pragma endregion

	glm::mat4 viewMat;
	while (!glfwWindowShouldClose(window)) {

		//prepare MVP matrices
		glm::mat4 modelMat;
		modelMat = glm::rotate(modelMat, (float)glfwGetTime()*glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0));
		glm::mat4 projMat;
		projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		
		// process input
		processInput(window);

		//clear screen
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewMat = camera.getViewMatrix();
		for (int i = 0; i < 10; i++)
		{
			//set model matrix
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			//modelMat = glm::rotate(modelMat, (float)glfwGetTime() * glm::radians(-55.0f)*(i+1), glm::vec3(1.0f, 1.0f, 0));

			//set view and projection matrices here

			//set material -> shader program
			myShader->use();
			//set material -> textures
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
			//glActiveTexture(GL_TEXTURE0 + 1);
			//glBindTexture(GL_TEXTURE_2D, myMaterial->specular);
			//set material -> uniform
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 1);
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(myShader->ID, "objColor"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightd.pos"), lightd.position.x,lightd.position.y,lightd.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lightd.color"), lightd.color.x,lightd.color.y,lightd.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightd.dirToLight"), lightd.direction.x, lightd.direction.y, lightd.direction.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

			//4 light point
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp0.pos"), lightp0.position.x, lightp0.position.y, lightp0.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp0.color"), lightp0.color.x, lightp0.color.y, lightp0.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp0.dirToLight"), lightp0.direction.x, lightp0.direction.y, lightp0.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp0.constant"), lightp0.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp0.linear"), lightp0.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp0.quadratic"), lightp0.quadratic);

			glUniform3f(glGetUniformLocation(myShader->ID, "lightp1.pos"), lightp1.position.x, lightp1.position.y, lightp1.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp1.color"), lightp1.color.x, lightp1.color.y, lightp1.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp1.dirToLight"), lightp1.direction.x, lightp1.direction.y, lightp1.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp1.constant"), lightp1.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp1.linear"), lightp1.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp1.quadratic"), lightp1.quadratic);

			glUniform3f(glGetUniformLocation(myShader->ID, "lightp2.pos"), lightp2.position.x, lightp2.position.y, lightp2.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp2.color"), lightp2.color.x, lightp2.color.y, lightp2.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp2.dirToLight"), lightp2.direction.x, lightp2.direction.y, lightp2.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp2.constant"), lightp2.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp2.linear"), lightp2.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp2.quadratic"), lightp2.quadratic);

			glUniform3f(glGetUniformLocation(myShader->ID, "lightp3.pos"), lightp3.position.x, lightp3.position.y, lightp3.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp3.color"), lightp3.color.x, lightp3.color.y, lightp3.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightp3.dirToLight"), lightp3.direction.x, lightp3.direction.y, lightp3.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp3.constant"), lightp3.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp3.linear"), lightp3.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightp3.quadratic"), lightp3.quadratic);
			
			glUniform3f(glGetUniformLocation(myShader->ID, "lights.pos"), lights.position.x, lights.position.y, lights.position.z); //点光源
			glUniform3f(glGetUniformLocation(myShader->ID, "lights.color"), lights.color.x, lights.color.y, lights.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lights.dirToLight"), lights.direction.x, lights.direction.y, lights.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lights.constant"), lights.constant);
			glUniform1f(glGetUniformLocation(myShader->ID, "lights.linear"), lights.linear);
			glUniform1f(glGetUniformLocation(myShader->ID, "lights.quadratic"), lights.quadratic);
			glUniform1f(glGetUniformLocation(myShader->ID, "lights.cosPhyInner"), lights.cosPhyInner);
			glUniform1f(glGetUniformLocation(myShader->ID, "lights.cosPhyOutter"), lights.cosPhyOutter);

			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			//myMaterial->shader->SetUniform3f("material.diffuse", myMaterial->diffuse);
			//myMaterial->shader->SetUniform3f("material.specular", myMaterial->specular);
			myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);
			

			//set model
			//glBindVertexArray(VAO);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			//drawcall
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			cube.Draw(myMaterial->shader);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//clean up, prepare for next render loop
		//check and call events and setup the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();
	}

	//exit program
	glfwTerminate();
	return 0;
}

