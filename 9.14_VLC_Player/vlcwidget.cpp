#include "vlcwidget.h"
#include "cframelesswidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QListIterator>
#include <QKeyEvent>
#include <string>
#include <QVBoxLayout>

using std::string;

VLCWidget::VLCWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VLCWidget)
    ,_pVLCKits(new VLCKits)
    //,_pbutton(new CVolumeButton(this))
    //,_pTitleBar(new CTitleBar(this))
{
    ui->setupUi(this);
    //ui->horizontalLayout->addWidget(_pbutton);
    bool ret=_pVLCKits->initVLC();
    if(!ret)
    {
        QMessageBox::information(this,"提示","libvlc init failed");
        exit(EXIT_FAILURE);
    }
    //视频位置（帧）改变时，触发进度条移动信号
    connect(_pVLCKits,&VLCKits::sigTimeSliderPos,[=](int value){
        ui->TimeSlider->setValue(value);
    });
    //视频位置（帧）改变时，触发时间显示改变信号
    connect(_pVLCKits,&VLCKits::sigTimeText,[=](QString str){
       ui->Time->setText(str);
    });
    //音量改变时，触发改变音量条位置信号
    connect(_pVLCKits,&VLCKits::sigVolumeSliderPos,[=](int val){
       ui->volumeSlider->setValue(val);//改变音量条大小

    });
    //音量改变，触发信号改变按钮图标
    connect(_pVLCKits,&VLCKits::sigChangeVoice,this,&VLCWidget::changevoice);

    //当音量条滑动时调节系统音量
    connect(this,&VLCWidget::on_volumeSlider_sliderMoved,[=](int val){
        _pVLCKits->setVolume(val);

        //VolumeChange(val);
    });

    //当双击播放列表项时，触发获取当前播放文件索引信号
    connect(ui->PlayList,&QListWidget::itemDoubleClicked,[=](QListWidgetItem* item){
        int index=ui->PlayList->row(item);
        emit sigPlayItem(index);
    });
    //获取索引后触发播放列表中对应文件信号
    connect(this,&VLCWidget::sigPlayItem,[=](int index){
        _pVLCKits->itemPlay(index);
    });
    connect(_pVLCKits,&VLCKits::sigWillEnd,[=](bool flag){
        if(flag){
        ui->Time->setStyleSheet(
                    "QLabel{"
                    "color:red"
                    "}");
        }
        else{
            ui->Time->setStyleSheet(
                        "QLabel{"
                        "color:white"
                        "}");
        }
    });
    _pVLCKits->setVolume(50);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinMaxButtonsHint);
    QPalette color;
    color.setColor(QPalette::Window,QColor("#1e272e"));
    this->setPalette(color);
    this->setAutoFillBackground(true);
    ui->PlayList->setStyleSheet(
                "QListWidget {"
                "background-color:#485460;"
                "color:white;"
                "}"
                "QListWidget::item{"
                "font-weight: bold;"             // 列表项字体加粗
                "}"
                "QListWidget::item:selected{"
                "background-color:#485460;"
                "color:orange;"
                "}"
                );
    ui->Time->setStyleSheet(
                "QLabel{"
                "color:white"
                "}"
                );
    ui->btnOpen->setStyleSheet(
                "QPushButton{"
                "background-color:#3c40c6;"
                "color:white"
                "}"
                );
    ui->btnPause->setStyleSheet(
                "QPushButton{"
                "background-color:#ffc048;"
                "color:white"
                "}"
                );
    ui->btnPlayer->setStyleSheet(
                "QPushButton{"
                "background-color:#05c46b;"
                "color:white"
                "}"
                );
    ui->btnStop->setStyleSheet(
                "QPushButton{"
                "background-color:#ff3f34;"
                "color:white"
                "}"
                );

    ui->voice->setStyleSheet(
                "QPushButton{background-image:url(:/volumeST.png);border:none}");


    ui->play->setStyleSheet(
            "QPushButton{background-image:url(:/play.png);border:none}"
            "QPushButton:hover{"
                    "background-image:url(:/play_after.png);border:none;}");



//    ui->graphicsView->setStyleSheet(
//                " //background-color: transparent;border:none;");
//    _scene=new QGraphicsScene(this);
//    _scene->setBackgroundBrush(Qt::NoBrush);
//    ui->graphicsView->setScene(_scene);
//    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setAttribute(Qt::WA_TranslucentBackground,true);





}

VLCWidget::~VLCWidget()
{
    delete ui;
    qDebug()<<"~VLCWidget()";
}

void VLCWidget::hide(bool set)
{
    if(set){
    ui->PlayList->hide();
    }
    else{
    ui->PlayList->show();
        };
    //ui->btnOpen->hide();
    //ui->btnStop->hide();
    //ui->btnPause->hide();
    //ui->btnPlayer->hide();
    //ui->volumeSlider->hide();

}


//列表去重
void VLCWidget::UnRepeat(QStringList& list)
{
    bool hasDuplicate = false;
    //完成列表去重
    for(int i=0;i<list.size();++i){
        for(int j=list.size()-1;j>i;--j)
        {
            if(list[i]==list[j])
            {
                hasDuplicate=true;
                list.erase(list.begin()+j);
            }
        }
    }
    if (hasDuplicate) {
            // 显示添加重复文件弹窗
            QMessageBox::information(this, "提示", "请勿添加重复文件!");
    }
}

void VLCWidget::on_play_clicked()
{
    qDebug()<<"on_btn_clicked";
    QStringList filelist = QFileDialog::getOpenFileNames(this, "请选择视频文件", "D:/",
                                                       "视频文件(*.mp4);;"
                                                       "视频文件(*.avi);;");
    qDebug()<<"filename:"<<filelist;
    if(filelist.isEmpty())
    {
        return;
    }


    for(const QString& file:filelist)
    {
        _fileList.push_back(file);
    }
    //给媒体播放列表去重
    UnRepeat(_fileList);
    //检查并添加到播放列表中
    for(const QString& file:_fileList)
    {
      bool isDuplicate=false;
      if(_HasAdd)
      {
          for(int j=0;j<ui->PlayList->count();++j)
          {
              if(file==ui->PlayList->item(j)->text())
              {
                  isDuplicate=true;
                  break;
              }
          }
      }
      if(!isDuplicate)
      {
          ui->PlayList->addItem(file);
      }
    }
    _HasAdd=true;
    delete  ui->play;

    bool ret=_pVLCKits->play(_fileList,(void*)ui->videoWidget->winId());
    if(!ret)
    {
        QMessageBox::information(this,"提示","libvlc_new failed!");
        return;
    }

}


void VLCWidget::on_btnOpen_clicked()
{
    qDebug()<<"on_btn_clicked";
    QStringList filelist = QFileDialog::getOpenFileNames(this, "请选择视频文件", "D:/",
                                                       "视频文件(*.mp4);;"
                                                       "视频文件(*.avi);;");
    qDebug()<<"filename:"<<filelist;
    if(filelist.isEmpty())
    {
        return;
    }


    for(const QString& file:filelist)
    {
        _fileList.push_back(file);
    }
    //给媒体播放列表去重
    UnRepeat(_fileList);
    //检查并添加到播放列表中
    for(const QString& file:_fileList)
    {
      bool isDuplicate=false;
      if(_HasAdd)
      {
          for(int j=0;j<ui->PlayList->count();++j)
          {
              if(file==ui->PlayList->item(j)->text())
              {
                  isDuplicate=true;
                  break;
              }
          }
      }
      if(!isDuplicate)
      {
          ui->PlayList->addItem(file);
      }
    }
    _HasAdd=true;

    bool ret=_pVLCKits->play(_fileList,(void*)ui->videoWidget->winId());
    if(!ret)
    {
        QMessageBox::information(this,"提示","libvlc_new failed!");
        return;
    }
//    int nret = SetTimer(NULL, 1, 300, TimeProc);//全屏模式相关
}


void VLCWidget::on_btnPlayer_clicked()
{
    qDebug()<<"on_btn_clicked";
    _pVLCKits->play();
    _pixmapItem->hide();
    ui->play->hide();

}


void VLCWidget::on_btnPause_clicked()
{

    _pVLCKits->pause();
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    ui->videoWidget->setAttribute(Qt::WA_TranslucentBackground,true);

    QPixmap pixmap(":/Stop.png");

//    if(!pixmap.isNull())
//    {
//       _pixmapItem=_scene->addPixmap(pixmap);
//       _pixmapItem->setFlag(QGraphicsItem::ItemIsMovable,false);
//       qDebug()<<"on_btnPause_clicked";
//    }



}

void VLCWidget::on_btnStop_clicked()
{
    qDebug()<<"on_btnStop_clicked";
    _pVLCKits->stop();
}



void VLCWidget::on_TimeSlider_sliderMoved(int position)
{
    _pVLCKits->setPosition(position);
}


void VLCWidget::on_volumeSlider_sliderMoved(int position)
{
    _pVLCKits->setVolume(position);
}

void VLCWidget::changevoice(int val)
{
    if(val==0&&val<25){
        ui->voice->setStyleSheet(
                    "QPushButton{background-image:url(:/closevolume.png);border:none}");
    }
    if(val>25&&val<50){
        ui->voice->setStyleSheet(
                    "QPushButton{background-image:url(:/volumeST.png);border:none}");
    }
    if(val>50&&val<75){
        ui->voice->setStyleSheet(
                    "QPushButton{background-image:url(:/volumeND.png);border:none}");
    }
    if(val>75&&val<99)
    {
        ui->voice->setStyleSheet(
                    "QPushButton{background-image:url(:/volumeRD.png);border:none}");
    }
    if(val==99)
    {
        ui->voice->setStyleSheet(
                    "QPushButton{background-image:url(:/volumeRD2.png);border:none}");
    }

}


