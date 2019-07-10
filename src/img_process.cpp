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

/* gray: change the origin image to grayscale */
QImage &gray( QImage *image )
{
    int height = image->height();
    int width = image->width();
    QImage *ret = new QImage(width, height, QImage::Format_Indexed8);
    ret->setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret->setColor(i, qRgb(i, i, i));
    }
    switch(image->format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image->constScanLine(i);
            uchar *pDest = (uchar *)ret->scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image->constScanLine(i);
            uchar *pDest = (uchar *)ret->scanLine(i);
//QRgb *pDest = (QRgb*)image->scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
//((unsigned char*)&pDest[j])[3] = 56;
            }
        }
        break;
    default:
        break;
    }
    return *ret;
}
