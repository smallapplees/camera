#ifndef RTSPSTREAMMUXTASK_H
#define RTSPSTREAMMUXTASK_H
#include <QObject>
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

#include <QThread>
#include <cmath>
#include <vector>

#include<QString>
#include<QFileDialog>
#include<iostream>
#include<sstream>
#include <QTimer>

#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>
#include <Windows.h>
#include <string>
#include <QDebug>
#include <QTimer>

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

class RtspStreamMuxTask: public QThread
{
    Q_OBJECT
public:
    RtspStreamMuxTask();
     ~RtspStreamMuxTask();

        void  SetInputUrl(string rtspUrl);
        void  SetOutputPath(string outputPath);

        void StartRecvStream();
        void StopRecvStream();

        void setTime(int i);
        void onTimerOut();

        void GetVideoSize(long & width, long & height)  //获取视频分辨率
        {
            width  = coded_width;
            height = coded_height;
        }

        int getStopFrame(){return m_stopFrame;}

    private:
        void run();

        int  OpenInputStream();
        void CloseInputStream();

        void readAndMux();

        static DWORD WINAPI  ReadingThrd(void * pParam);

        int  openOutputStream();
        void closeOutputStream();

        void ReleaseCodecs();

signals:
        void runtimer();



    private:

        string m_inputUrl;
        string m_outputFile;

        AVFormatContext* m_inputAVFormatCxt;
        AVBitStreamFilterContext* m_bsfcAAC;
        AVBitStreamFilterContext* m_bsfcH264;

        int m_videoStreamIndex;
        int m_audioStreamIndex;

        AVFormatContext* m_outputAVFormatCxt;

        char m_tmpErrString[64];
        bool m_stop_status;

        HANDLE m_hReadThread;

        BOOL   m_bInputInited;
        BOOL   m_bOutputInited;

        int    coded_width, coded_height;
        int    m_frame_rate;
        int    m_stopFrame;

        QTimer *m_timer;

};

#endif // RTSPSTREAMMUXTASK_H
