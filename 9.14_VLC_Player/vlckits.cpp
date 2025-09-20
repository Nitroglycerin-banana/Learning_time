#include "vlckits.h"
#include <QDebug>
#include <vlc/vlc.h>
#include <QMessageBox>
#include <QFileDialog>


VLCKits::VLCKits(QObject *parent)
{  
}

VLCKits::~VLCKits()
{
    libvlc_media_player_release(_pMediaPlayer);
    libvlc_media_release(_pMedia);
    libvlc_release(_pInstance);
}

void VLCKits::itemPlay(int index)
{
    libvlc_media_list_player_play_item_at_index(_pMediaListPlayer,index);
    _currentIndex=index;//更新当前切换的视频总时长列表索引

}


static void processPosition(VLCKits* pkits)
{
    float pos=libvlc_media_player_get_position(pkits->mediaPlayer());
    pkits->setTimeSliderPos(pos*100);
    libvlc_time_t totalSecs=pkits->durations();
    libvlc_time_t curSecs=libvlc_media_player_get_time(pkits->mediaPlayer())/1000;
    int hh = curSecs / 3600;
    int mm = curSecs % 3600 / 60;
    int ss = curSecs % 60;
    int HH = totalSecs / 3600;
    int MM = totalSecs % 3600 / 60;
    int SS = totalSecs % 60;
    if(totalSecs-curSecs<=15)
    {
        emit pkits->sigWillEnd(true);
    }
    else{
        emit pkits->sigWillEnd(false);
    }

    char buff[64] = {0};
    sprintf(buff, "%02d:%02d:%02d/%02d:%02d:%02d",
                    hh, mm, ss, HH, MM, SS);
    QString str(buff);
    pkits->setTimeText(str);
}

static void processVolume(VLCKits * pkits)
{
    int val = libvlc_audio_get_volume(pkits->mediaPlayer());
    qDebug() << "volume:" << val;
    if(val == -1) {
        return;
    }
    emit pkits->sigChangeVoice(val);
    pkits->setVolumePos(val);
}




void VLCKits::addMediaIndex()
{
    ++_currentIndex;
    _currentIndex %=_durationArr.size();
}

static void processMediaChanged(VLCKits* pkits)
{
    qDebug()<<">>>> processMediaChanged()";
    pkits->addMediaIndex();
}

static void vlc_callback( const struct libvlc_event_t *p_event, void *p_data )
{
    VLCKits * pkits = static_cast<VLCKits*>(p_data);
    if(pkits) {
        switch(p_event->type) {
        case libvlc_MediaPlayerPositionChanged://播放位置变化时
            processPosition(pkits); break;
        case libvlc_MediaPlayerAudioVolume://音量变化时
            processVolume(pkits); break;
        case libvlc_MediaPlayerMediaChanged://播放的媒体变化时
            processMediaChanged(pkits);break;
        }
    }
}




bool VLCKits::initVLC()
{
    _pInstance=libvlc_new(0,nullptr);
    if(_pInstance)
    {
        _pMediaPlayer=libvlc_media_player_new(_pInstance);
        if(_pMediaPlayer)
        {
            _pEventManager=libvlc_media_player_event_manager(_pMediaPlayer);
            if(_pEventManager)
            {
                //视频位置发生改变时
                libvlc_event_attach(_pEventManager,libvlc_MediaPlayerPositionChanged,
                                    vlc_callback,this);
                //声音发生改变时
                libvlc_event_attach(_pEventManager,libvlc_MediaPlayerAudioVolume,
                                    vlc_callback,this);
                //媒体改变时
                //当前文件播放完毕后，到下一个文件播放时触发
                libvlc_event_attach(_pEventManager,libvlc_MediaPlayerMediaChanged,
                                    vlc_callback,this);
                return true;
            }else{
              libvlc_media_player_release(_pMediaPlayer);
              libvlc_release(_pInstance);
              return false;
            }
        }else{
        libvlc_release(_pInstance);
        return false;
    }
   }else{
        return  false;
   }

}

//加载视频文件路径并播放视频
bool VLCKits::play(const QStringList& names,void* hwnd)
{
//    //设置当前播放列表大小
//    _ListNow=names.count();
    //设置列表播放器
    _pMediaListPlayer=libvlc_media_list_player_new(_pInstance);
    if(!_pMediaListPlayer)
    {
        return false;
    }
    _pMediaList=libvlc_media_list_new(_pInstance);
    if(!_pMediaList)
    {
        return false;
    }
    for(int i=0;i<names.size();++i)
    {
        QString filename=names[i];
        filename=QDir::toNativeSeparators(filename);
         //设置媒体
        _pMedia=libvlc_media_new_path(_pInstance,filename.toStdString().c_str());
        if(!_pMedia)
        {
        return false;
        }

        //将媒体添加到播放列表中
        libvlc_media_list_add_media(_pMediaList,_pMedia);
        //解析媒体元数据，获取每一个文件的时间长度
        libvlc_media_parse(_pMedia);
        libvlc_time_t durationSecs=libvlc_media_get_duration(_pMedia)/1000;
        _durationArr.push_back(durationSecs);
        libvlc_media_release(_pMedia);

    }
    //列表播放器关联媒体列表
    libvlc_media_list_player_set_media_list(_pMediaListPlayer,_pMediaList);
    //设置播放模式（列表循环播放）
    libvlc_media_list_player_set_playback_mode(_pMediaListPlayer,libvlc_playback_mode_loop);
    //列表播放器关联媒体播放器
    libvlc_media_list_player_set_media_player(_pMediaListPlayer,_pMediaPlayer);
    //设置播放窗口
    libvlc_media_player_set_hwnd(_pMediaPlayer,hwnd);
    if(_first){
    //开始播放
    libvlc_media_list_player_play(_pMediaListPlayer);
    }
    _first=false;

    return true;
}

//暂停和停止状态下如果点击播放继续播放
void VLCKits::play()
{
    libvlc_state_t state = libvlc_media_player_get_state(_pMediaPlayer);
    if(state == libvlc_Paused ||
        state == libvlc_Stopped) {
        libvlc_media_player_play(_pMediaPlayer);
    }

}

//暂停播放
void VLCKits::pause()
{
    libvlc_state_t state=libvlc_media_player_get_state(_pMediaPlayer);
    if(state==libvlc_Playing)
    {
        libvlc_media_player_pause(_pMediaPlayer);
    }
    QPixmap map;
    map.load(":/volumeST.png");


}

//停止播放
void VLCKits::stop()
{
    libvlc_state_t state = libvlc_media_player_get_state(_pMediaPlayer);
    if(state == libvlc_Playing ||
        state == libvlc_Paused) {
        libvlc_media_player_stop(_pMediaPlayer);
        //重置进度条的位置
        emit sigTimeSliderPos(0);
        emit sigTimeText("00:00:00/00:00:00");
    }
}

//更新进度条位置
void VLCKits::setTimeSliderPos(int value)
{   //发送信号
    emit sigTimeSliderPos(value);
}

//更新（当前/总时间）显示
void VLCKits::setTimeText(const QString &str)
{   //发送信号
    emit sigTimeText(str);
}

//更新音量条位置
void VLCKits::setVolumePos(int value)
{
    emit sigVolumeSliderPos(value);
}

//调整音量
void VLCKits::setVolume(int value)
{
    libvlc_audio_set_volume(_pMediaPlayer, value);
}

//调整进度条位置
void VLCKits::setPosition(int value)
{
    libvlc_media_player_set_position(_pMediaPlayer, value / 100.0);
}

