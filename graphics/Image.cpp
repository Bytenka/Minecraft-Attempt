#include "Image.h"

// Requirered to use stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "../utils/stb_image.h"

#include "../utils/Logging.h"
#include "../utils/Exceptions.h"

namespace tk
{
Image::Image(const std::string &imgPath)
{
	m_imgPath.assign(imgPath);
	try
	{
		loadData();
	}
	catch (RuntimeException &e)
	{
		e.pushCurrentContext(__FUNCTION__);
	}
}

Image::~Image() noexcept
{
	stbi_image_free(m_data);
}

void Image::loadData()
{
	// Discard previous data if existing
	if (m_data != NULL)
	{
		LOG_INFO("Reloading file \"" << m_imgPath << "\"");
		stbi_image_free(m_data);
	}

	//stbi_set_flip_vertically_on_load(true);
	m_data = stbi_load(m_imgPath.c_str(), &m_width, &m_height, &m_nbrChannels, 0);

	if (m_data == NULL)
	{
		throw RuntimeException(__FUNCTION__, "Unable to load file \"" + m_imgPath + "\"!\n\tReason: " + std::to_string(*stbi_failure_reason()));
	}
}
} // namespace tk