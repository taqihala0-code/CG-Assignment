#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	// ---------------------------
	// 1️⃣ Initialize GLFW
	// ---------------------------
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}

	// Set OpenGL version 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ---------------------------
	// 2️⃣ Create Window
	// ---------------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Modern", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// ---------------------------
	// 3️⃣ Initialize GLEW
	// ---------------------------
	glewExperimental = GL_TRUE; // مهم مع Core Profile
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW\n";
		glfwTerminate();
		return -1;
	}

	// ---------------------------
	// 4️⃣ Render Loop
	// ---------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Clear screen with a color
		glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ---------------------------
	// 5️⃣ Cleanup
	// ---------------------------
	glfwTerminate();

	return 0;
}
