#pragma once

#include <memory>
#include <W_Engine/Model.h>

namespace W_Engine
{
	class ResourceManager
	{
		static Model LoadModel(const std::string& filepath);
	};
}