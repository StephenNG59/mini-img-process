#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    QImage *img_origin, *img_adjusted, *img_filtered;
    QPixmap *pixmap_origin, *pixmap_changed;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int ave_r = 0, ave_g = 0, ave_b = 0, ave_light = 0, ave_saturation = 0;

private slots:
    // self-defined slots
    void on_actionOpen_Image_clicked();
    void on_pushButton_test_clicked();
    void on_horizontalSlider_lightness_valueChanged(int value);
    void on_horizontalSlider_contrast_sliderReleased();
    void on_horizontalSlider_saturation_sliderReleased();
    void on_pushButton_showOrigin_pressed();
    void on_pushButton_showOrigin_released();
    void on_pushButton_gray_clicked();
};

#endif // MAINWINDOW_H
