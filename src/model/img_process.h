#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include <algorithm>
#include <memory>
#include <time.h>

#include <QPixmap>
#include <QMovie>
#include "../view/mainwindow.h"

void convolve2D(unsigned char ***src, unsigned char ***dst, float **kernel, int src_h, int src_w, int ker_h, int ker_w, int bias=0);

void convolve2D(unsigned char **src, unsigned char **dst, float **kernel, int src_h, int src_w, int ker_h, int ker_w, int bias=0);

void convolve3D(unsigned char ***src, unsigned char ***dst, float ***kernel, int src_h, int src_w, int ker_h, int ker_w);

void grayFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void sharpenFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void smoothFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void warmFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void coldFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void sketchFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

void sculptureFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width);

#endif // IMG_PROCESS_H
