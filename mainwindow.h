#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>

#include <Windows.h>

#include "opencv2/imgcodecs.hpp"
#include<opencv2/video.hpp>

#include <cmath>
#include<vector>

#include<QString>
#include<QFileDialog>
#include<iostream>
#include<sstream>
#include <QTimer>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QTimer>
#include <QMessageBox>

#include "rtspstreammuxtask.h"

using namespace cv;
using namespace std;

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include <libavdevice/avdevice.h>
    #include <libavformat/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
}



class MainWindow :public QWidget //public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void processVideo();
    void detectAndDisplay(Mat frame);
    static void mouseRectHandler(int event, int x, int y , int flags, void *param);

    ~MainWindow();

    bool connectRtsp();

     void setini();
     Q_INVOKABLE void cameraStart();
     Q_INVOKABLE void cameraStop();

signals:
    void stopSig();



public slots:
        void onCheckBody();
        void onVlcTest();

     //   void onStop();
        void onChangeName();
        void onTimerOut();

        void onTimerOn();


private:
  //  Mat image; // 当前帧
 //   Mat fgMaskMOG2;
//    Mat segm; //fream的副本

      string m_strFilePath;
      string m_savePath;

      int m_left = 0;
      int m_right = 0;

    //  const char *m_strIPCAddr;
      string m_strIPCAddr;

      RtspStreamMuxTask *m_rts;

      QLineEdit *m_reName;
      QWidget *m_setName ;
      QSettings *m_setIni;
      QTimer *m_timer;





//    vector<vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    BackgroundSubtractorMOG2 *pMOG2;
};

#endif // MAINWINDOW_H
