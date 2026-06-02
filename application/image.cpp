#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include "image.h"

Image::Image(const uint32_t& width, const uint32_t& height, RGBA* data) {
	mWidth = width;
	mHeight = height;
	if (data) {
		mData = new RGBA[mWidth * mHeight];
		memcpy(mData, data, sizeof(RGBA) * mWidth * mHeight);
	}
}

Image::~Image() {
	if (mData != nullptr) {
		delete[] mData;
	}
}

Image* Image::createImage(const std::string& path) {
	int picType = 0;
	int width{ 0 }, height{ 0 };


	stbi_set_flip_vertically_on_load(true);


	unsigned char* bits = stbi_load(path.c_str(), &width, &height, &picType, STBI_rgb_alpha);
	for (int i = 0; i < width * height * 4; i += 4)
	{
		byte tmp = bits[i];
		bits[i] = bits[i + 2];
		bits[i + 2] = tmp;
	}

	Image* image = new Image(width, height, (RGBA*)bits);

	stbi_image_free(bits);

	return image;
}

Image* Image::createImageFromMemory(
	const std::string& path,
	unsigned char* dataIn,
	uint32_t widthIn,
	uint32_t heightIn
) {
	int picType = 0;
	int width{ 0 }, height{ 0 };


	uint32_t dataInSize = 0;


	if (!heightIn) {
		dataInSize = widthIn;
	}
	else {
		dataInSize = widthIn * heightIn;
	}


	unsigned char* bits = stbi_load_from_memory(dataIn, dataInSize, &width, &height, &picType, STBI_rgb_alpha);
	for (int i = 0; i < width * height * 4; i += 4)
	{
		byte tmp = bits[i];
		bits[i] = bits[i + 2];
		bits[i + 2] = tmp;
	}

	Image* image = new Image(width, height, (RGBA*)bits);

	return image;
}

void Image::destroyImage(Image* image) {
	if (image) {
		delete image;
	}
}
