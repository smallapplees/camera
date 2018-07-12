#include "mainwindow.h"
#include <QDebug>

//String face_cascade_name = "haarcascade_frontalface_default.xml";
//String face_cascade_name = "D:/WORK/Exercise/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "D:/WORK/Exercise/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
//String body_cascade_name = "D:/WORK/Exercise/opencv/sources/data/haarcascades_cuda/haarcascade_upperbody.xml";
//String leftFace_cascade_name = "D:/WORK/Exercise/opencv/sources/data/haarcascades/haarcascade_profileface.xml";

String face_cascade_name = "D:/WORK/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "D:/WORK/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
String body_cascade_name = "D:/WORK/opencv/sources/data/haarcascades_cuda/haarcascade_upperbody.xml";
String leftFace_cascade_name = "D:/WORK/opencv/sources/data/haarcascades/haarcascade_profileface.xml";

CascadeClassifier face_cascade;   //定义人脸分类器
CascadeClassifier eyes_cascade;   //定义人眼分类器
String window_name = "Capture - Face detection";
Point downPoint;
Rect box;
bool drawing_box = false;
bool gotBox =  false;
Mat m_frame;
Mat m_roi;

MainWindow::MainWindow(QWidget *parent) :QWidget(parent)
{
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    this->resize(QSize(600,480));

  //  QTimer::singleShot(1000, this, SLOT(onCheckBody()));
    m_strFilePath = "D:/WORK/Exercise/mp4Move/";

    QTimer::singleShot(1000, this, SLOT(onVlcTest()));
    QPushButton *start = new QPushButton;

    QPushButton *stop  = new QPushButton;
    QLineEdit *setTime = new QLineEdit;
    QHBoxLayout *hLayout=new QHBoxLayout;
    QVBoxLayout *vLayout =new QVBoxLayout;
 //   QGroupBox* recording = new QGroupBox(tr("set Recording time"));

    start->setText("Start");
    stop->setText("Stop");
    hLayout->addWidget(start);
    hLayout->addWidget(stop);
    vLayout->addWidget(setTime);
    vLayout->addLayout(hLayout);
  //  recording->setLayout(vLayout);

  //  connect(start, SIGNAL(clicked(bool)), this, SLOT(onStart()));
  //  connect(stop, SIGNAL(clicked(bool)), this, SLOT(onStop()));

    this->setLayout(vLayout);

}

MainWindow::~MainWindow()
{
}

void MainWindow::mouseRectHandler(int event, int x, int y , int flags, void *param)
{
    Mat img = m_frame.clone();

    switch (event) {
    case CV_EVENT_MOUSEMOVE:

        if(drawing_box)
        {
            if(x >= downPoint.x && y >= downPoint.y)
            {
                box.x = downPoint.x;
                box.y = downPoint.y;
                box.width = x - downPoint.x;
                box.height =  y -  downPoint.y;
            }
            //鼠标的移动到downPoint的右上角
            if( x >= downPoint.x && y <= downPoint.y)
            {
                box.x =  downPoint.x;
                box.y = y;
                box.width = x - downPoint.x;
                box.height = downPoint.y - y;
            }
            //鼠标的移动到downPoint的左上角
            if( x <= downPoint.x && y <= downPoint.y)
            {
                box.x = x;
                box.y = y;
                box.width = downPoint.x - x;
                box.height = downPoint.y - y;
            }
            //鼠标的移动到downPoint的左下角
            if( x <= downPoint.x && y >= downPoint.y)
            {
                box.x = x;
                box.y = downPoint.y;
                box.width = downPoint.x -x;
                box.height = y - downPoint.y;
            }
        }
        break;
    case CV_EVENT_LBUTTONDOWN:
        drawing_box = true;
        //起点
        downPoint = Point(x,y);
        break;

    case CV_EVENT_LBUTTONUP:
        //松开鼠标，结束画矩形
        drawing_box = false;
        gotBox = true;

        if(box.height > 0 && box.width > 0)
        {
            m_roi = img(Rect(box.x, box.y, box.width, box.height));
            rectangle(img, box, Scalar(255,255,255),2);
            //  namedWindow("signROI");
            //    imshow("SignROI", img);
            namedWindow("ROI");
//            while(1){
            imshow("ROI", m_roi);
//             waitKey(50);
//            //    stop = true;
//            }
        }

        break;
    default:
        break;
    }
}

void MainWindow::processVideo()
{
    /*    Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
//    bgsubtractor->setHistory(20);            //训练背景帧数
//    bgsubtractor->setVarThreshold(100);      //方差阀值，判断前景还是背景，值越大灵敏度越低
//    bgsubtractor->setDetectShadows(true);    //是否检测有影子，开启后增加算法复杂度
//    bgsubtractor->setBackgroundRatio(4);     //高斯背景模型权重喝阀值，nmixtures个模型按权重排序后
//    bgsubtractor->setNMixtures(5);           //高斯模型个数（默认5个，最高8个）
//    bgsubtractor->setShadowThreshold(40);
//    bgsubtractor->setVarInit(15);            //新建高斯模型的方差初始值
//    bgsubtractor->setVarMax(20);             //背景更新时，用于限制高斯模型方差的最大值，默认（20）
//    bgsubtractor->setVarMin(4);              //背景更新时，用于限制高斯模型方差的最小值，默认（4）
//    bgsubtractor->setVarThresholdGen(100);   //方差阀值，用于已经醉在的匹配模型，不存在则新建一个
*/
    //   Mat image;
    /*
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        qDebug()<<"the camera is no open::";
    }

  //  namedWindow("mask", WINDOW_AUTOSIZE);
    Mat frame, mask, maskCp;
    capture.read(frame);
    //  Ptr<BackgroundSubtractor> pKNN = createBackgroundSubtractorKNN();
    //  long FRAMECNT = capture.get(CV_CAP_PROP_FRAME_COUNT);

    std::vector<std::vector<Point>> cnts;

    Rect maxRect;

    const double RECT_HW_RATIO    = 1.5;    //人体长宽比阀值;
    const double RECT_AREA_RATIO  = 0.008;  //人体站整个图想最小比例值；
    const double RECT_AREA_RATIO2 = 0.2;    //人体占整体图像最大比例值;

    Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setHistory(20);
    bgsubtractor->setVarThreshold(100);
    bgsubtractor->setDetectShadows(true);

    bool hasPeople = false;
    int count = 0;
    int hasPeopleFrameCnt = 0;
    int spaceFrames = 0;
    const int SPACE_FRAME = 125;
    if (!face_cascade.load(face_cascade_name))
    {
        qDebug()<<"--(!)Error loading face cascade\n";
        // return -1;
    }
    if (!eyes_cascade.load(eyes_cascade_name))
    {
        qDebug()<<"--(!)Error loading eyes cascade\n";

    }

    while(capture.read(frame)) {
        capture >> frame;
         resize(frame, Size(frame.cols/4, frame.rows/4));
                bgsubtractor->apply(frame, mask, 0.002);            //背景更新
                medianBlur(mask, mask, 3);                          //中值滤波
                threshold(mask, mask, 200, 255, CV_THRESH_BINARY);  //比值分割，去阴影
                maskCp = mask.clone();                              //找轮廓
                findContours(maskCp, cnts, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
                vector<Point> maxCnt;
                for(int i = 0; i < cnts.size(); ++i)
                {
                    maxCnt = maxCnt.size() > cnts[i].size() ? maxCnt: cnts[i];
                }

                if(maxCnt.size() > 0) //最大外接矩形
                {
                    maxRect = boundingRect(maxCnt);
                    double rectAreaRatio = (double)maxRect.area() / (frame.cols * frame.rows);
                    if((double)maxRect.height / maxRect.width > RECT_HW_RATIO && rectAreaRatio >RECT_AREA_RATIO && rectAreaRatio < RECT_AREA_RATIO2)
                    {
                        rectangle(frame, maxRect.tl(), maxRect.br(), Scalar(0,255, 0), 2);
                        ++hasPeopleFrameCnt;
                    }
                }
                ++spaceFrames;
                if(spaceFrames >= SPACE_FRAME)
                {
                    if(hasPeopleFrameCnt > SPACE_FRAME /8)
                    {
                        hasPeople = true;
                        qDebug()<< "people::";
                    }else{
                        hasPeople = false;
                        qDebug()<<" no";
                    }
                    hasPeopleFrameCnt = 0;
                    spaceFrames = 0;
                }

              imshow("frame", frame);
              imshow("mask", mask);

        if(waitKey(10) == 27)
        {
            break;
        }
    }
    */
    // return 0;
    /*
//        pKNN->apply(image, KNNMask);
//        threshold(KNNMask, KNNMask, 100, 255, THRESH_BINARY);
//        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
//        morphologyEx(KNNMask, KNNMask, MORPH_OPEN, kernel, Point(-1,-1));
//        findContours(KNNMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0,0));
//        for(int i = 0; i< contours.size(); i++)
//        {
//            double area = contourArea(contours[i]);
//            //轮廓外接矩阵
//            Rect rect = boundingRect(contours[i]);
//            if (area < 500 || rect.width < 50 || rect.height < 50) continue;
//            rectangle(image, rect, Scalar(0,255,255),2);
//            putText(image, "Target", Point(rect.x, rect.y), CV_FONT_NORMAL, FONT_HERSHEY_PLAIN, Scalar(0,255,0),2,8);

//        }
//        imshow("image", image);
//        imshow("mask", KNNMask);
//        waitKey(1);
        */

}

void MainWindow::detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, Size(30, 30),Size(300,300));

    for(size_t i = 0; i < faces.size(); i++)
    {
        rectangle(frame, faces[i],Scalar(255,0,0),2,8,0);
        Mat faceROI = frame_gray(faces[i]);
        std::vector<Rect> eyes;
        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 1, CV_HAAR_DO_ROUGH_SEARCH, Size(3, 3));
        for (size_t j = 0; j < eyes.size(); j++)
        {
            Rect rect(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y, eyes[j].width, eyes[j].height);
            rectangle(frame, rect, Scalar(0, 255, 0), 2, 8, 0);
        }
    }
    namedWindow(window_name, 2);
    imshow(window_name, frame);

}

void MainWindow::onCheckBody()
{
    Ptr<TrackerKCF> tracker = TrackerKCF::create();

        // Read video
        VideoCapture cap(0);

        // Check video is open
        if (!cap.isOpened())
        {
            cout << "Could not open!" << endl;

        }

        // Read first frame.
        Mat frame;
        cap >> frame;

        // Define an initial bounding box
        Rect2d bbox;

        // Uncomment the line below if you
        // want to choose the bounding box
        bbox = selectROI(frame, false);

        // Initialize tracker with first frame and bounding box
        tracker->init(frame, bbox);

        while (waitKey(30) != 27)
        {
            cap >> frame;
            // Update tracking results
            tracker->update(frame, bbox);

            // Draw bounding box
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

            // Display result
            imshow("Tracking", frame);


        }


    /*VideoCapture cap(0);

    if(!cap.isOpened())
    {
        qDebug()<<"The camera is no Open::";
    }

    Mat gray;
    Mat rightFace;
    CascadeClassifier cascade;
    bool stop = false;
    cascade.load(face_cascade_name);

    CascadeClassifier leftFaceCascade;
    leftFaceCascade.load(leftFace_cascade_name);

    CascadeClassifier *cascadeBody = new CascadeClassifier;
    cascadeBody->load(body_cascade_name);
    namedWindow("face", CV_WINDOW_NORMAL);
    setMouseCallback("face",mouseRectHandler,NULL);

    while (!stop)
    {
        cap >> m_frame;

        //建立用于存放人脸的向量容器
        vector<Rect> faces(0), leftFaces(0), rightFaces(0);

        cvtColor(m_frame, gray, CV_BGR2GRAY);
         flip(gray, rightFace, 1);
        //    gray.resize((800,600), 1);
        //变换后的图像进行直方图均值化处理
        //     equalizeHist(gray, gray);

 //           HOGDescriptor hog;//HOG特征检测器
        //      hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//设置SVM分类器为默认参数
     //        hog.load(body_cascade_name);
        vector<Rect> found, found_filtered;//矩形框数组
         //    hog.detectMultiScale(m_frame, found, 0, Size(8,8), Size(32,32), 1.05, 2);//对图像进行多尺度检测，检测窗口移动步长为(8,8)

        //  cascadeBody->detectMultiScale(m_frame, found, 1.1, 3, 0| CV_HAAR_SCALE_IMAGE, Size(30, 30),Size(1000,1000));
        cascadeBody->detectMultiScale(m_frame, found, 1.1, 3, 0| CV_HAAR_DO_CANNY_PRUNING, Size(100, 100),Size(500,500));

        //找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
        for(int i=0; i < found.size(); i++)
        {
            Rect r = found[i];
            int j=0;
            for(; j < found.size(); j++)
                if(j != i && (r & found[j]) == r)
                    break;
            if( j == found.size())
                found_filtered.push_back(r);
        }

        //画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
        for(int i=0; i<found_filtered.size(); i++)
        {
            Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.07);
            r.height = cvRound(r.height*1.2);
            rectangle(m_frame, r.tl(), r.br(), Scalar(0,255,0), 3);
        //    rectangle(m_frame, found_filtered[i], Scalar(0,255,0), 3, 8,0);

        }

        cascade.detectMultiScale(gray, faces, 1.1, 5, 0| CV_HAAR_DO_CANNY_PRUNING, Size(60, 60),Size(500,500));

        //左脸
        leftFaceCascade.detectMultiScale(gray, leftFaces, 1.1, 5, 0| CV_HAAR_DO_CANNY_PRUNING, Size(60, 60),Size(500,500));

        //右脸
        leftFaceCascade.detectMultiScale(rightFace, rightFaces, 1.1, 5, 0| CV_HAAR_DO_CANNY_PRUNING, Size(60, 60),Size(500,500));

        for (size_t i = 0; i < faces.size(); i++)
        {
            if (faces[i].height > 0 && faces[i].width > 0)
            {
            //    face = gray(faces[i]);
             //   text_lb = Point(faces[i].x, faces[i].y);
                rectangle(m_frame, faces[i], Scalar(0, 0, 255), 3, 8, 0);

            }
        }

        for( size_t j = 0; j < leftFaces.size(); j++)
        {
            if(leftFaces[j].height > 0 && leftFaces[j].width > 0)
            {
                rectangle(m_frame, leftFaces[j], Scalar(255, 0, 0), 3, 8, 0);

            }
        }

        for(size_t k = 0; k < rightFaces.size(); k++)
        {
            if(rightFaces[k].height > 0 && rightFaces[k].width > 0)
            {
                rectangle(m_frame, Rect(m_frame.cols - rightFaces[k].x - rightFaces[k].width, rightFaces[k].y, rightFaces[k].width, rightFaces[k].height), Scalar(255,255,0), 3, 8, 0);
            }
        }


        rectangle(m_frame,box,Scalar(255,255,255),2);

        imshow("face", m_frame);
        if (waitKey(5) >= 0)
            stop = true;
    }
    */
}

//测试ffmpeg是否成功
void MainWindow::onVlcTest()
{
//    qDebug()<< "AAA::"<<avcodec_configuration();
//    unsigned version = avcodec_version();
//    QString ch = QString::number(version, 10);
//    qDebug()<< "version::"<< version;

    m_strIPCAddr = "rtsp://192.168.0.253/video2";
    QSettings *strini = new QSettings( QString::fromStdString(m_strFilePath) + "setTime.ini",QSettings::IniFormat);
    if(strini->value("time/seconds").toInt()<= 0){
        setini();
    }




}

void MainWindow::cameraStart()
{
    QSettings *pstr = new QSettings( QString::fromStdString(m_strFilePath) + "setTime.ini",QSettings::IniFormat);
    int i = 0;
    i = pstr->value("time/seconds").toInt();
    if(i)
    {
        qDebug()<<"the time is::"<< i;
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
    string strStartTime = current_date.toStdString();

    m_savePath = m_strFilePath + strStartTime+ ".mp4";

    m_rts = new RtspStreamMuxTask();

    m_timer = new QTimer();
    connect(m_rts, SIGNAL(runtimer()), this, SLOT(onTimerOn()));
    m_rts->SetInputUrl(m_strIPCAddr);
    m_rts->setTime(i );
    m_rts->SetOutputPath(m_savePath);
    m_rts->StartRecvStream();
 //   if(m_rts)
 //   {
 //       m_timer = new QTimer();
//        connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
//        m_timer->start(1000);
//    }


}

void MainWindow::cameraStop()
{

    m_timer->stop();
//    m_setName= new QWidget();
//    m_setName->resize(QSize(200,100));
//    m_reName = new QLineEdit;
//    QPushButton *mok = new QPushButton;
//    QHBoxLayout *hlayout = new QHBoxLayout;
//    hlayout->addWidget(m_reName);
//    hlayout->addWidget(mok);
//    m_setName->setLayout(hlayout);
//    mok->setText("SAVE");
//    connect(mok, SIGNAL(clicked(bool)), this, SLOT(onChangeName()));;
//    m_setName->show();


//    QString filePath;//存储保存路径
//    filePath = QFileDialog::getExistingDirectory(this,"");//获取文件夹路径
//    qDebug()<<"LLLLLLl::"<< filePath<< QString::fromStdString(m_savePath);
//    QFile file(filePath+"/"+QString::fromStdString(m_savePath));
//    file.open(QFile::WriteOnly);


    QString strPath = QFileDialog::getSaveFileName(this,tr("Save"),"","*.mp4");
    QFile::rename(QString::fromStdString(m_savePath),strPath);
 //   qDebug()<<"LLLL::"<<strPath;


//    if(filePath.isEmpty())
//    {
//        QMessageBox::information(this,"信息","保存失败");
//    }
//    else
//    {
//        dirPath=QString("%1/保存实验%2").arg(filePath).arg(str);
//        QDir *temp = new QDir;//声明文件对象
//        temp->mkdir(dirPath);//创建文件夹
//        QImage images;

//        //将图片重新命名并保存至刚刚创建的文件夹里
//        QString savePath=QString("%1//%2.jpg").arg(dirPath).arg(saveName);
//        QMessageBox::information(this,"信息","保存成功");
//    }



    m_rts->StopRecvStream();
}

void MainWindow::onChangeName()
{

    if(!m_reName->text().isEmpty())
    {
        QString newName = QString::fromStdString(m_strFilePath)+m_reName->text()+".mp4";
        QString oldName = QString::fromStdString(m_savePath);
        QFile::rename(oldName,newName);
        m_setName->hide();
    }
}

void MainWindow::setini()
{
    qDebug()<<"AAAAA::"<< QCoreApplication::applicationDirPath();
  //  m_setIni = new QSettings( QString::fromStdString(m_strFilePath) + "setTime.ini",QSettings::IniFormat);
  //  m_setIni->setValue("/time/seconds", 5400);


}

void MainWindow::onTimerOn()
{
    if(m_rts)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
        m_timer->start(1000);
    }

}

void MainWindow::onTimerOut()
{
    if(m_rts)
    {
        m_rts->onTimerOut();
        if(m_rts->getStopFrame() <= 1)
        {
            cameraStop();
           // m_timer->stop();
        }
    }
}









