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
	int GetIndexOfPos(int row, int line);

	~SimpleBmp24();
};

