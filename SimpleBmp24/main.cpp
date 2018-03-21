#include <iostream>
#include "SimpleBmp24.h"
#include "PerlinNoise.hpp"

// 函数指针, 用于Bmp图像变换
void TransformToGray(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	r = g = b = (r + g + b)*0.33f;
}
void TransformByThreshold(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	unsigned int max = (r + g + b)*0.33f;
	if (max > 167)
		r = g = b = 255;
	else
		r = g = b = 0;
}
void RandomNoise(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	// 所有的像素不是一起处理的, 为了连续性需要static
	static siv::PerlinNoise perlinNoise;
	float dx[] = { 1, 1, 1};
	float dy[] = { 1, 1, 1 };
	float sc[] = { 0.02f ,0.02f, 0.02f};
	unsigned char* rgb[] = { &r,&g,&b };
	
	for (int i = 0; i < 3; i++)
		*rgb[i] = (unsigned char)(perlinNoise.noise0_1(x*sc[i]+dx[i], y*sc[i]+dy[i]) * 255);
}

int main()
{
	using namespace std;
	SimpleBmp24 *bmp = new SimpleBmp24(512,512);

	// 柏林噪音
	bmp->Transform(RandomNoise);
	
	// 变灰
	//bmp->Transform(TransformToGray);
	
	// 阈值
	//bmp->Transform(TransformByThreshold);

	bmp->Save("Newbmp.bmp");
	
	return 0;
}