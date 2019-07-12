#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img_process.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    img_origin = (std::make_shared<QImage>(QString::fromUtf8("./img/a.png")));
    img_adjusted = (std::make_shared<QImage>(*img_origin));
    img_filtered = (std::make_shared<QImage>(*img_origin));

    // With proper names, belows will result in connected-twice.
    /*
    connect(ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_actionOpen_Image_triggered()));
    connect(ui->horizontalSlider_lightness, SIGNAL(sliderReleased()), this, SLOT(on_horizontalSlider_lightness_sliderReleased()));
    connect(ui->horizontalSlider_contrast, SIGNAL(sliderReleased()), this, SLOT(on_horizontalSlider_contrast_sliderReleased()));
    connect(ui->horizontalSlider_saturation, SIGNAL(sliderReleased()), this, SLOT(on_horizontalSlider_saturation_sliderReleased()));
    connect(ui->pushButton_showOrigin, SIGNAL(pressed()), this, SLOT(on_pushButton_showOrigin_pressed()));
    connect(ui->pushButton_showOrigin, SIGNAL(released()), this, SLOT(on_pushButton_showOrigin_released()));
    connect(ui->pushButton_gray, SIGNAL(clicked()), this, SLOT(on_pushButton_gray_clicked()));
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetButtons()
{
    this->ui->pushButton_noFilter->setChecked(true);
    this->ui->horizontalSlider_lightness->setSliderPosition(50);
    this->ui->horizontalSlider_contrast->setSliderPosition(50);
    this->ui->horizontalSlider_saturation->setSliderPosition(50);
}

/* adjustImage(): apply contrast->saturation->light adjustment to filter image */
void MainWindow::adjustImage()
{
    float light = ui->horizontalSlider_lightness->value() / 100.0f + 0.5f,
            contrast = ui->horizontalSlider_contrast->value() / 100.0f + 0.5f,
            saturation = ui->horizontalSlider_saturation->value() / 50.0f;

    updateAdjustedImage(this, light, contrast, saturation);
    *img_filtered = *img_adjusted;

    if (this->ui->pushButton_noFilter->isChecked() == false)
    {
        if (this->ui->pushButton_gray->isChecked())
            updateFilteredImage(this, grayFunc);
    }

    *pixmap_changed = QPixmap::fromImage((*img_filtered).scaled(
                                             this->ui->label_imgFrame->width(),
                                             this->ui->label_imgFrame->height(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::filterImage(void (*filter)(std::shared_ptr<QImage>, std::shared_ptr<QImage>))
{
    updateFilteredImage(this, filter);

    *pixmap_changed = QPixmap::fromImage((*img_filtered).scaled(
                                             this->ui->label_imgFrame->width(),
                                             this->ui->label_imgFrame->height(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    ui->label_imgFrame->setPixmap(*pixmap_changed);
    ui->pushButton_close->setText("Test Done");
}

void MainWindow::on_actionOpen_Image_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png, *.jpg)"));
    if (fileName == nullptr) return;

    resetButtons();

    ui->label_imgFrame->setScaledContents(false);

    *img_origin = QImage(fileName);
    img_adjusted = std::make_shared<QImage>(*img_origin);
    img_filtered = std::make_shared<QImage>(*img_origin);
    pixmap_origin = new QPixmap(QPixmap::fromImage((*img_adjusted).scaled(
                                    this->ui->label_imgFrame->width(),
                                    this->ui->label_imgFrame->height(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation)));
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

void MainWindow::on_horizontalSlider_lightness_sliderReleased() { adjustImage(); }

void MainWindow::on_horizontalSlider_contrast_sliderReleased() { adjustImage(); }

void MainWindow::on_horizontalSlider_saturation_sliderReleased() { adjustImage(); }

void MainWindow::on_pushButton_noFilter_clicked()
{
    *pixmap_changed = QPixmap::fromImage((*img_adjusted).scaled(
                                             this->ui->label_imgFrame->width(),
                                             this->ui->label_imgFrame->height(),
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    ui->label_imgFrame->setPixmap(*pixmap_changed);
}

void MainWindow::on_pushButton_gray_clicked() { filterImage(grayFunc); }

void MainWindow::on_pushButton_showOrigin_pressed()
{
    ui->label_imgFrame->setPixmap(*pixmap_origin);
}

void MainWindow::on_pushButton_showOrigin_released()
{
    ui->label_imgFrame->setPixmap(*pixmap_changed);
}
