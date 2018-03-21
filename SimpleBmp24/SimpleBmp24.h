#pragma once
#include <string>

// 输入颜色, 像素坐标, 图像大小
using ColorTransform = void (unsigned char& r,unsigned char& g, unsigned char& b, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

class SimpleBmp24
{
private:
	int width;
	int height;
	unsigned char* data;

	SimpleBmp24()
	{}
public:
	// 读取路径, 获得文件名, 所在文件夹
	static SimpleBmp24* Read(std::string filePath);
	static void Save(SimpleBmp24& bmp, std::string filePath);
public:
	// 创建!
	SimpleBmp24(int width, int height);
	// 储存!
	void Save(std::string filePath);
	// 变换!
	void Transform(ColorTransform transform);
	// 获得相应位置的下标, row, 行; line, 列
	int GetIndexOfPos(int row, int line);

	~SimpleBmp24();
};

