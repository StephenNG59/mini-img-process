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

    connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(on_pushButton_test_clicked()));
    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_actionOpen_Image_clicked()));
    connect(ui->horizontalSlider_test, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->pushButton_showOrigin, SIGNAL(pressed()), this, SLOT(on_pushButton_showOrigin_pressed()));
    connect(ui->pushButton_showOrigin, SIGNAL(released()), this, SLOT(on_pushButton_showOrigin_released()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png, *.jpg)"));
    img_origin = new QImage(fileName);
    pixmap_origin = new QPixmap(QPixmap::fromImage(*img_origin));
    pixmap_changed = new QPixmap(*pixmap_origin);
    ui->label_imgFrame->setPixmap(*pixmap_origin);
//    QMovie *move_origin = new QMovie(fileName);
//    ui->label_imgFrame->setMovie(move_origin);
//    move_origin->start();
}

void MainWindow::on_pushButton_test_clicked()
{
    QImage changed_img = testFunc(img_origin);

    *pixmap_changed = QPixmap::fromImage(changed_img);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // value: 0~99
    QImage changed_img = testFunc(img_origin, float(100 - value) / 100);
//    ui->label_hello->setText(QString::fromStdString(std::to_string(value)));

    *pixmap_changed = QPixmap::fromImage(changed_img);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_pushButton_showOrigin_pressed()
{
    ui->label_imgFrame->setPixmap(*pixmap_origin);
}

void MainWindow::on_pushButton_showOrigin_released()
{
    ui->label_imgFrame->setPixmap(*pixmap_changed);
}
