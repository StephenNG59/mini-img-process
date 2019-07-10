#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include <algorithm>
#include <QPixmap>
#include <QMovie>

QImage &lightnessFunc(QImage *origin, float ratio=1.0);

QImage &contrastFunc(QImage *origin, int &ave_gray, float ratio=1.0);

QImage &grayFunc(QImage *origin);

#endif // IMG_PROCESS_H
