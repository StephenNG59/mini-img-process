#include "img_process.h"

/* testFunc: multiply the origin image by ratio */
QImage &testFunc(QImage *origin, float ratio/*=0.5*/)
{
    assert(origin);

    QImage *changed = new QImage(*origin);

    int width = origin->width(), height = origin->height();
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            QRgb rgb = origin->pixel(i, j);
            int r = ratio * qRed(rgb), g = ratio * qGreen(rgb), b = ratio * qBlue(rgb);
            changed->setPixel(i, j, qRgb(r, g, b));
        }

    return *changed;
}
