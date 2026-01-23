#pragma once

enum class BufferTarget : uint8_t
{
	Array,
	ElementArray,
	Uniform
};

enum class BufferUsage : uint8_t
{
	StaticDraw,
	DynamicDraw,
	StreamDraw,
	StaticRead,
	DynamicRead,
	StreamRead,
	StaticCopy,
	DynamicCopy,
	StreamCopy
};

GLuint GetCurrentBuffer(BufferTarget target);

struct BufferHandle final { GLuint handle{ 0u }; };

BufferHandle CreateBuffer(BufferTarget target, BufferUsage usage, size_t size, const void* data);
void BufferSubData(BufferHandle bufferId, BufferTarget target, GLintptr offset, GLsizeiptr size, const void* data);