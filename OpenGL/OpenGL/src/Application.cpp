#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <filesystem>
#include "stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
//#include <direct.h>
float mixValue = 0.2f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
	//�ӽ��ƶ�
float pitch = 0.0f;
float yaw = -90.0f;
bool firstMouse = true;
glm::vec3 direction;
float fov = 45.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float ambientStrength = 0.0;
float specularStrength = 0.5;
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
float lastX = 400, lastY = 300;
//�����ƶ�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;//��ҰԽС������Խ��
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//����ƶ�����
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;//�ƶ�x����
	float yoffset = lastY - ypos;//���������Ӧ����������
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch += yoffset;
	yaw += xoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	//�õ����ڵķ�������
	direction.y = glm::sin(glm::radians(pitch));
	direction.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	direction.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	direction = glm::normalize(direction);//��һ��
	cameraFront = direction;
}

void processInput(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//���������ƶ������ӽ�����
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//���������ƶ������ӽ�����
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//cameraPos.y = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		specularStrength += 0.1;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		specularStrength -= 0.1;
}		
int main()
{
	glfwInit();//ȷ���汾�������г�ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	// --------------------
	unsigned int screenWidth = 800;
	unsigned int screenHeight = 600;
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// ��ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(window, true);
	// ����Imgui�ķ��
	ImGui::StyleColorsDark();
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);


	//Ȼ�������û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵��á�����ص�������ԭ������
	Shader shader("./res/shaders/Basic.shader");
	//��������
	//float vertices[] = {
	//	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
	//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
	//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
	//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // ����
	//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	//};
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//glm::vec3 cubePositions[] = {
 // glm::vec3(0.0f,  0.0f,  0.0f),
 // glm::vec3(2.0f,  5.0f, -15.0f),
 // glm::vec3(-1.5f, -2.2f, -2.5f),
 // glm::vec3(-3.8f, -2.0f, -12.3f),
 // glm::vec3(2.4f, -0.4f, -3.5f),
 // glm::vec3(-1.7f,  3.0f, -7.5f),
 // glm::vec3(1.3f, -2.0f, -2.5f),
 // glm::vec3(1.5f,  2.0f, -2.5f),
 // glm::vec3(1.5f,  0.2f, -1.5f),
 // glm::vec3(-1.3f,  1.0f, -1.5f)
	//};
	//��������
	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	//����ͼƬ�ļ���
		//��һ��ͼƬ
	unsigned int texture1, texture2;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./res/texture/bin.jpg", &width, &height, &nrChannels, 0);
		//��һ������Ĵ���
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//������Сʹ���ڽ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//���Ŵ�ʱʹ�����Թ���
		//��һ������ͼƬ���ݴ���
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//ͼƬ���ݼ���
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenerateMipmap(GL_TEXTURE_2D);//������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
	}
		//�ͷŵ�һ������ͼƬ
	stbi_image_free(data);
	//�ڶ���ͼƬ

	data = stbi_load("./res/texture/haozi1.jpg", &width, &height, &nrChannels, 0);
		//�ڶ�������Ĵ���
	glGenTextures(1, &texture2);
		//�ڶ�������Ĵ���
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//������Сʹ���ڽ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//���Ŵ�ʱʹ�����Թ���
		//�ڶ�������ͼƬ���ݴ���
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//ͼƬ���ݼ���
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenerateMipmap(GL_TEXTURE_2D);//������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
	}
		//�ͷŵڶ�������ͼƬ
	stbi_image_free(data);
	//����VBO��VAO��EBO��texture
	//���ߵĴ���
	unsigned int VAO, VBO, EBO, lightVAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//���ߵİ�
		//1.��������İ�
	glBindVertexArray(VAO);
		//2.���㻺��İ��Լ����ݰ�
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//3.��������İ��Լ����ݰ�
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//����GPU��ôȡ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	//�����»�һ����Դ
	Shader shader_light("./res/shaders/Light.shader");
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);//���
	//glBindVertexArray(0);//���
	//ʹ��uniform
	//shader.use();
	//shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//shader.setInt((shader.ID, "texture1"), 0);
	//shader.setInt((shader.ID, "texture2"), 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//���漸�еĴ����ʵ����һ���򵥵���Ⱦѭ����
	while (!glfwWindowShouldClose(window))
	{
		//std::cout << "Current path is " << std::filesystem::current_path() << '\n';
		//����
		processInput(window);//����Ƿ���esc��û�а��Ͳ����window����Ϊ��Ҫ�ر�
		ImGui_ImplGlfwGL3_NewFrame();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			/*static float f = 0.0f;*/
			static int counter = 0;
			ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			ImGui::Checkbox("Another Window", &show_another_window);
			if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		//�Ȼ���Ʒ
		shader.use();
		shader.setFloat("ambientStrength", ambientStrength);
		shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		std::cout << cameraPos.x << std::endl;
		std::cout << cameraPos.y << std::endl;
		std::cout << cameraPos.z << std::endl;
		shader.setFloat("specularStrength", specularStrength);
		lightPos.x = 1.0f + glm::sin(glfwGetTime()) * 2;
		lightPos.y = glm::sin(glfwGetTime() / 2.0f);
		shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		glm::mat4 model;
		model = glm::mat4(1.0f);
		glm::mat4 view;
		//float radius = 10.0f;
		//float x = sin(glfwGetTime()) * radius;
		//float z = cos(glfwGetTime()) * radius;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//�ڶ���������float����
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//�ڶ�����Դ
		shader_light.use();
		shader_light.setMat4("projection", projection);
		shader_light.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		shader_light.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//������ɫ��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//shader.use();
		//shader.setFloat("mixValue", mixValue);
		//glBindVertexArray(VAO);
		//����ʱ����ת
		//����mvp�����Լ�uniform
		//glm::mat4 model;
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//���������
		//glm::mat4 view;
		////float radius = 10.0f;
		////float x = sin(glfwGetTime()) * radius;
		////float z = cos(glfwGetTime()) * radius;
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//glm::mat4 projection;
		//projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//�ڶ���������float����
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//������indices������ͼ
		//glDrawArrays(GL_TRIANGLES, 0, 36);//����indices��
		//for (unsigned int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat2(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);//����indices��
		//}
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);//����indices��
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		//��Ⱦָ��
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//��鲢�����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();//�൱���ͷ�������Դ

	return 0;
}