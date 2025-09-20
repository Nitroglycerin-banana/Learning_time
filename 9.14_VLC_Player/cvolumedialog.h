#ifndef CVOLUMESLIDERDIALOG_H
#define CVOLUMESLIDERDIALOG_H
#include <QDialog>
#include <QSlider>

class CVolumeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CVolumeDialog(QWidget * parent = Q_NULLPTR);
    ~CVolumeDialog();
    void setslider(int val);
signals:
    void sigvolume(int val);


public:
    QSlider * _pslider = nullptr;

};

#endif // CVOLUMESLIDERDIALOG_H
