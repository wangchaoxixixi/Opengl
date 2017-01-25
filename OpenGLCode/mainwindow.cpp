#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "firstshadercode.h"
#include "secondshadercode.h"
#include "textureshader.h"
#include "yuvtextureshader.h"
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //FirstShaderCode* fsc = new FirstShaderCode();
    //ui->scrollArea->setWidget(fsc);

    //SecondShaderCode* ssc = new SecondShaderCode();
    //ui->scrollArea->setWidget(ssc);

    ts = new TextureShader();
    ui->scrollArea->setWidget(ts);

    //YUVts = new YUVtextureShader();
    //ui->scrollArea->setWidget(YUVts);

    yuvsphere = new YUVsphere();
    //ui->scrollArea->setWidget(yuvsphere);

    //sph = new sphere();
    //ui->scrollArea->setWidget(sph);

    ui->brightness_Slider->setMinimum(-100);
    ui->brightness_Slider->setMaximum(100);
    ui->brightness_Slider->setValue(0);
    connect(ui->brightness_Slider, SIGNAL(valueChanged(int)), this, SLOT(setBrightnessValue(int)));
    ui->contrast_Slider->setMinimum(-100);
    ui->contrast_Slider->setMaximum(100);
    ui->contrast_Slider->setValue(0);
    connect(ui->contrast_Slider, SIGNAL(valueChanged(int)), this, SLOT(setContrastValue(int)));
    ui->saturation_Slider->setMinimum(-100);
    ui->saturation_Slider->setMaximum(100);
    ui->saturation_Slider->setValue(0);
    connect(ui->saturation_Slider, SIGNAL(valueChanged(int)), this, SLOT(setSaturationValue(int)));
    ui->sepia_Slider->setMinimum(-100);
    ui->sepia_Slider->setMaximum(100);
    ui->sepia_Slider->setValue(0);
    connect(ui->sepia_Slider, SIGNAL(valueChanged(int)), this, SLOT(setSopiaIntensityValue(int)));
    timer = new QTimer(this);
    timer->setInterval(30); // 暂定间隔30ms
    //connect(sphere, SIGNAL(zoomChanged()), this, SLOT(setZoomLabelValue()));
    connect(timer, SIGNAL(timeout()), this, SLOT(setZoomLabelValue()));
    timer->start();
    ui->sphere_checkBox->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
    case Qt::Key_W:
        /*
        ts->mixValue += 0.1f;
        if (ts->mixValue >= 1.0f)
          ts->mixValue = 1.0f;
        ts->updateGL();*/
        if(YUVts->brightness>1.0)
        {
            YUVts->brightness = 1.0;
        }
        YUVts->brightness +=0.05f;
        YUVts->updateGL();
        break;
    case Qt::Key_S:
        /*
        ts->mixValue -= 0.1f;
        if (ts->mixValue <= 0.0f)
          ts->mixValue = 0.0f;
        ts->updateGL();*/
        YUVts->brightness -=0.05f;
        if(YUVts->brightness<-1.0)
        {
            YUVts->brightness = -1.0;
        }
        YUVts->updateGL();
        break;
  }
}
void MainWindow::setBrightnessValue(int value)
{
    YUVts->brightness =(float)value/100.0;
    if(YUVts->brightness<-1.0)
    {
        YUVts->brightness = -1.0;
    }
    if(YUVts->brightness>1.0)
    {
        YUVts->brightness = 1.0;
    }
    YUVts->updateGL();
}
void MainWindow::setContrastValue(int value)
{
    if(value>=0)
    {
        YUVts->contrast = float(value)/100.0+1.0;
    }
    else
    {
        YUVts->contrast = float(100 + value)/100.0;
    }
    if(YUVts->contrast<-100.0)
    {
        YUVts->contrast = 0.0;
    }
    if(YUVts->contrast>100.0)
    {
        YUVts->contrast = 2.0;
    }
    YUVts->updateGL();
}
void MainWindow::setSaturationValue(int value)
{
    if(value>=0)
    {
        YUVts->saturation = float(value)/100.0+1.0;
    }
    else
    {
        YUVts->saturation = float(100 + value)/100.0;
    }
    if(YUVts->saturation<-100.0)
    {
        YUVts->saturation = 0.0;
    }
    if(YUVts->saturation>100.0)
    {
        YUVts->saturation = 2.0;
    }
    YUVts->updateGL();
}
void MainWindow::on_isSepia_checkBox_clicked()
{
    if(ui->isSepia_checkBox->isChecked())
    {
        YUVts->isSepia = 1;
    }
    else
    {
        YUVts->isSepia = 0;
    }
    YUVts->updateGL();
}
void MainWindow::setSopiaIntensityValue(int value)
{
    if(value>=0)
    {
        YUVts->sepiaIntensity = float(value)/100.0+1.0;
    }
    else
    {
        YUVts->sepiaIntensity = float(100 + value)/100.0;
    }
    if(YUVts->sepiaIntensity<-100.0)
    {
        YUVts->sepiaIntensity = 0.0;
    }
    if(YUVts->sepiaIntensity>100.0)
    {
        YUVts->sepiaIntensity = 2.0;
    }
    YUVts->updateGL();
}
void MainWindow::setZoomLabelValue()
{
    ui->zoom_label->setText(QString::number(yuvsphere->zoom));
}
void MainWindow::on_sphere_checkBox_clicked()
{
    if(ui->sphere_checkBox->checkState() == Qt::Checked)
        this->sph->drawSphere = true;
    else
        this->sph->drawSphere = false;
    //sph->update();
}
