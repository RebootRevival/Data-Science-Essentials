#include <stdio.h>
#include <math.h>
#include <opencv\cv.hpp>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// Create the classifier object 
	CascadeClassifier face_cascade;
	// Uses haarcascade_frontalface_alt2.xml
	if (!face_cascade.load("C:/Users/TVita/Documents/OpenCV3/source/opencv/data/haarcascades/haarcascade_frontalface_alt2.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	// Device ID is 0
	cv::VideoCapture capWebcam(0); 
	if (!capWebcam.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}
	
	vector<Rect> faces;
	// input image
	cv::Mat imgOriginal;	
	// grayscale of input image
	cv::Mat imgGrayscale;		

	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && capWebcam.isOpened())
	{
		// get next frame of cam feed
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);		
		// if frame not read successfully
		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {		
			// print error message to std out and jump out of while loop
			cout << "error: frame not read from webcam\n";		
			break;													
		}
		// convert to grayscale
		cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);	
		// equalize image
		cv::equalizeHist(imgGrayscale, imgGrayscale);
		// apply cascade to grayscale image
		face_cascade.detectMultiScale(imgGrayscale, faces, 1.1, 15, CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(300, 300));
		for (int i = 0; i < faces.size(); i++)
		{
			// iterate through faces matrix to create points around face
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			Point pt2(faces[i].x, faces[i].y);
			Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
			int radius = cvRound((faces[i].width + faces[i].height)*0.25);
			Mat faceROI = imgGrayscale(faces[i]);
			// draw the rectangle and circle on those points
			rectangle(imgOriginal, pt1, pt2, cvScalar(0, 255, 0), 2, 8, 0);
			circle(imgOriginal, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
		// dispaly the cam feed with shapes drawn
		cv::imshow("CamFeed", imgOriginal);		

		charCheckForEscKey = cv::waitKey(1);
	}
	return 0;
}