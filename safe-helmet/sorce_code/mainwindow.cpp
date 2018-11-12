#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    flag_play=0;
    num_person=0;
    setWindowTitle(tr("安全帽检测"));
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setScaledContents(false);  //fit video to label area

    open_action = new QAction(QIcon(":/images/file-open"),tr("&open..."),this);
    open_action->setShortcut(QKeySequence::Open);
    open_action->setToolTip(tr("打开视频文件"));
    play_action = new QAction(QIcon(":/images/play"),tr("&play..."),this);
    play_action->setToolTip(tr("开始检测"));
    stop_action = new QAction(QIcon(":/images/stop"),tr("&stop..."),this);
    stop_action->setToolTip(tr("暂停"));
    end_action = new QAction(QIcon(":/images/end"),tr("&end..."),this);
    end_action->setToolTip(tr("终止"));


    QToolBar *toolbar = addToolBar(tr("&file"));
    toolbar->addAction(open_action);
    toolbar->addAction(play_action);
    toolbar->addAction(stop_action);
    toolbar->addAction(end_action);

    setCentralWidget(ui->label);
    //槽函数
    connect(open_action,&QAction::triggered,this,&MainWindow::LoadFile);
    connect(play_action,&QAction::triggered,[=]() {
        SetPlay(1);
    });
    connect(stop_action,&QAction::triggered,[=]() {
        SetPlay(0);
    });
    connect(end_action,&QAction::triggered,[=]() {
        SetPlay(2);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    capture.release();
}

///auto get next frame
void MainWindow::nextFrame()
{
    capture >> frame;
    if (!frame.empty() && flag_play==1)
    {
        image = Mat2QImage(frame);
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        timer->stop();
    }

}

void MainWindow::LoadFile()
{
    if (capture.isOpened())
        capture.release();     //decide if capture is already opened; if so,close it
    QString filename =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv)"));
    capture.open(filename.toLocal8Bit().data());
    ui->label->setText(tr("点击播放键开始检测"));

}



QImage MainWindow:: Mat2QImage(cv::Mat cvImg)
{

    int now_frame = capture.get(CV_CAP_PROP_POS_FRAMES);
    qDebug()<<"当前帧： "<<now_frame;
    if(now_frame%10==0)
    {
        qDebug()<<"检测";
        dector->image = cvImg;
        num_person = dector->decte();
        cvImg = dector->draw();
        //状态栏显示检测结果
        statusBar()->showMessage(tr("total person: %1,  warning: %2").arg(num_person).arg(dector->warning));
    }
    else
    {
        cvImg = dector->draw();
    }
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    //适应窗口
    double label_height=ui->label->height();
    double label_width = ui->label->width();
    double img_height = qImg.height();
    double img_width = qImg.width();
    double rate_h=label_height/img_height;
    double rate_w=label_width/img_width;
    double sub_h = label_height-img_height;
    double sub_w = label_width-img_width;
    if ((rate_h>=rate_w))
    {
        if(sub_h*sub_w>=0 && sub_h>=0 &&sub_w>=0)
        {
            //放大
        new_img = qImg.scaledToWidth(label_width);
        }
        else
        {
            //缩小
        new_img = qImg.scaledToHeight(label_height);
        }

    }
    else
    {
        if(sub_h*sub_w>=0 && sub_h>=0 &&sub_w>=0)
        {
            //放大
        new_img = qImg.scaledToWidth(label_width);
        }
        else
        {
            //缩小
        new_img = qImg.scaledToHeight(label_height);

        }

    }

    return new_img;

}


void MainWindow::Play()
{
    if (capture.isOpened())
    {
        if(flag_play==1)
        {
            rate= capture.get(CV_CAP_PROP_FPS);
            capture >> frame;

            if (!frame.empty() && flag_play==1)
            {
                dector = new Dector(frame);
                image = Mat2QImage(frame);
                ui->label->setPixmap(QPixmap::fromImage(image));
                timer = new QTimer(this);
                timer->setInterval(1000/rate);   //set timer match with FPS
                connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
                timer->start();
            }
        }
        else if(flag_play==2)
        {
            capture.release();
            ui->label->clear();
            ui->label->setText(tr("待读入数据"));


        }
    }
}

void MainWindow:: SetPlay(int flag)
{
    flag_play=flag;
    Play();
}
