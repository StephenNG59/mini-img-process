#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMovie>
#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPixmap *pixmap_GSW;
    QImage *img_origin;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString fileName;

private slots:
    // self-defined slots
    void on_actionOpen_Image_clicked();
    void on_pushButton_test_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_help_clicked();
};

#endif // MAINWINDOW_H
