#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include <algorithm>
#include <memory>
#include <time.h>

#include <QPixmap>
#include <QMovie>
#include <mainwindow.h>

void grayFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void edgeFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void smoothFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void convolve2D(unsigned char ***src, unsigned char ***dst, float **kernel, int src_h, int src_w, int ker_h, int ker_w);

#endif // IMG_PROCESS_H
