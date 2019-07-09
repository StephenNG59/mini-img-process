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
    QPixmap *pixmap_GSW;

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
};

#endif // MAINWINDOW_H
