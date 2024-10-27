#include <array>
#include <iostream>

#include <glad/glad.h> // should be included before glfw

#include <GLFW/glfw3.h>

auto process_input(GLFWwindow* window) -> void {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		return;
	}

	glfwSetWindowShouldClose(window, true);
}

auto main() -> int {
	if (glfwInit() == 0) {
		std::cerr << "Failed to initialize GLFW.\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window =
		glfwCreateWindow(800, 600, "learnopengl", nullptr, nullptr);

	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window.\n";

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
		[](GLFWwindow* /*window*/, const int width,
		   const int height) -> void {
		glViewport(0, 0, width, height);
	};

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	constexpr auto vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader_id, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader_id);

	int vs_compile_status = 0;
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vs_compile_status);
	if (vs_compile_status == 0) {
		std::array<char, 512> vs_log{};

		glGetShaderInfoLog(vertex_shader_id, vs_log.size(), nullptr,
				   vs_log.data());

		std::cerr << "While compiling vertex shader:\n"
			  << vs_log.data() << '\n';

		return -1;
	}

	constexpr auto fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment_shader_id, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader_id);

	int fs_compile_status = 0;
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS,
		      &fs_compile_status);
	if (fs_compile_status == 0) {
		std::array<char, 512> fs_log{};

		glGetShaderInfoLog(vertex_shader_id, fs_log.size(), nullptr,
				   fs_log.data());

		std::cerr << "While compiling fragment shader:\n"
			  << fs_log.data() << std::endl;

		return -1;
	}

	const GLuint shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader_id);
	glAttachShader(shader_program, fragment_shader_id);

	glLinkProgram(shader_program);

	int program_link_status = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &program_link_status);
	if (program_link_status == 0) {
		std::array<char, 512> sp_log{};

		glGetProgramInfoLog(shader_program, sp_log.size(), nullptr,
				    sp_log.data());

		std::cerr << "While linking shader program:\n"
			  << sp_log.data() << std::endl;

		return -1;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);


	constexpr std::array<float, 9> vertices = { -0.5f, -0.5f, 0.0f,
						    +0.5f, -0.5f, 0.0f,
						    +0.0f, +0.5f, 0.0f };

	GLuint vertex_buffer_object_id = 0;
	glGenBuffers(1, &vertex_buffer_object_id);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// from this point VAO stores:
	// - calls to glEnableVertexAttribArray, glDisableVertexAttribArray
	// - vertex attribute configurations via glVertexAttribPointer
	// - vertex buffer objects associated with vertex attributes by calls
	// to glVertexAttribPointer.

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_id);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      nullptr);
	glEnableVertexAttribArray(0 /* location */);


	glUseProgram(shader_program);

	// the render loop
	while (glfwWindowShouldClose(window) == 0) {
		process_input(window);

		glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
