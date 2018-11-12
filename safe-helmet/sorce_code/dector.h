#ifndef DECTOR_H
#define DECTOR_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

class Dector
{
public:
    Dector(Mat mat);
    int decte();
    Mat draw();
    Mat image;
    int warning;
private:

    vector<Rect> people;
    bool FindRed(const Mat &inputImage);


};
#endif // DECTOR_H
