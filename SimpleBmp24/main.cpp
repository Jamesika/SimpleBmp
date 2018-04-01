#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include "SimpleBmp24.h"
#include "PerlinNoise.hpp"

#pragma region ����ָ��, �ı����������ɫ
// ���
void TransformToGray(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	r = g = b = (r + g + b)*0.33f;
}
// ��ֵ
void TransformByThreshold(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	unsigned int max = (r + g + b)*0.33f;
	if (max > 167)
		r = g = b = 255;
	else
		r = g = b = 0;
}
// ��������
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
// 3.2.1 ͼ��ת
void TransformInversion(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	r = 256 - 1 - r;
	g = 256 - 1 - g;
	b = 256 - 1 - b;
}
// 3.2.2 �����任
void TransformLog(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	r = 256 * (log(r + 1) / log(256));
	g = 256 * (log(g + 1) / log(256));
	b = 256 * (log(b + 1) / log(256));
}
// 3.2.3 ����(٤��)�任
void TransformGama(unsigned char& r, unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	float gama = 0.5f;
	r = 255 * pow((float)r/255, gama);
	g = 255 * pow((float)g/255, gama);
	b = 255 * pow((float)b/255, gama);
}
#pragma endregion

#pragma region ������
// 3.2.4 ������, ����ƽ��ֲ�
std::vector<SimpleBmp24*>* BitLayers(SimpleBmp24& bmp, int layerCount = 8)
{
	using namespace std;
	vector<SimpleBmp24*> *layers = new vector<SimpleBmp24*>();
	int width = 0;
	int height = 0;
	bmp.GetSize(width, height);
	for (int i = 0; i < layerCount; i++)
	{
		SimpleBmp24* temp = new SimpleBmp24(width, height);
		layers->push_back(temp);
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int bitIndex = 0; bitIndex < layerCount; bitIndex++)
			{
				int r = 0, g = 0, b = 0;
				bmp.GetPixel(i, j, r, g, b);
				int gray = (r + g + b) / 3;
				if ((gray&(1 << bitIndex)) != 0)
					(*layers)[bitIndex]->SetPixel(i, j, 255, 255, 255);
				else 
					(*layers)[bitIndex]->SetPixel(i, j, 0, 0, 0);
			}
		}
	}
	return layers;
}
// 3.3.1 ֱ��ͼ����(��RGB����ͨ���ֱ�ִ��)
void HistogramEqualization(SimpleBmp24& bmp)
{
	int count[3][256];
	for (int i = 0; i < 256; i++)
	{
		count[0][i] = 0;
		count[1][i] = 0;
		count[2][i] = 0;
	}
	int width = 0;
	int height = 0;
	bmp.GetSize(width, height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int rgb[3] = { 0 };
			bmp.GetPixel(i, j, rgb[0], rgb[1], rgb[2]);
			for (int rgbIndex = 0; rgbIndex < 3; rgbIndex++)
			{
				for (int colorIndex = rgb[rgbIndex]; colorIndex < 256; colorIndex++)
					count[rgbIndex][colorIndex]++;
			}
			
		}
	}
	int bmpSize = width * height;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int r = 0, g = 0, b = 0;
			bmp.GetPixel(i, j, r, g, b);
			r = 255 * (float)count[0][r] / (float)bmpSize;
			g = 255 * (float)count[1][g] / (float)bmpSize;
			b = 255 * (float)count[1][b] / (float)bmpSize;
			bmp.SetPixel(i, j, r,g,b);
		}
	}
		
}
#pragma endregion

#pragma region ͼ��Ŵ�, ��С
SimpleBmp24& ZoomNearest(SimpleBmp24& bmp, float scale = 0.5f)
{
	int width = 0;
	int height = 0;
	bmp.GetSize(width, height);
	int scaledWidth = width * scale;
	int scaledHeight = height * scale;
	SimpleBmp24 *newBmp = new SimpleBmp24(scaledWidth, scaledHeight);
	for (int i = 0; i < scaledWidth; i++)
	{
		for (int j = 0; j < scaledHeight; j++)
		{
			int r = 0;
			int g = 0;
			int b = 0;
			bmp.GetPixel(i/scale,j/scale,r,g,b);
			newBmp->SetPixel(i, j, r, g, b);
		}
	}
	return *newBmp;
}

// Bi,TriҪ�ⷽ��...
/*
SimpleBmp24& ZoomBilinear(SimpleBmp24& bmp, float scale = 0.5f)
{
	int width = 0;
	int height = 0;
	bmp.GetSize(width, height);
	int scaledWidth = width * scale;
	int scaledHeight = height * scale;
	SimpleBmp24 *newBmp = new SimpleBmp24(scaledWidth, scaledHeight);
	for (int i = 0; i < scaledWidth; i++)
	{
		for (int j = 0; j < scaledHeight; j++)
		{
			int r = 0;
			int g = 0;
			int b = 0;
			bmp.GetPixel(i / scale, j / scale, r, g, b);
			newBmp->SetPixel(i, j, r, g, b);
		}
	}
	return *newBmp;
}
SimpleBmp24& ZoomTrilinear(SimpleBmp24& bmp, float scale = 0.5f)
{
	return bmp;
}
*/
#pragma endregion


int main()
{
	using namespace std;
	//==============================================================
	// ����һ���µ�Bmp
	//SimpleBmp24 *bmp = new SimpleBmp24(512,512);
	// ��ȡBmp
	SimpleBmp24 *bmp = SimpleBmp24::Read("Night.bmp");
	//==============================================================
	// 1. ��������
	//bmp->Transform(RandomNoise);
	// 2. ���
	// bmp->Transform(TransformToGray);
	// 3. ��ֵ
	//bmp->Transform(TransformByThreshold);
	// 4. ���ڽ��ڲ�
	// auto newBmp = ZoomNearest(*bmp, 0.1f);
	// 5. ��ת
	// bmp->Transform(TransformInversion);
	// 6. �����任
	// bmp->Transform(TransformLog);
	// 7. ����(٤��)�任
	// bmp->Transform(TransformGama);
	// 8. ����ƽ��ֲ�
	// auto layers = BitLayers(*bmp,8);
	// 9. ֱ��ͼ����
	HistogramEqualization(*bmp);
	//==============================================================
	bmp->Save("Night_HistogramEqualization.bmp");
	//==============================================================
	return 0;
}