#pragma once

class Transformation
{
public:
	Transformation() = default;
	Transformation(const glm::vec3& position, const glm::quat& rotation = glm::quat(1, 0, 0, 0));
	Transformation(const glm::mat4& matrix);

	glm::vec3 Forward() const;
	glm::vec3 Back() const;
	glm::vec3 Left() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;
	glm::vec3 Down() const;

	glm::mat4 Matrix(const glm::vec3& scalefactor = glm::vec3(1, 1, 1)) const;
	glm::mat4 LookAt() const;

	void Move(const glm::vec3& direction);

	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);

	static Transformation Interpolate(
		const Transformation& transformation1,
		const Transformation& transformation2,
		float weight);

	glm::vec3 Position = { 0, 0, 0 };
	glm::quat Rotation;

};