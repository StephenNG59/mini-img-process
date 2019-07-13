#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include <algorithm>
#include <memory>
#include <QPixmap>
#include <QMovie>
#include <mainwindow.h>

void updateAdjustedImage(MainWindow *mainWindow, float light, float contrast, float saturation);

void updateFilteredImage(MainWindow *mainWindow, void (*filterFunc)(std::shared_ptr<QImage>, std::shared_ptr<QImage>));

void updateFilteredData(MainWindow *mainWindow, void (*filterFunc)(unsigned char ***, unsigned char ***));

QImage &lightnessFunc(std::shared_ptr<QImage> origin, float ratio=1.0);

QImage &contrastFunc(std::shared_ptr<QImage> origin, int &ave_light, float ratio=1.0);

QImage &saturationFunc(std::shared_ptr<QImage> origin, int &ave_light, float ratio=1.0);

QImage &grayFunc(std::shared_ptr<QImage> origin);

void grayFunc(std::shared_ptr<QImage> adjust, std::shared_ptr<QImage> filter);

void grayFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void edgeFunc(std::shared_ptr<QImage> adjust, std::shared_ptr<QImage> filter);

void conv3x3(QRgb mat[3][3], float kernel[3][3], int *r, int *g, int *b);

#endif // IMG_PROCESS_H
