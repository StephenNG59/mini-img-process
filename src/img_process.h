#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include <QPixmap>
#include <QMovie>

QImage &testFunc(QImage *origin, float ratio=0.5);

QImage &gray(QImage *origin);

#endif // IMG_PROCESS_H
