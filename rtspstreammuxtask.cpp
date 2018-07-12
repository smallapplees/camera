#include "RtspStreamMuxTask.h"
#include <sstream>



//////////////////////////////////////////////////////////////

RtspStreamMuxTask::RtspStreamMuxTask()
{
    m_stop_status = false;
    m_inputAVFormatCxt = nullptr;
    m_bsfcAAC = nullptr;
    m_bsfcH264 = nullptr;
    m_videoStreamIndex = -1;
    m_audioStreamIndex = -1;
    m_outputAVFormatCxt = nullptr;
    m_hReadThread = NULL;
    m_bInputInited = FALSE;
    m_bOutputInited = FALSE;
    coded_width = coded_height = 0;
    m_frame_rate = 25;



}

RtspStreamMuxTask::~RtspStreamMuxTask()
{
    StopRecvStream();
}

//设置rspt地址
void  RtspStreamMuxTask::SetInputUrl(string rtspUrl)
{
    m_inputUrl = rtspUrl;
}

void RtspStreamMuxTask::setTime(int i)
{
    m_stopFrame = i;// * 25;
}

//设置视频输出位置
void  RtspStreamMuxTask::SetOutputPath(string outputPath)
{
    m_outputFile = outputPath;
}

void RtspStreamMuxTask::StartRecvStream()
{
 //
    if(m_inputUrl.empty())
        return;

    m_videoStreamIndex = -1;
    m_audioStreamIndex = -1;

    m_bInputInited  = FALSE;
    m_bOutputInited = FALSE;

    coded_width = coded_height = 0;

    DWORD threadID = 0;
    m_hReadThread = CreateThread(NULL, 0, ReadingThrd, this, 0, &threadID);
}


void RtspStreamMuxTask::StopRecvStream()
{
    m_stop_status = true;

    if (m_hReadThread != NULL)
    {
        WaitForSingleObject(m_hReadThread, INFINITE);
        CloseHandle(m_hReadThread);
        m_hReadThread = NULL;
    }
    CloseInputStream();
}

DWORD WINAPI RtspStreamMuxTask::ReadingThrd(void * pParam)
{
    RtspStreamMuxTask * pTask = (RtspStreamMuxTask *) pParam;

    pTask->run();

    return 0;
}

void RtspStreamMuxTask::run()
{
    try
    {
        m_stop_status = false;

        OpenInputStream();
        openOutputStream();
        m_stop_status = false;

        readAndMux();

        CloseInputStream();
        closeOutputStream();

    }
    catch(std::exception& e)
    {
        CloseInputStream();
    }
}

int RtspStreamMuxTask::OpenInputStream()
{
    if (m_inputAVFormatCxt)
    {
        QString strError  = ("already has input avformat");
        qDebug()<<strError;
        return -1;
    }

    m_bsfcAAC = av_bitstream_filter_init("aac_adtstoasc");
    if(!m_bsfcAAC)
    {
        QString strError = ("can not create aac_adtstoasc filter");
        qDebug()<<strError;
        return -1;
    }

    m_bsfcH264 = av_bitstream_filter_init("h264_mp4toannexb");
    if(!m_bsfcH264)
    {
        QString strError= ("can not create h264_mp4toannexb filter");
        qDebug()<<strError;
        return -1;
    }

    int res = 0;

    res = avformat_open_input(&m_inputAVFormatCxt, m_inputUrl.c_str(), 0, NULL);

    if(res < 0)
    {
        qDebug()<<"can not open file:";
        return -1;
    }

    if (avformat_find_stream_info(m_inputAVFormatCxt, 0) < 0)
    {
        QString strError = ("can not find stream info");
        qDebug()<<strError;
        return -1;
    }
    av_dump_format(m_inputAVFormatCxt, 0, m_inputUrl.c_str(), 0);
    for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
    {
        AVStream *in_stream = m_inputAVFormatCxt->streams[i];

        qDebug()<< "codec id:: "<< in_stream->codec->codec_id<< " URL::"<<  m_inputUrl.c_str();

        if (in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_videoStreamIndex = i;

            coded_width = in_stream->codec->width;
            coded_height = in_stream->codec->height;

            if(in_stream->avg_frame_rate.den != 0 && in_stream->avg_frame_rate.num != 0)
            {
              m_frame_rate = in_stream->avg_frame_rate.num/in_stream->avg_frame_rate.den;//每秒多少帧
            }
            qDebug()<<"video stream index::"<< m_videoStreamIndex<< " width::"<< in_stream->codec->width
                   << " height::"<< in_stream->codec->height<< " FrameRate::"<<m_frame_rate;

      }
        else if (in_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_audioStreamIndex = i;
        }
    }

    m_bInputInited = TRUE;
    return 0;
}

void RtspStreamMuxTask::CloseInputStream()
{
    if (m_inputAVFormatCxt)
    {
        avformat_close_input(&m_inputAVFormatCxt);
    }
    if(m_bsfcAAC)
    {
        av_bitstream_filter_close(m_bsfcAAC);
        m_bsfcAAC = nullptr;
    }
    if(m_bsfcH264)
    {
        av_bitstream_filter_close(m_bsfcH264);
        m_bsfcH264 = nullptr;
    }

    m_bInputInited = FALSE;
}


int  RtspStreamMuxTask::openOutputStream()
{
    if (m_outputAVFormatCxt)
    {
        qDebug()<<"already has rtmp avformat::";
        return -1;
    }

    int res = 0;
    if(!m_outputFile.empty())
    {
        res = avformat_alloc_output_context2(&m_outputAVFormatCxt, NULL, "mp4", m_outputFile.c_str());

        if (m_outputAVFormatCxt == NULL)
        {
            qDebug()<<"can not alloc output context::";
            return -1;
        }

        AVOutputFormat* fmt = m_outputAVFormatCxt->oformat;

  //      fmt->audio_codec = AV_CODEC_ID_AAC;
  //      fmt->video_codec = AV_CODEC_ID_H264;

        for (int i = 0; i < m_inputAVFormatCxt->nb_streams; i++)
        {
            AVStream *in_stream = m_inputAVFormatCxt->streams[i];

            AVStream *out_stream = avformat_new_stream(m_outputAVFormatCxt, in_stream->codec->codec);
            if (!out_stream)
            {
                qDebug()<<"can not new out stream::";
                return -1;
            }
            res = avcodec_copy_context(out_stream->codec, in_stream->codec);
            if (res < 0)
            {
                qDebug()<<"can not copy context::";
                return -1;
            }
            if (m_outputAVFormatCxt->oformat->flags & AVFMT_GLOBALHEADER)
            {
                out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
            }
        }
        av_dump_format(m_outputAVFormatCxt, 0, m_outputFile.c_str(), 1);
        if (!(fmt->flags & AVFMT_NOFILE))
        {
            res = avio_open(&m_outputAVFormatCxt->pb, m_outputFile.c_str(), AVIO_FLAG_WRITE);
            qDebug()<<"res::"<< res<<m_outputFile.c_str();
            if (res < 0)
            {
                qDebug()<<"can not open output io::";
                return -1;
            }
        }


        res = avformat_write_header(m_outputAVFormatCxt, NULL);
        if (res < 0)
        {
            qDebug()<<"can not write outputstream header::";
            m_bOutputInited = FALSE;
            return -1;
        }

        m_bOutputInited = TRUE;
    }
    return 0;
}

void RtspStreamMuxTask::closeOutputStream()
{
    if (m_outputAVFormatCxt)
    {
        if(m_bOutputInited)
        {
          int res = av_write_trailer(m_outputAVFormatCxt);
        }

        if (!(m_outputAVFormatCxt->oformat->flags & AVFMT_NOFILE))
        {
            if(m_outputAVFormatCxt->pb)
            {
                avio_close(m_outputAVFormatCxt->pb);
            }
        }

        avformat_free_context(m_outputAVFormatCxt);
        m_outputAVFormatCxt = nullptr;
    }
    m_bOutputInited = FALSE;
}

void RtspStreamMuxTask::readAndMux()
{
    int nVideoFramesNum = 0;
    int64_t  first_pts_time = 0;
    DWORD start_time = GetTickCount();

    AVPacket pkt;
    av_init_packet(&pkt);

    emit runtimer();

    while(1)
    {
        if(m_stop_status == true)
        {
            break;
        }

        int res;

        res = av_read_frame(m_inputAVFormatCxt, &pkt);

        if (res < 0)  //读取错误或流结束
        {
            if(AVERROR_EOF == res)
            {
                qDebug()<<"End of file";
            }
            else
            {
                qDebug()<<"av_read_frame() got error:";
            }

            break;
        }

        AVStream *in_stream = m_inputAVFormatCxt->streams[pkt.stream_index];
        AVStream *out_stream = m_outputAVFormatCxt->streams[pkt.stream_index];

        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        if(in_stream->codec->codec_type != AVMEDIA_TYPE_VIDEO && in_stream->codec->codec_type != AVMEDIA_TYPE_AUDIO)
        {
            continue;
        }

        if(in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)  //视频
        {
       //     m_stopFrame--;
            nVideoFramesNum++;
            qDebug()<<"RRRRRRRR::"<<nVideoFramesNum<<m_stopFrame;
            // write the compressed frame to the output format
            int nError = av_interleaved_write_frame(m_outputAVFormatCxt, &pkt);
            if (nError != 0)
            {
                char tmpErrString[AV_ERROR_MAX_STRING_SIZE] = {0};
                av_make_error_string(tmpErrString, AV_ERROR_MAX_STRING_SIZE, nError);
                qDebug()<<"Error: while writing video frame";
            }
            //int nSecs = pkt.pts*in_stream->time_base.num/in_stream->time_base.den;
            //TRACE("Frame time: %02d:%02d \n", nSecs/60, nSecs%60);

        }
         else if(in_stream->codec->codec_type == AVMEDIA_TYPE_AUDIO) //音频
         {
            // write the compressed frame to the output format
            int nError = av_interleaved_write_frame(m_outputAVFormatCxt, &pkt);
            if (nError != 0)
            {
                char tmpErrString[AV_ERROR_MAX_STRING_SIZE] = {0};
                av_make_error_string(tmpErrString, AV_ERROR_MAX_STRING_SIZE, nError);
                qDebug()<<"Error: while writing audio frame";
        //		TRACE("Error: %d while writing audio frame, %s\n", nError, tmpErrString);
            }
         }

        if((in_stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)	)
        {
            if(first_pts_time == 0)
                first_pts_time = pkt.pts;

            int64_t pts_time = (pkt.pts - first_pts_time)*1000*in_stream->time_base.num/in_stream->time_base.den; //转成毫秒
            int64_t now_time = GetTickCount() - start_time;

            //if(pts_time > now_time + 10 && pts_time < now_time + 3000)
            //{
            //	Sleep(pts_time-now_time);
            //}
            //else if(pts_time == 0 && nVideoFramesNum > 1)
            //{
            //	Sleep(20);
            //}
        }

        av_free_packet(&pkt);

    }//while
    qDebug()<<"Reading ended, read video frames";
}

void RtspStreamMuxTask::onTimerOut()
{
    m_stopFrame--;
    qDebug()<< "KKKKKKK::"<<m_stopFrame;
    if(m_stopFrame <=1 )
    {
        m_stop_status = true;
        qDebug()<<"IIIIIII::";
    }
}



