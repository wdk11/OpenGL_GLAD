#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//着色器的创建
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

//片段着色器的创建
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
//"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";


int main()
{
	glfwInit();//确定版本，并进行初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//然而，当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用。这个回调函数的原型如下

	//着色器的创建
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//片段着色器的创建
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//顶点输入
	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	//索引输入
	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 2, // 第一个三角形
		//1, 2, 3  // 第二个三角形
	};
	//创建VBO、VAO、EBO
	unsigned int VBO, VAO, EBO;
	//三者的创建
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//三者的绑定
		//1.顶点数组的绑定
	glBindVertexArray(VAO);
		//2.顶点缓冲的绑定以及数据绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//3.索引缓冲的绑定以及数据绑定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//告诉GPU怎么取
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑
	glBindVertexArray(0);//解绑


	////索引缓冲对象
	//unsigned int IBO;
	//glGenBuffers(1, &IBO);
	////绑定索引缓冲对象以及位置
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	////顶点缓冲对象
	//unsigned int VBO;
	//glGenBuffers(1, &VBO);//意思就是把1绑定在VBO上了，缓冲ID为1，是独一无二的
	////顶点数组对象
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);//意思就是把1绑定在VBO上了，缓冲ID为1，是独一无二的
	////绑定顶点数组
	//glBindVertexArray(VAO);
	////确定数组的数据以及位置
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////告诉GPU数据的形式
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);
	////启用0号顶点属性
	//glEnableVertexAttribArray(0);

	//下面几行的代码就实现了一个简单的渲染循环：
	while (!glfwWindowShouldClose(window))
	{
		//输入
		processInput(window);//检查是否按了esc，没有按就不会把window设置为需要关闭
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//启用着色器
		glUseProgram(shaderProgram);
		////使用uniform
		//float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0f + 0.5f;
		//int vertexColorLacation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLacation, 0.0f, greenValue, 0.0f, 1.0f);
		//绑定顶点数组
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		////渲染指令
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();//相当于释放所有资源

	return 0;
}