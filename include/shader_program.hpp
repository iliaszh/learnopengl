#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <format>
#include <array>
#include <filesystem>

#include <glad/glad.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vsPath, const char* fsPath)
	{
		const auto vsSource = load_source(vsPath);
		const auto* vsSourceStr = vsSource.c_str();

		const auto fsSource = load_source(fsPath);
		const auto* fsSourceStr = fsSource.c_str();

		const auto vertexShader = compile_shader(
			vsSourceStr, GL_VERTEX_SHADER);

		const auto fragmentShader = compile_shader(
			fsSourceStr, GL_FRAGMENT_SHADER);

		program_id = glCreateProgram();
		glAttachShader(program_id, vertexShader);
		glAttachShader(program_id, fragmentShader);

		GLint link_status{};
		glLinkProgram(program_id);
		glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
		if (link_status == GL_FALSE)
		{
			std::array<char, MAX_LOG_SIZE_BYTES> link_log{};

			glGetShaderInfoLog(program_id, link_log.size(),
			                   nullptr, link_log.data());

			std::cerr << "Failed to link the shader program.\n";

			throw std::runtime_error(link_log.data());
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	[[nodiscard]] auto get_id() const -> GLuint { return program_id; }

	auto use() const -> void
	{
		glUseProgram(program_id);
	}

	void set_bool(const std::string& name, const bool value) const
	{
		const auto uniform_id = get_uniform_location(name);

		glUniform1i(uniform_id, static_cast<int>(value));
	}

	void set_int(const std::string& name, const int value) const
	{
		const auto uniform_id = get_uniform_location(name);

		glUniform1i(uniform_id, value);
	}

	void set_float(const std::string& name, const float value) const
	{
		const auto uniform_id = get_uniform_location(name);

		glUniform1f(uniform_id, value);
	}

private:
	constexpr static auto MAX_LOG_SIZE_BYTES = 512;

	[[nodiscard]] auto get_uniform_location(
		const std::string& name) const -> GLint
	{
		const GLint uniform_id = glGetUniformLocation(
			program_id, name.c_str());
		if (uniform_id == -1)
		{
			std::cerr << std::format(
				"Failed to get '{}' uniform variable's location.\n",
				name);
		}

		return uniform_id;
	}

	static auto compile_shader(const char* source,
	                           const GLenum shader_type) -> GLuint
	{
		const GLuint shader = glCreateShader(shader_type);

		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint success{};
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			std::array<char, MAX_LOG_SIZE_BYTES> compileLog{};

			glGetShaderInfoLog(shader, compileLog.size(),
			                   nullptr, compileLog.data());

			std::cerr << std::format(
				"Failed to compile the {} shader.\n",
				shader_type_name(shader_type));

			throw std::runtime_error(compileLog.data());
		}

		return shader;
	}

	static auto load_source(const char* filepath) -> std::string
	{
		const auto file_size = std::filesystem::file_size(filepath);

		std::string source(file_size, '\0');

		std::ifstream file{};
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			file.open(filepath);
			file.read(source.data(),
			          static_cast<std::streamsize>(file_size));
			file.close();
		}
		catch (const std::ifstream::failure&)
		{
			std::cerr << std::format(
				"Failed to read shader source file. Path: {}\n",
				filepath);

			throw;
		}

		return source;
	}

	static auto shader_type_name(
		const GLenum shader_type) -> std::string_view
	{
		switch (shader_type)
		{
		case GL_VERTEX_SHADER:
			return "vertex";
		case GL_FRAGMENT_SHADER:
			return "fragment";
		}

		return "unknown";
	}

	GLuint program_id{};
};

#endif
