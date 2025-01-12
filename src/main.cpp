#include <array>
#include <iostream>

#include <glad/glad.h> // should be included before glfw

#include <GLFW/glfw3.h>

#include "shader_program.hpp"

namespace {
auto process_input(GLFWwindow* window) -> void {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		return;
	}

	glfwSetWindowShouldClose(window, GLFW_TRUE);
}
} // namespace

auto render_loop(GLFWwindow* window) -> int;

auto main() -> int {
	if (glfwInit() == 0) {
		std::cerr << "Failed to initialize GLFW.\n";
	}

	constexpr int screen_width  = 800;
	constexpr int screen_height = 600;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		screen_width, screen_height,
		"learnopengl", nullptr, nullptr
		);

	if (window == nullptr) {
		std::cerr << "Failed to create a GLFW window.\n";

		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)) == 0) {
		std::cerr << "Failed to initialize GLAD.\n";

		glfwTerminate();

		return -1;
	}

	constexpr auto framebuffer_size_callback =
		[](GLFWwindow* /*window*/,
		   const int width, const int height) -> void {
		glViewport(0, 0, width, height);
	};

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	try {
		render_loop(window);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	}
}

auto render_loop(GLFWwindow* window) -> int {
	GLuint vao{};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// from this point VAO stores:
	// - calls to glEnableVertexAttribArray, glDisableVertexAttribArray
	// - vertex attribute configurations via glVertexAttribPointer
	// - vertex buffer objects associated with vertex attributes by calls
	// to glVertexAttribPointer.

	constexpr std::array vertices = { +0.5F, +0.5F, +0.0F,
	                                  -0.5F, +0.5F, +0.0F,
	                                  -0.5F, -0.5F, +0.0F };
	GLuint vbo{}; // vertex buffer object
	glGenBuffers(1 /* number of buffers */, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(vertices), vertices.data(),
	             GL_STATIC_DRAW);

	glVertexAttribPointer(0 /* location from vs */,
	                      3 /* bytes per vertex */,
	                      GL_FLOAT,
	                      GL_FALSE,
	                      3 * sizeof(float) /* space between vertices */,
	                      nullptr /* position data location */);
	glEnableVertexAttribArray(0 /* location from vs */);

	const ShaderProgram shader_program(
		"../shaders/vertex.glsl",
		"../shaders/fragment.glsl");
	shader_program.use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// the render loop
	while (glfwWindowShouldClose(window) == 0) {
		process_input(window);

		glClearColor(0.2F, 0.2F, 0.2F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(
			GL_TRIANGLES,
			0,
			3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
