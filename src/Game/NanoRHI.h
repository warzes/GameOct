#pragma once

namespace gl
{
	GLuint CreateShaderProgram(std::string_view vertexShader);
	GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view fragmentShader);
	GLuint CreateShaderProgram(std::string_view vertexShader, std::string_view geometryShader, std::string_view fragmentShader);

} // namespace gl