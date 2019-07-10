#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img_process.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    img_origin = new QImage(QString::fromUtf8("./img/a.png"));
    img_adjusted = new QImage(*img_origin);
    img_filtered = new QImage(*img_origin);

    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_actionOpen_Image_clicked()));
    connect(ui->horizontalSlider_lightness, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_lightness_valueChanged(int)));
    connect(ui->horizontalSlider_contrast, SIGNAL(sliderReleased()), this, SLOT(on_horizontalSlider_contrast_sliderReleased()));
    connect(ui->horizontalSlider_saturation, SIGNAL(sliderReleased()), this, SLOT(on_horizontalSlider_saturation_sliderReleased()));
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
    if (fileName == nullptr) return;

    ui->label_imgFrame->setScaledContents(false);

    *img_origin = (new QImage(fileName))->scaled(ui->label_imgFrame->width(), ui->label_imgFrame->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    *img_origin = img_origin->scaled(ui->label_imgFrame->width(), ui->label_imgFrame->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
    QColor ave_color = QColor::fromRgb(ave_r, ave_g, ave_b);
    ave_light = ave_color.lightness();
    ave_saturation = ave_color.hslSaturation();
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
    *img_adjusted = lightnessFunc(img_origin, value / 100.0f + 0.5f);

    *pixmap_changed = QPixmap::fromImage(*img_adjusted);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_horizontalSlider_contrast_sliderReleased()
{
    // value: 0~100, contrast ratio: 0.5 ~ 1.5
    int value = this->ui->horizontalSlider_contrast->value();
    *img_adjusted = contrastFunc(img_origin, ave_light, value / 100.0f + 0.5f);

    *pixmap_changed = QPixmap::fromImage(*img_adjusted);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_horizontalSlider_saturation_sliderReleased()
{
    // value: 0~100, saturation ratio: 0 ~ 2
    int value = this->ui->horizontalSlider_saturation->value();
    *img_adjusted = saturationFunc(img_origin, ave_saturation, value / 50.0f);

    *pixmap_changed = QPixmap::fromImage(*img_adjusted);
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
