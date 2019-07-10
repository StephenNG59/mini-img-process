#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img_process.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmap_GSW = new QPixmap(QString::fromUtf8("./img/a.png"));
    ui->label_imgFrame->setScaledContents(true);

    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_actionOpen_Image_clicked()));
    connect(ui->horizontalSlider_lightness, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_lightness_valueChanged(int)));
    connect(ui->horizontalSlider_contrast, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_contrast_valueChanged(int)));
    connect(ui->pushButton_showOrigin, SIGNAL(pressed()), this, SLOT(on_pushButton_showOrigin_pressed()));
    connect(ui->pushButton_showOrigin, SIGNAL(released()), this, SLOT(on_pushButton_showOrigin_released()));
    connect(ui->pushButton_gray, SIGNAL(clicked()), this, SLOT(on_pushButton_gray_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png, *.jpg)"));
    img_origin = new QImage(fileName);
    img_adjusted = new QImage(*img_origin);
    img_filtered = new QImage(*img_origin);
    pixmap_origin = new QPixmap(QPixmap::fromImage(*img_origin));
    pixmap_changed = new QPixmap(*pixmap_origin);
    ui->label_imgFrame->setPixmap(*pixmap_origin);

    // calculate average
    int height = img_origin->height(), width = img_origin->width();
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            QRgb rgb = img_origin->pixel(j, i);
            ave_r += qRed(rgb);
            ave_g += qGreen(rgb);
            ave_b += qBlue(rgb);
        }
    ave_r = int(float(ave_r) / (height * width));
    ave_g = int(float(ave_g) / (height * width));
    ave_b = int(float(ave_b) / (height * width));
    ave_gray = (ave_r * 153 + ave_g * 300 + ave_b * 59) >> 9;
}

void MainWindow::on_pushButton_test_clicked()
{
    QImage changed_img = lightnessFunc(img_origin);

    *pixmap_changed = QPixmap::fromImage(changed_img);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_horizontalSlider_lightness_valueChanged(int value)
{
    // value: 0~100, lightness ratio: 0.5 ~ 1.5
    QImage changed_img = lightnessFunc(img_origin, value / 100.0f + 0.5f);

    *pixmap_changed = QPixmap::fromImage(changed_img);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_horizontalSlider_contrast_valueChanged(int value)
{
    // value: 0~100, lightness ratio: 0.5 ~ 1.5
    QImage changed_img = contrastFunc(img_origin, ave_gray, value / 100.0f + 0.5f);

    *pixmap_changed = QPixmap::fromImage(changed_img);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_pushButton_gray_clicked()
{
    *img_filtered = grayFunc(img_origin);

    *pixmap_changed = QPixmap::fromImage(*img_filtered);
    ui->label_imgFrame->setPixmap(*pixmap_changed);

//    ui->pushButton_gray->setDown(!(ui->pushButton_gray->isDown()));
}

void MainWindow::on_pushButton_showOrigin_pressed()
{
    ui->label_imgFrame->setPixmap(*pixmap_origin);
}

void MainWindow::on_pushButton_showOrigin_released()
{
    ui->label_imgFrame->setPixmap(*pixmap_changed);
}
