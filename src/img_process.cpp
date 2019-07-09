#include "img_process.h"

QImage &testFunc(QImage *origin)
{
    assert(origin);

    QImage *changed = new QImage(*origin);

    int width = origin->width(), height = origin->height();
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            QRgb rgb = origin->pixel(i, j);
            int r = 0.5 * qRed(rgb), g = 0.5 * qGreen(rgb), b = 0.5 * qBlue(rgb);
            changed->setPixel(i, j, qRgb(r, g, b));
        }

    return *changed;
}
