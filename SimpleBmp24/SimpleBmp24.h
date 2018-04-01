#pragma once
#include <string>

// ������ɫ, ��������, ͼ���С
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
	// ��ȡ·��, ����ļ���, �����ļ���
	static SimpleBmp24* Read(std::string filePath);
	static void Save(SimpleBmp24& bmp, std::string filePath);
public:
	// ����!
	SimpleBmp24(int width, int height);
	// ����!
	void Save(std::string filePath);
	// �任!
	void Transform(ColorTransform transform);
	// �����Ӧλ�õ��±�, row, ��; line, ��
	bool GetIndexOfPos(int row, int line, int& index);

	bool GetPixel(int x, int y, int& r, int& g, int& b);
	bool SetPixel(int x,int y,int r,int g,int b);

	void GetSize(int& width,int& height)
	{
		width = this->width;
		height = this->height;
	}
	

	~SimpleBmp24();
};

