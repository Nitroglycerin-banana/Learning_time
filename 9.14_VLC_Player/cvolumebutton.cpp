#include "cvolumebutton.h"
#include <QDebug>
#include <QStylePainter>
#include <QStyleOptionButton>
#include "vlcwidget.h"


CVolumeButton::CVolumeButton(QWidget *parent)
    : QPushButton(parent)
    , _pvolumeSliderDlg(new CVolumeDialog(this))
{

    connect(_pvolumeSliderDlg,&CVolumeDialog::sigvolume,[=](int val){
        emit sigvolume2(val);

    });
    this->setFixedSize(32,32);

    setStyleSheet("QPushButton{background-image:url(:/resources/audio_open.svg);border:none;}"
        "QPushButton:hover{background-image:url(:/resources/audio_open_hover.svg);border:none;}"
        "QPushButton:pressed{background-image:url(:/resources/audio_open.svg);border:none;}");
}

CVolumeButton::~CVolumeButton()
{}

void CVolumeButton::enterEvent(QEvent *event)
{
    qDebug() << "CVolumeButton::enterEvent()";
    if (!_pvolumeSliderDlg)
        _pvolumeSliderDlg = new CVolumeDialog(this);

    QPoint p1 = this->mapToGlobal(QPoint(0, 0));  //声音按钮左上角相对于屏幕的绝对位置
    QRect rect1 = this->rect();
    QRect rect2 = _pvolumeSliderDlg->rect();     //rect包含标题栏，去掉标题栏后height不变

    int x = p1.x() + (rect1.width() - rect2.width()) / 2;
    int y = p1.y() - rect2.height() - 5;
    _pvolumeSliderDlg->move(x, y);   //move是相对于桌面原点的位置
    _pvolumeSliderDlg->show();
}


void CVolumeButton::paintEvent(QPaintEvent*)
{
    qDebug() << "CVolumeButton::paintEvent";
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    p.drawControl(QStyle::CE_PushButton, option);
}



void CVolumeButton::hide()
{
    if(_pvolumeSliderDlg) {
        //根据鼠标的位置判断音量大小窗口是否消失
        QPoint mousePos = QCursor::pos();//获取鼠标的位置
        QRect rect = _pvolumeSliderDlg->geometry();
        if(!rect.contains(mousePos) && _pvolumeSliderDlg->isVisible()) {
            _pvolumeSliderDlg->hide();
            qDebug() << ">> hide...";
        }
    }
}


void CVolumeButton::setslider(int val)
{
    //qDebug()<<"==========";
    _pvolumeSliderDlg->setslider(val);
}
