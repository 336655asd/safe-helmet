#include "dector.h"
#include <QDebug>
Dector::Dector(Mat mat)
{
    image=mat;
    warning=0;
}
//行人检测
int Dector::decte()
{
    HOGDescriptor defaultHog;
    defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    defaultHog.detectMultiScale(image, people,0,Size(8,8),Size(0,0),1.03,2);
    return people.size();
}
//作图
Mat Dector::draw()
{
    warning=0;
    for (int i = 0; i < people.size(); i++)
    {
        bool flag=true;
        Rect r = people[i];
        Mat red= image(r);
        flag=FindRed(red);
        if(flag==true)
        {
            rectangle(image, r.tl(), r.br(), Scalar(0, 255, 0), 3);
        }
        else
        {
            rectangle(image, r.tl(), r.br(), Scalar(0, 0, 255), 3);
            warning++;
        }

    }

    return image;
}

//安全帽检测
bool Dector:: FindRed(const Mat &inputImage){
    int count=0;
    Mat hsvImage;
    cvtColor(inputImage, hsvImage, CV_BGR2HSV);
    double H=0.0,S=0.0,V=0.0;
    for(int i=0;i<hsvImage.rows;i++)
    {
        for(int j=0;j<hsvImage.cols;j++)
        {
            H=hsvImage.at<Vec3b>(i,j)[0];
            S=hsvImage.at<Vec3b>(i,j)[1];
            V=hsvImage.at<Vec3b>(i,j)[2];

            //if(100<S && S<160)
            if(100<S<160)
            {
                if((  H>=0 && H < 5) && V >= 160)
                {
                    count++;
                }

            }
        }
    }
    qDebug()<<count;
    if(count>20)
    {
        return true;
    }
    else
    {
        return false;
    }
}
