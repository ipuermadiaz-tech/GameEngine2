#pragma once
#include "Resource.h"
#include <string> // Standard library header must come before SDL

struct TTF_Font;

namespace nu {
	class Font : public Resource {
	public:
		Font() = default;
		~Font();

		bool Load(const std::string& name, float fontSize);

	private:
		friend class Text;

		TTF_Font* m_ttfFont{ nullptr };
	};


}