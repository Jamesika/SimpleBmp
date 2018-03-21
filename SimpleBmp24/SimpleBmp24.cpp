#include "SimpleBmp24.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <regex>

SimpleBmp24* SimpleBmp24::Read(std::string filePath)
{
	using namespace std;
	ifstream fin;
	fin.open(filePath, ifstream::binary);
	if (!fin.is_open())
	{
		cout << "Unable to open : " + filePath;
		return nullptr;
	}
	fin.seekg(18, ios::beg);
	unsigned int size = 0;
	fin.read((char*)(&size), 4);
	int width = size;
	fin.read((char*)(&size), 4);
	int height = size;
	
	SimpleBmp24* newBmp = new SimpleBmp24(width, height);
	
	fin.seekg(54, ios::beg);
	int space = (4 - (width * 3) % 4) % 4;
	for (int i = 0; i < height; i++)
	{
		fin.read((char*)(&newBmp->data[i*width * 3]), width * 3 * sizeof(char));
		fin.seekg(space, ios::cur);
	}
	fin.seekg(0, ios::beg);
	fin.close();
	return newBmp;
}
void SimpleBmp24::Save(SimpleBmp24& bmp, std::string filePath)
{
	using namespace std;
	ofstream fout;
	fout.open(filePath);
	if (!fout.is_open())
	{
		cout << "Fail save!";
		return;
	}
	// 4×Ö½Ú¶ÔÆë
	int space = (4 - (bmp.width * 3) % 4) % 4;
	// Bitmap File Header  
	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof((bmp.width * 3 +space)*bmp.height);
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Bitmap Info Header  
	BITMAPINFOHEADER bitmapHeader = { 0 };
	bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapHeader.biHeight = bmp.height;
	bitmapHeader.biWidth = bmp.width;
	bitmapHeader.biPlanes = 1;
	bitmapHeader.biBitCount = 24;
	bitmapHeader.biSizeImage = bmp.width*bmp.height*3;

	fout.write((char*)(&fileHeader), sizeof(fileHeader));
	fout.write((char*)(&bitmapHeader), sizeof(bitmapHeader));

	for (int i = 0; i < bmp.height; i++)
	{
		fout.write((char*)(&bmp.data[i*bmp.width * 3]), bmp.width * 3);
		fout.write("\0\0\0\0",space);
	}
	fout.close();
}

SimpleBmp24::SimpleBmp24(int width, int height)
{
	this->width = width;
	this->height = height;
	this->data = new unsigned char[width * height * 3];
}
void SimpleBmp24::Save(std::string filePath)
{
	Save(*this, filePath);
}
void SimpleBmp24::Transform(ColorTransform transform)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int index = GetIndexOfPos(i, j);
			transform(data[index], data[index + 1], data[index + 2], j, i, width, height);
		}
}
int SimpleBmp24::GetIndexOfPos(int row, int line)
{
	return 3 * (row*width + line);
}
SimpleBmp24::~SimpleBmp24()
{
	delete[]data;
}