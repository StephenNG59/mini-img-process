#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "img_process.h"
#include "basic.h"


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

void MainWindow::storeImage()
{
    *img_origin = QImage(fileName);
    img_adjusted = std::make_shared<QImage>(*img_origin);
    img_filtered = std::make_shared<QImage>(*img_origin);

    int height = img_origin->height(), width = img_origin->width(), channel = 4;
    data_origin = new unsigned char**[height];
    data_adjusted = new unsigned char**[height];
    data_filtered = new unsigned char**[height];
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)img_origin->constScanLine(i);
        data_origin[i] = new unsigned char*[width];
        data_adjusted[i] = new unsigned char*[width];
        data_filtered[i] = new unsigned char*[width];
        for (int j = 0; j < width; j++)
        {
            data_origin[i][j] = new unsigned char[channel];
            data_adjusted[i][j] = new unsigned char[channel];
            data_filtered[i][j] = new unsigned char[channel];
            data_origin[i][j][0] = data_adjusted[i][j][0] = data_filtered[i][j][0] = (unsigned char)qRed(p_origin[j]);
            data_origin[i][j][1] = data_adjusted[i][j][1] = data_filtered[i][j][1] = (unsigned char)qGreen(p_origin[j]);
            data_origin[i][j][2] = data_adjusted[i][j][2] = data_filtered[i][j][2] = (unsigned char)qBlue(p_origin[j]);
            data_origin[i][j][3] = data_adjusted[i][j][3] = data_filtered[i][j][3] = (unsigned char)qAlpha(p_origin[j]);
        }
    }
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

void MainWindow::adjustData()
{
    float light = ui->horizontalSlider_lightness->value() / 100.0f + 0.5f,
            contrast = ui->horizontalSlider_contrast->value() / 100.0f + 0.5f,
            saturation = ui->horizontalSlider_saturation->value() / 50.0f;
    int height = this->img_origin->height(), width = this->img_origin->width();
    int ave_light = this->ave_light, ave_saturation = this->ave_saturation;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int h, s, l;
            int r = data_origin[i][j][0], g = data_origin[i][j][1], b = data_origin[i][j][2];
            QColor::fromRgb(r, g, b).getHsl(&h, &s, &l);

            // Origin --> Contrast adjusted --> Saturation adjusted --> Lightness adjusted --> Adjusted
            // ----------------------------------------------------------------------------------------

            // 1. contrast
            l = clamp(int(ave_light + contrast * (l - ave_light)), 0, 255);
            // 2. saturation
            s = clamp(int(ave_saturation + saturation * (s - ave_saturation)), 0, 255);
            // 3. lightness
            QColor::fromHsl(h, s, l).getRgb(&r, &g, &b);
            r = clamp(int(light * r), 0, 255);
            g = clamp(int(light * g), 0, 255);
            b = clamp(int(light * b), 0, 255);

            // results
            data_adjusted[i][j][0] = (unsigned char)r;
            data_adjusted[i][j][1] = (unsigned char)g;
            data_adjusted[i][j][2] = (unsigned char)b;
        }
    }

    updateImgFromData(img_adjusted, data_adjusted);

    if (this->ui->pushButton_noFilter->isChecked() == false)
    {
        if (this->ui->pushButton_gray->isChecked())
            updateFilteredImage(this, grayFunc);
    }

//    updateImgFromData(img_filtered, data_filtered);

    updatePixmapFromImg(pixmap_changed, img_filtered);
    ui->label_imgFrame->setPixmap(*pixmap_changed);
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

void MainWindow::filterData(void (*filter)(unsigned char ***, unsigned char ***, int, int), int height, int width)
{
    filter(this->data_adjusted, this->data_filtered, height, width);
}

void MainWindow::updateImgFromData(std::shared_ptr<QImage> img, unsigned char ***data)
{
    int height = img->height(), width = img->width();
    for (int i = 0; i < height; i++)
    {
        QRgb *p_img = (QRgb *)img->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            p_img[j] = qRgb(data[i][j][0], data[i][j][1], data[i][j][2]);
        }
    }
}

void MainWindow::updatePixmapFromImg(QPixmap *pixmap, std::shared_ptr<QImage> img)
{
    *pixmap = QPixmap::fromImage(img->scaled(
                                     this->ui->label_imgFrame->width(),
                                     this->ui->label_imgFrame->height(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
}

void MainWindow::on_actionOpen_Image_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png, *.jpg)"));
    if (fileName == nullptr) return;

    resetButtons();

    ui->label_imgFrame->setScaledContents(false);

    storeImage();

    pixmap_origin = new QPixmap(QPixmap::fromImage((*img_origin).scaled(
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

void MainWindow::on_pushButton_showOrigin_pressed()
{
    ui->label_imgFrame->setPixmap(*pixmap_origin);
}

void MainWindow::on_pushButton_showOrigin_released()
{
    ui->label_imgFrame->setPixmap(*pixmap_changed);
}

void MainWindow::on_horizontalSlider_lightness_sliderReleased() { adjustData(); }

void MainWindow::on_horizontalSlider_contrast_sliderReleased() { adjustData(); }

void MainWindow::on_horizontalSlider_saturation_sliderReleased() { adjustData(); }

void MainWindow::on_pushButton_noFilter_clicked()
{
    updatePixmapFromImg(pixmap_changed, this->img_adjusted);
//    *pixmap_changed = QPixmap::fromImage(this->img_adjusted->scaled(
//                                             this->ui->label_imgFrame->width(),
//                                             this->ui->label_imgFrame->height(),
//                                             Qt::KeepAspectRatio,
//                                             Qt::SmoothTransformation));
    this->ui->label_imgFrame->setPixmap(*pixmap_changed);
}

void MainWindow::on_pushButton_gray_clicked()
{
    /*filterImage(grayFunc);*/
    filterData(grayFunc, this->img_origin->height(), this->img_origin->width());
    updateImgFromData(this->img_filtered, this->data_filtered);
    updatePixmapFromImg(this->pixmap_changed, this->img_filtered);
    this->ui->label_imgFrame->setPixmap(*pixmap_changed);
}

void MainWindow::on_pushButton_edge_clicked() { filterImage(edgeFunc); }
