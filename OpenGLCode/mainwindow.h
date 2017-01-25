#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "firstshadercode.h"
#include "secondshadercode.h"
#include "textureshader.h"
#include "yuvtextureshader.h"
#include "yuvsphere.h"
#include "sphere.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent( QKeyEvent *e );
private:
    TextureShader* ts;
    YUVsphere* yuvsphere;
    YUVtextureShader* YUVts;
    sphere* sph;
    Ui::MainWindow *ui;
    QTimer* timer;
private slots:
    void setBrightnessValue(int value);
    void setContrastValue(int value);
    void setSaturationValue(int value);
    void setSopiaIntensityValue(int value);
    void on_isSepia_checkBox_clicked();
    void setZoomLabelValue();
    void on_sphere_checkBox_clicked();
};

#endif // MAINWINDOW_H
