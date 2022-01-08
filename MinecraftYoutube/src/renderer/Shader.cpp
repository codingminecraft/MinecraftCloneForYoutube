#include "renderer/Shader.h"

namespace MinecraftClone
{
	// Forward Declarations
	static std::string ReadFile(const char* filepath);

	bool Shader::compile(ShaderType type, const char* shaderFilepath)
	{
		// Copy the shader filepath into a string
		int strLength = (int)std::strlen(shaderFilepath);
		filepath = (char*)g_memory_allocate(sizeof(char) * (strLength + 1));
		std::strcpy(filepath, shaderFilepath);
		filepath[strLength] = '\0';
		g_logger_info("Compiling shader: %s", filepath);

		// Read the shader source code from the file
		std::string shaderSourceCode = ReadFile(filepath);

		GLenum shaderType = toGlShaderType(type);
		if (shaderType == GL_INVALID_ENUM) {
			g_logger_error("ShaderType is unknown");
			return false;
		}

		// Create an empty shader handle
		shaderId = glCreateShader(shaderType);

		// Send the shader source code to GL
		const GLchar* sourceCStr = shaderSourceCode.c_str();
		glShaderSource(shaderId, 1, &sourceCStr, 0);

		// Compile the shader
		glCompileShader(shaderId);

		// Check if the compilation succeeded
		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// If compilation failed, find out why and log the error
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore if compilation failed
			glDeleteShader(shaderId);

			g_logger_error("Shader Compilation failed: \n%s", infoLog.data());

			shaderId = UINT32_MAX;
			return false;
		}
		
		return true;
	}

	void Shader::destroy()
	{
		if (shaderId != UINT32_MAX)
		{
			glDeleteShader(shaderId);
			shaderId = UINT32_MAX;
		}

		if (filepath != nullptr)
		{
			g_memory_free(filepath);
			filepath = nullptr;
		}
	}


	GLenum Shader::toGlShaderType(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		}
		return GL_INVALID_ENUM;
	}

	static std::string ReadFile(const char* filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			g_logger_error("Could not open file: '%s'", filepath);
		}

		return result;
	}
}