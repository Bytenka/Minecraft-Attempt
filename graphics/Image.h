#pragma once

#include <string>

namespace tk
{
class Image
{
  public:
	Image(const std::string &imgPath);
	~Image() noexcept;

	inline int getNbrChannels() const noexcept { return m_nbrChannels; };
	inline int getWidth() const noexcept { return m_width; };
	inline int getHeight() const noexcept { return m_height; };
	inline const unsigned char *getData() const noexcept { return m_data; };
	inline std::string getName() const noexcept { return m_imgPath.substr(0, m_imgPath.find(".") - 1); }

  private:
	void loadData();

	int m_width, m_height, m_nbrChannels;
	unsigned char *m_data = NULL;
	std::string m_imgPath;
};
} // namespace tk