#include <iostream>
#include <opencv2/opencv.hpp>
#include <direct.h>
#include <stdlib.h>
#include <string>
#include <opencv2/core/utils/logger.hpp>

using namespace std;
using namespace cv;


int main()
{
	cout << "Hello World!" << endl;
	cout << "有图像显示的时候请按下键盘按键!" << endl;
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);//只输出错误日志
	char buffer[128] = { 0 };   //当前运行路径
	if (_getcwd(buffer, sizeof(buffer)) != buffer)
	{
		cout << "Open File Err" << endl;
		return -1;
	}
	string imagePath = string(buffer) + "\\demo1.bmp";
	cout << "Image Path = " << imagePath << endl;
	//读取图像数据
	Mat imageMat = imread(imagePath.c_str());
	//显示图像数据
	namedWindow("ImageOut", WINDOW_NORMAL);
	imshow("ImageOut", imageMat);
	waitKey(0);  //等待按键按下
	//Mat转HIK ImageBaseData
	/*  我们这边会提供的数据是：
	*  图像宽度：ImageWidth
	*  图像高度：ImageHeight
	*  图像数据：imageData--3通道的话会将R与B交互后的的数据发送
	*/
	int dataLen = imageMat.rows * imageMat.cols * imageMat.channels();
	int ImageWidth = imageMat.cols;
	int ImageHeight = imageMat.rows;

	uchar * imageData = (uchar *)malloc(dataLen);
	memcpy(imageData, imageMat.data, dataLen);  //拷贝数据---imageData
	cout << "Copy Success!" << endl;
	//显示一下拷贝后的图像数据
	Mat imageOut = Mat(ImageHeight, ImageWidth, imageMat.type(), imageData);
	namedWindow("ImageCopy", WINDOW_NORMAL);
	imshow("ImageCopy", imageOut);
	waitKey(0);  //等待按键按下

	//转化成HIK数据  ImageBaseData
	if (1 == imageMat.channels())
	{
		//imageBaseData = new ImageBaseData(imageData, dataLen, matInputImg.Width, matInputImg.Height, (int)VMPixelFormat.VM_PIXEL_MONO_08);
	}
	else if (3 == imageMat.channels())
	{
		//交换R与B通道
		for (int i = 0; i < dataLen - 2; i += 3)
		{
			uchar temp = imageData[i];
			imageData[i] = imageData[i + 2];
			imageData[i + 2] = temp;
		}
		//imageBaseData = new ImageBaseData(imageData, dataLen, imageMat.Width, imageMat.Height, (int)VMPixelFormat.VM_PIXEL_RGB24_C3);
	}

	//保存图像数据
	string exportPath = string(buffer) + "\\out.bmp";
	imwrite(exportPath, imageOut);
	cout << "End Test" << endl;
	return 0;
}
