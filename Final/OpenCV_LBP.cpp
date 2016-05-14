/* I left Philipp Wagner's copyright in this code because I started with
his code to learn and get an understanding of what was happening for EigenFaces.
However the only part here that is really his code are a few variable
names and the read_csv function.
*/

/*
* Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
* Released to public domain under terms of the BSD Simplified license.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the organization nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*   See <http://www.opensource.org/licenses/bsd-license>
*/

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace face;
using namespace std;

// Philipp Wagner's read function
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int main(int argc, char *argv[]) {
	int index;
	int KNN;
	char* temp;
	// Check for valid command line arguments, print info
	// if no/invalid arguments were given.
	// There are 400 possible indexes so anything greater
	// than would cause an error. Enforce that selection here.
	// If you add to the dataset, this number will need updating.

	
	if (argc != 3 && (strtol(argv[1], &temp, 10) < 400)) {
	  cout << "Invalid number of arguments" << endl;
	  cout << "Call FacePrediction.exe with only 1 integer as an argument up to 399." << endl;
	  exit(1);
	}

	if (argc == 3) {
	  //Resolve Argument
	  index = strtol(argv[1], &temp, 10);
	  KNN = strtol(argv[2], &temp, 10);
	}
	
	// This line for Debug
	//index = 345;
	//KNN = 8;

	// CSV file provided here.
	string fn_csv = string("C:/Users/TVita/Documents/DSE project/Faces/FacesSet.csv");
	// These vectors will hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try {
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		exit(1);
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
	}
	// The following lines simply get the selected image from
	// your dataset and remove it from the vector. This is
	// done, so that the training data (which we learn the
	// BasicFaceRecognizer on) and the test data we test
	// the model with, do not overlap.

	cout << "Index Value " << index << " Corresponds to class labed " << labels[index] << endl;
	// Save test image out
	Mat testSample = images[index];
	int testLabel = labels[index];

	// push back all images from the selected image to the end of the vector.
	for (int i = index; i < images.size(); i++){
		images[index] = images[index + 1];
		labels[index] = labels[index + 1];
	}
	// Pop the last element.
	images.pop_back();
	labels.pop_back();
	// Image selected has been removed from training data and vector integrity remains for training.

	cout << "Training Model for prediction" << endl;

	Ptr<LBPHFaceRecognizer> model = createLBPHFaceRecognizer(8, KNN);
	model->train(images, labels);

	// The following lines predict the label of a given test image and writes it out.
	int predictedLabel = model->predict(testSample);
	cout << "Predicted class = " << predictedLabel << endl;


	cout << "KNN = " << model->getNeighbors() << endl;
	
	vector<Mat> histograms = model->getHistograms();
	cout << "Size of Histogram = " << histograms[0].total() << endl;

	return 0;
}