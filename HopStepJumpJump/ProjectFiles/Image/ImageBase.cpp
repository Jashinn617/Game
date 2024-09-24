#include "ImageBase.h"

ImageBase::ImageBase():
	m_H(-1),
	m_center{0,0},
	m_isDisplay(true)
{
	/*処理無し*/
}

ImageBase::~ImageBase()
{
	DeleteGraph(m_H);
}

void ImageBase::SetUp()
{
	/*処理無し*/
}

void ImageBase::SetUp(const char* fileName, Vec2 center)
{
	m_H = LoadGraph(fileName);
	m_center = center;
}
