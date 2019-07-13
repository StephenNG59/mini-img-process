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

void updateFilteredData(MainWindow *mainWindow, void (*filterFunc)(unsigned char ***, unsigned char ***))
{
    unsigned char ***adjust = mainWindow->getAdjustData(), ***filter = mainWindow->getFilterData();

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

void grayFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width)
{
    int r, g, b, gray;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            r = adjust[i][j][0];
            g = adjust[i][j][1];
            b = adjust[i][j][2];
            gray = (r*4 + g*10 + b*2) >> 4;
            filter[i][j][0] = filter[i][j][1] = filter[i][j][2] = gray;
            filter[i][j][3] = adjust[i][j][3];
        }
    }
}

void edgeFunc(std::shared_ptr<QImage> adjust, std::shared_ptr<QImage> filter)
{
    int height = adjust->height(), width = adjust->width();
    const QRgb *p_adjusted_u, *p_adjusted_c = (const QRgb *)adjust->constScanLine(0), *p_adjusted_d;
    QRgb mat[3][3],
            &ul = mat[0][0], &uc = mat[0][1], &ur = mat[0][2],
            &cl = mat[1][0], &cc = mat[1][1], &cr = mat[1][2],
            &dl = mat[2][0], &dc = mat[2][1], &dr = mat[2][2];

//    float kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    float kernel[3][3] = {{0.1f, 0.1f, 0.1f}, {0.1f, -0.9f, 0.1f}, {0.1f, 0.1f, 0.1f}};

    for (int i = 0; i < height; i++)
    {
        p_adjusted_u = p_adjusted_c;
        p_adjusted_c = (const QRgb *)adjust->constScanLine(i);
        p_adjusted_d = (i == height-1) ? p_adjusted_c : (const QRgb *)adjust->constScanLine(i+1);
        QRgb *p_filtered = (QRgb *)filter->scanLine(i);
        for (int j = 0; j < width; j++)
        {
            ul = j == 0 ? p_adjusted_u[0] : p_adjusted_u[j-1];
            cl = j == 0 ? p_adjusted_c[0] : p_adjusted_c[j-1];
            dl = j == 0 ? p_adjusted_d[0] : p_adjusted_d[j-1];

            ur = j == width-1 ? p_adjusted_u[j] : p_adjusted_u[j+1];
            cr = j == width-1 ? p_adjusted_c[j] : p_adjusted_c[j+1];
            dr = j == width-1 ? p_adjusted_d[j] : p_adjusted_d[j+1];

            uc = p_adjusted_u[j];
            cc = p_adjusted_c[j];
            dc = p_adjusted_d[j];

            int r, g, b;
            conv3x3(mat, kernel, &r, &g, &b);
            p_filtered[j] = qRgb(r, g, b);
        }
    }
}

void conv3x3(QRgb mat[3][3], float kernel[3][3], int *r, int *g, int *b)
{
    int rr = 0, gg = 0, bb = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            rr += int(kernel[i][j] * qRed(mat[i][j]));
            gg += int(kernel[i][j] * qGreen(mat[i][j]));
            bb += int(kernel[i][j] * qBlue(mat[i][j]));
        }
    *r = clamp(rr, 0, 255);
    *g = clamp(gg, 0, 255);
    *b = clamp(bb, 0, 255);
}

void qConv(QImage *src, QImage *dst, float **kernel)
{
    // get sizes
    int s_height = src->height(), s_width = src->width(), k_height, k_width;
    getArraySize(kernel, k_height, k_width);

//    src->bits()
}
