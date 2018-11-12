#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QTimer>
#include <QImage>
#include "dector.h"
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int num_person;
    //与动作相关的一些标志
    int flag_play;

private slots:
    void nextFrame();

    void LoadFile();


private:
    Ui::MainWindow *ui;
    cv::Mat frame;
    cv::VideoCapture capture;
    QImage  image;
    QImage new_img;
    QTimer *timer;
    Dector *dector;

    QAction *open_action;
    QAction *play_action;
    QAction *stop_action;
    QAction *end_action;



    double rate; //FPS
    cv::VideoWriter writer;   //make a video record
    QImage Mat2QImage(cv::Mat cvImg);


    //函数
    void Play();
    void SetPlay(int flag);
};

#endif // MAINWINDOW_H
