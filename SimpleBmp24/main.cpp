#include <iostream>
#include "SimpleBmp24.h"
#include "PerlinNoise.hpp"

// ����ָ��, ����Bmpͼ��任
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
	// ���е����ز���һ�����, Ϊ����������Ҫstatic
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

	// ��������
	bmp->Transform(RandomNoise);
	
	// ���
	//bmp->Transform(TransformToGray);
	
	// ��ֵ
	//bmp->Transform(TransformByThreshold);

	bmp->Save("Newbmp.bmp");
	
	return 0;
}