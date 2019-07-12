#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString fileName;
    QPixmap *pixmap_GSW;
    std::shared_ptr<QImage> img_origin, img_adjusted, img_filtered;
    QPixmap *pixmap_origin, *pixmap_changed;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int ave_r = 0, ave_g = 0, ave_b = 0, ave_light = 0, ave_saturation = 0;
    void resetButtons();
    void adjustImage();
    void filterImage(void (*filter)(std::shared_ptr<QImage>, std::shared_ptr<QImage>));

private slots:
    // self-defined slots
    void on_actionOpen_Image_triggered();
    void on_horizontalSlider_lightness_sliderReleased();
    void on_horizontalSlider_contrast_sliderReleased();
    void on_horizontalSlider_saturation_sliderReleased();
    void on_pushButton_showOrigin_pressed();
    void on_pushButton_showOrigin_released();
    void on_pushButton_gray_clicked();

public:
    std::shared_ptr<QImage> getOriginImage() { return this->img_origin; }
    std::shared_ptr<QImage> getAdjustImage() { return this->img_adjusted; }
    std::shared_ptr<QImage> getFilterImage() { return this->img_filtered; }
    int getAveLight() { return this->ave_light; }
    int getAveSaturation() { return this->ave_saturation; }
};

#endif // MAINWINDOW_H
