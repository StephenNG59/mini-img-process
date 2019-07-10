#include "img_process.h"
#include "basic.h"

/* lightnessFunc: multiply the origin image by ratio */
QImage &lightnessFunc(QImage *origin, float ratio/*=1.0*/)
{
    QImage *changed = new QImage(*origin);

    int height = origin->height(), width = origin->width();
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)origin->constScanLine(i);
        QRgb *p_changed = (QRgb *)changed->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int r = std::min(255, int(qRed(p_origin[j]) * ratio)),
                    g = std::min(255, int(qGreen(p_origin[j]) * ratio)),
                    b = std::min(255, int(qBlue(p_origin[j]) * ratio));
            p_changed[j] = qRgb(r, g, b);
        }
    }
    return *changed;
}

QImage &contrastFunc(QImage *origin, int &ave_gray, float ratio/*=1.0*/)
{
    QImage *changed = new QImage(*origin);

    int height = origin->height(), width = origin->width();
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)origin->constScanLine(i);
        QRgb *p_changed = (QRgb *)changed->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int r = std::min(255, int(qRed(p_origin[j]) * ratio)),
                    g = std::min(255, int(qGreen(p_origin[j]) * ratio)),
                    b = std::min(255, int(qBlue(p_origin[j]) * ratio));
            p_changed[j] = qRgb(r, g, b);
        }
    }

    return *changed;
}

/* grayFunc: change the origin image to grayscale */
QImage &grayFunc( QImage *image )
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
            const uchar *pSrc = (const uchar *)image->constScanLine(i);
            uchar *pDest = (uchar *)ret->scanLine(i);
            memcpy(pDest, pSrc, size_t(width));
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (const QRgb *)image->constScanLine(i);
            uchar *pDest = (uchar *)ret->scanLine(i);
//QRgb *pDest = (QRgb*)image->scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = uchar(qGray(pSrc[j]));
//((unsigned char*)&pDest[j])[3] = 56;
            }
        }
        break;
    default:
        break;
    }
    return *ret;
}
