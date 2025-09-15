#pragma once

struct RenderCreateInfo final
{
};

class Render final
{
public:
	bool Init(const RenderCreateInfo& createInfo);
	void Close();
};