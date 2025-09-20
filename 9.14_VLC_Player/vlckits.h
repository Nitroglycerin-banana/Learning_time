#ifndef VLCKITS_H
#define VLCKITS_H

#include <QObject>
#include <vlc/vlc.h>
#include "ui_vlcwidget.h"

class VLCKits : public QObject
{
    Q_OBJECT
public:
    explicit VLCKits(QObject *parent = nullptr);
    ~VLCKits();

    bool initVLC();
    bool play(const QStringList& names,void* hwnd);
    void play();
    void pause();
    void stop();
    void setVolume(int value);
    void setPosition(int value);
    void addMediaIndex();
    void itemPlay(int index);

    libvlc_media_player_t* mediaPlayer()const{return _pMediaPlayer;};
    //根据列表索引获取当前播放视频的总时长
    libvlc_time_t durations() const {return _durationArr[_currentIndex];};

    //需要修改ui界面时，通过信号来触发，以下三个函数调用时触发信号
    void setTimeSliderPos(int value);
    void setTimeText(const QString& str);
    void setVolumePos(int value);


signals:
    //自定义信号
    void sigTimeSliderPos(int value);
    void sigTimeText(const QString& str);
    void sigVolumeSliderPos(int value);
    void sigWillEnd(bool flag);
    void sigChangeVoice(int val);


private:
    libvlc_instance_t* _pInstance=nullptr;
    libvlc_media_t* _pMedia=nullptr;
    //libvlc_media_player_t* _pMediaPlayer=nullptr;
    libvlc_event_manager_t* _pEventManager=nullptr;
    //libvlc_time_t _totalSec=0;

    libvlc_media_list_player_t* _pMediaListPlayer=nullptr;
    libvlc_media_list_t* _pMediaList=nullptr;

    std::vector<libvlc_time_t> _durationArr;
    int _currentIndex=-1;

    bool _first=true;
public:
    libvlc_media_player_t* _pMediaPlayer=nullptr;


};

#endif // VLCKITS_H
