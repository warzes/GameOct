#include "stdafx.h"
#include "NanoMath.h"
//=============================================================================
#define DEFAULT_FORWARD		(glm::vec3(+0, +0, -1))
#define DEFAULT_UP			(glm::vec3(+0, +1, +0))
#define DEFAULT_RIGHT		(glm::vec3(+1, +0, +0))
//=============================================================================
Transformation::Transformation(const glm::vec3& position, const glm::quat& rotation) : Position(position), Rotation(rotation)
{
}
//=============================================================================
Transformation::Transformation(const glm::mat4& matrix)
{
	Position = glm::vec3(matrix * glm::vec4(0, 0, 0, 1));
	Rotation = glm::toQuat(matrix);
}
//=============================================================================
glm::vec3 Transformation::Forward() const
{
	return glm::rotate(Rotation, DEFAULT_FORWARD);
}
//=============================================================================
glm::vec3 Transformation::Back() const
{
	return -glm::rotate(Rotation, DEFAULT_FORWARD);
}
//=============================================================================
glm::vec3 Transformation::Left() const
{
	return -glm::rotate(Rotation, DEFAULT_RIGHT);
}
//=============================================================================
glm::vec3 Transformation::Right() const
{
	return glm::rotate(Rotation, DEFAULT_RIGHT);
}
//=============================================================================
glm::vec3 Transformation::Up() const
{
	return glm::rotate(Rotation, DEFAULT_UP);
}
//=============================================================================
glm::vec3 Transformation::Down() const
{
	return -glm::rotate(Rotation, DEFAULT_UP);
}
//=============================================================================
glm::mat4 Transformation::Matrix(const glm::vec3& scalefactor) const
{
	return glm::translate(Position) * glm::toMat4(Rotation) * glm::scale(scalefactor);
}
//=============================================================================
glm::mat4 Transformation::LookAt() const
{
	return glm::inverse(Matrix());
}
//=============================================================================
void Transformation::Move(const glm::vec3& direction)
{
	Position += direction;
}
//=============================================================================
void Transformation::Pitch(float angle)
{
	Rotation = glm::rotate(Rotation, angle, DEFAULT_RIGHT);
}
//=============================================================================
void Transformation::Yaw(float angle)
{
	Rotation = glm::rotate(Rotation, angle, DEFAULT_UP);
}
//=============================================================================
void Transformation::Roll(float angle)
{
	Rotation = glm::rotate(Rotation, angle, DEFAULT_FORWARD);
}
//=============================================================================
Transformation Transformation::Interpolate(const Transformation& transformation1, const Transformation& transformation2, float weight)
{
	return {
		glm::lerp(transformation1.Position, transformation2.Position, weight),
		glm::slerp(transformation1.Rotation, transformation2.Rotation, weight) };
}
//=============================================================================