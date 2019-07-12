#include "img_process.h"
#include "basic.h"

void updateAdjustedImage(MainWindow *mainWindow, float light, float contrast, float saturation)
{
    std::shared_ptr<QImage> origin = mainWindow->getOriginImage(), adjusted = mainWindow->getAdjustImage();
    int height = origin->height(), width = origin->width();
    int ave_light = mainWindow->getAveLight(), ave_saturation = mainWindow->getAveSaturation();

    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)origin->constScanLine(i);
        QRgb *p_adjusted = (QRgb *)adjusted->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int r, g, b, h, s, l;
            QColor(p_origin[j]).getHsl(&h, &s, &l);


            // Origin --> Contrast adjusted --> Saturation adjusted --> Lightness adjusted --> Adjusted
            // ----------------------------------------------------------------------------------------

            // 1. contrast
            l = clamp(int(ave_light + contrast * (l - ave_light)), 0, 255);
            // 2. saturation
            s = clamp(int(ave_saturation + saturation * (s - ave_saturation)), 0, 255);
            // 3. lightness
            QColor::fromHsl(h, s, l).getRgb(&r, &g, &b);
            r = clamp(int(light * r), 0, 255);
            g = clamp(int(light * g), 0, 255);
            b = clamp(int(light * b), 0, 255);
            p_adjusted[j] = qRgb(r, g, b);

        }
    }
}

void updateFilteredImage(MainWindow *mainWindow, void (*filterFunc)(std::shared_ptr<QImage>, std::shared_ptr<QImage>))
{
    std::shared_ptr<QImage> adjust = mainWindow->getAdjustImage(), filter = mainWindow->getFilterImage();

    filterFunc(adjust, filter);
}

/*
// lightnessFunc: multiply the origin image by ratio
QImage &lightnessFunc(std::shared_ptr<QImage> origin, float ratio=1.0)
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

QImage &contrastFunc(std::shared_ptr<QImage> origin, int &ave_light, float ratio)
{
    QImage *changed = new QImage(*origin);

    int height = origin->height(), width = origin->width();
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)origin->constScanLine(i);
        QRgb *p_changed = (QRgb *)changed->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int h, s, l;
            QColor(p_origin[j]).getHsl(&h, &s, &l);
            l = clamp(int(ave_light + ratio * (l - ave_light)), 0, 255);
            p_changed[j] = QColor::fromHsl(h, s, l).rgb();
        }
    }

    return *changed;
}

QImage &saturationFunc(std::shared_ptr<QImage> origin, int &ave_saturation, float ratio)
{
    QImage *changed = new QImage(*origin);

    int height = origin->height(), width = origin->width();
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_origin = (const QRgb *)origin->constScanLine(i);
        QRgb *p_changed = (QRgb *)changed->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int h, s, l;
            QColor(p_origin[j]).getHsl(&h, &s, &l);
            s = clamp(int(ave_saturation + ratio * (s - ave_saturation)), 0, 255);
            p_changed[j] = QColor::fromHsl(h, s, l).rgb();
        }
    }

    return *changed;
}*/

/* grayFunc: change the origin image to grayscale */
QImage &grayFunc( std::shared_ptr<QImage> image )
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

void grayFunc(std::shared_ptr<QImage> adjust, std::shared_ptr<QImage> filter)
{
    int height = adjust->height(), width = adjust->width();
    for (int i = 0; i < height; i++)
    {
        const QRgb *p_adjusted = (const QRgb *)adjust->constScanLine(i);
        QRgb *p_filtered = (QRgb *)filter->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            int gray = qGray(p_adjusted[j]);
            p_filtered[j] = qRgb(gray, gray, gray);
        }
    }
}
