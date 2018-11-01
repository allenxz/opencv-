#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv/cv.h>
#include <QTextCodec>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //载入公告牌
    QString filename = QFileDialog::getOpenFileName(this,tr("lena"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    billboard=imread(name);
    img=QImage((const uchar*)(billboard.data), billboard.cols, billboard.rows, billboard.cols * billboard.channels(),QImage::Format_RGB888);
    ui->labelView->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::on_pushButton_2_clicked()
{
    Mat src,dst;
    //定义原图像和转换图像
    Point2f dstPoint[4];
    //定义四个点为浮点型

    const char* filename = "C:\\Users\\13226\\Desktop\\vision industrille\\exp2\\exp22\\lena.jpg";
    //目标图像地址

    cv::imread(filename).copyTo(src);
    if (src.empty()) {
        throw("Faild open file.");
    }
    //图像读取成功或者失败判定


    Point2f srcPoint[4] = {
        Point(20,26),
        Point(20,494),
        Point(722,26),
        Point(722,494),
    };
    //定义透视点的坐标

    dstPoint[0] = Point(67,21);
    dstPoint[1] = Point(58,259);
    dstPoint[2] = Point(425,200);
    dstPoint[3] = Point(450,363);
    //图像的透视方向

    Mat perspectiveMmat = cv::getPerspectiveTransform(srcPoint,dstPoint);
    warpPerspective(src,dst,perspectiveMmat,src.size(),cv::INTER_CUBIC);
    //图像透视化的函数

    Mat final;
    cv::resize(dst,final,billboard.size(),(0, 0), (0, 0),INTER_LINEAR);
    Mat imageROI=billboard(Rect(0,0,final.cols,final.rows));
    addWeighted(imageROI,0.5,final,0.5,0.,imageROI);

    img=QImage((const uchar*)(billboard.data), billboard.cols, billboard.rows, billboard.cols * billboard.channels(),QImage::Format_RGB888);
    ui->labelView->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::mouseMoveEvent(QMouseEvent *m){
        QString msg=QString("%1,%2").arg(m->pos().x()).arg(m->pos().y());
        ui->label->setText(msg);
}


