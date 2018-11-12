#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
//在终端编译运行，输出颜色分布直方图
vector<MatND> getHSVHist(Mat &src){

    //输入图片得是三通道彩色图片
    assert (!src.empty() && src.channels() == 3);

    //rgb转hsv图像
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    int hbins = 30;
    int sbins = 51;
    int vbins = 51;
    int hHistSize[] = {hbins};
    int sHistSize[] = {sbins};
    int vHistSize[] = {vbins};

    float hranges[] = {0, 180};
    float sranges[] = {0, 255};
    float vranges[] = {0, 255};
    const float* hRanges[] = {hranges};
    const float* sRanges[] = {sranges};
    const float* vRanges[] = {vranges};
    vector<MatND> hist;

    int hChannels[] = {0};
    int sChannels[] = {1};
    int vChannels[] = {2};
    MatND hHist, sHist, vHist;
    calcHist(&hsv, 1, hChannels, Mat(), hHist, 1, hHistSize, hRanges);
    calcHist(&hsv, 1, sChannels, Mat(), sHist, 1, sHistSize, sRanges);
    calcHist(&hsv, 1, vChannels, Mat(), vHist, 1, vHistSize, vRanges);
    hist.push_back(hHist);
    hist.push_back(sHist);
    hist.push_back(vHist);
    normalize( hist[0], hist[0], 0, 1, NORM_MINMAX, -1, Mat() );
    normalize( hist[1], hist[1], 0, 1, NORM_MINMAX, -1, Mat() );
    normalize( hist[2], hist[2], 0, 1, NORM_MINMAX, -1, Mat() );

    int i;  
    int start = -1, end = -1;
    for(i = 0; i < 30; i++)  
    {  
        float value = hist[0].at<float>(i);
        if (value  > 0)
        {
            if (start == -1)
            {
                start = i;
                end = i;
            }
            else
                end = i;
            cout << "H Value" << i << ": " << value << endl;
        }
        else
        {
            if (start != -1)
                cout <<"H:" <<start*6 <<"~"<<(end+1)*6-1<<endl;
            start = end = -1;
        }
    }  
    if (start != -1)
        cout <<"H:" <<start*5 <<"~"<<(end+1)*5-1<<endl;

    start = -1, end = -1;
    for(i = 0; i < 51; i++)  
    {  
        float value = hist[1].at<float>(i);
        if (value  > 0)
        {
            if (start == -1)
            {
                start = i;
                end = i;
            }
            else
                end = i;
            cout << "S Value" << i << ": " << value << endl;
        }
        else
        {
            if (start != -1)
                cout <<"S:"<< start*5 <<"~"<<(end+1)*5-1<<endl;
            start = end = -1;
        }
    }  
    if (start != -1)
        cout <<"S:" <<start*5 <<"~"<<(end+1)*5-1<<endl;

    start = -1, end = -1;
    for(i = 0; i < 51; i++)  
    {  
        float value = hist[2].at<float>(i);
        if (value  > 0)
        {
            if (start == -1)
            {
                start = i;
                end = i;
            }
            else
                end = i;
            cout << "V Value" << i << ": " << value << endl;
        }
        else
        {
            if (start != -1)
                cout <<"V:" <<start*5 <<"~"<<(end+1)*5-1<<endl;
            start = end = -1;
        }
    }  
    if (start != -1)
        cout <<"V:" <<start*5 <<"~"<<(end+1)*5-1<<endl;

    return hist;
}

int main()
{
Mat src=imread("label.png");
getHSVHist(src);
}
