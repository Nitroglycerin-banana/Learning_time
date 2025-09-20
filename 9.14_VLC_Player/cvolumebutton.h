#ifndef CVOLUMEBUTTON_H
#define CVOLUMEBUTTON_H

#include "cvolumedialog.h"

#include <QPushButton>

class CVolumeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CVolumeButton(QWidget *parent = nullptr);
    ~CVolumeButton();
    void hide();
    void setslider(int val);

signals:
    void sigvolume2(int val);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;

public:
    CVolumeDialog * _pvolumeSliderDlg;

};

#endif // CVOLUMEBUTTON_H
