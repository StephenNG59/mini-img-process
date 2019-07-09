#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img_process.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmap_GSW = new QPixmap(QString::fromUtf8("./img/a.png"));
//    ui->label_imgFrame->setPixmap(*pixmap_GSW);
    ui->label_imgFrame->setScaledContents(true);

    connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(on_pushButton_test_clicked()));
    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_actionOpen_Image_clicked()));

//    pixmap_GSW.trueMatrix()
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png, *.jpg)"));

    QMovie *move = new QMovie(fileName);
    ui->label_imgFrame->setMovie(move);
    move->start();
}

void MainWindow::on_pushButton_test_clicked()
{
    QImage *img = new QImage(fileName);
//    QImage *changed_img = new QImage(*img);
//    for (int i = 0; i < 100; i++)
//        for (int j = 0; j < 100; j++)
//        {
//            QRgb rgb = img->pixel(i, j);
//            int r = 0.5 * qRed(rgb), g = 0.5 * qGreen(rgb), b = 0.5 * qBlue(rgb);
//            changed_img->setPixel(i, j, qRgb(r, g, b));
//        }
    QImage changed_img = testFunc(img);

    ui->label_imgFrame->setPixmap(QPixmap::fromImage(changed_img));
//    pixmap_GSW = new QPixmap(QString::fromUtf8("./img/a.png"));
//    ui->label_imgFrame->setPixmap(*pixmap_GSW);
    ui->pushButton_close->setText("Test Done");
}
