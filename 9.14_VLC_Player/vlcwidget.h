#ifndef VLCWIDGET_H
#define VLCWIDGET_H

#include <QMainWindow>
#include <vlc/vlc.h>
#include <windows.h>
#include "ui_vlcwidget.h"
#include "vlckits.h"
#include "ctitlebar.h"
#include "cvolumebutton.h"
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>



QT_BEGIN_NAMESPACE
namespace Ui { class VLCWidget; }
QT_END_NAMESPACE

class VLCWidget : public QMainWindow
{
    Q_OBJECT

public:
    VLCWidget(QWidget *parent = nullptr);
    ~VLCWidget();
    void hide(bool set);//洁净模式
    //播放列表元素去重
    void UnRepeat(QStringList& list);
    void changevoice(int val);
    static void vlc_callback(const struct libvlc_event_t* p_event,void* p_data);//事件管理器的回调函数

//    libvlc_media_player_t* media_player() const{return _pmediaPlayer;};
//    qint64 durations() const{return _durationSec;};

//    void setTimeSliderValue(float value);
//    void setTimeText(QString str);
//    void setVolumeSliderValue(int val);
signals:
    void sigPlayItem(int index);


private slots:
    void on_btnOpen_clicked();
    void on_btnPlayer_clicked();
    void on_btnPause_clicked();
    void on_btnStop_clicked();
    void on_TimeSlider_sliderMoved(int position);
    void on_volumeSlider_sliderMoved(int position);
    void on_play_clicked();


private:
    Ui::VLCWidget *ui;
    VLCKits* _pVLCKits;
    //CVolumeButton * _pbutton=nullptr;
    QStringList _fileList={};
    bool _HasAdd=false;
    QGraphicsView* _graphicView;
    QGraphicsScene* _scene;
    QGraphicsPixmapItem* _pixmapItem=nullptr;



};
#endif // VLCWIDGET_H
