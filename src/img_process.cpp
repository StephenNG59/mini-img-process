#include "img_process.h"
#include "basic.h"

/* grayFunc: change the origin image to grayscale */
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

void edgeFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width)
{
    float **kernel = new float*[7];
    float k[7][7] = {
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, +5.8f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f},
        {-0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f}};
    for (int i = 0; i < 7; i++)
    {
        kernel[i] = new float[7];
        for (int j = 0; j < 7; j++)
            kernel[i][j] = k[i][j];
    }

    convolve2D(adjust, filter, kernel, height, width, 7, 7);
}

void smoothFunc(unsigned char ***adjust, unsigned char ***filter, int height, int width)
{
    float **kernel = new float*[7];
    float k[7][7] = {
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.04f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f},
        {0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f, 0.02f}};
    for (int i = 0; i < 7; i++)
    {
        kernel[i] = new float[7];
        for (int j = 0; j < 7; j++)
            kernel[i][j] = k[i][j];
    }

    convolve2D(adjust, filter, kernel, height, width, 7, 7);
}

void convolve2D(unsigned char ***src, unsigned char ***dst, float **kernel, int src_h, int src_w, int ker_h, int ker_w)
{
    assert(ker_h % 2 == 1 && ker_w % 2 == 1);

    float t1 = clock();
    // padding (use replicate)
    int pad_h = src_h + ker_h - 1, pad_w = src_w + ker_w - 1;
    unsigned char ***pad = new unsigned char **[pad_h];
    for (int i = 0; i < pad_h; i++)
    {
        pad[i] = new unsigned char *[pad_w];
        for (int j = 0; j < pad_w; j++)
        {
            pad[i][j] = new unsigned char[4];
            for (int k = 0; k < 4; k++)
            {
//                pad[i][j][k] = src[std::max(i-ker_h/2, 0)][std::max(j-ker_w/2, 0)][k];
                pad[i][j][k] = src[clamp(i - ker_h/2, 0, src_h-1)][clamp(j - ker_w/2, 0, src_w-1)][k];
            }
        }
    }

    std::cout << "here#121: time used: " << (clock() - t1) / 1000.f << std::endl;
    t1 = clock();

    // convolve 2D (kernel is same for all color channels)
    for (int i = 0; i < src_h; i++)
    {
        for (int j = 0; j < src_w; j++)
        {
            int r = 0, g = 0, b = 0, a = 0;
            for (int m = 0; m < ker_h; m++)
            {
                for (int n = 0; n < ker_w; n++)
                {
                    r += pad[i+m][j+n][0] * kernel[m][n];
                    g += pad[i+m][j+n][1] * kernel[m][n];
                    b += pad[i+m][j+n][2] * kernel[m][n];
//                    std::cout << i << " " << j << " " << m << " " << n << std::endl;
//                    a += pad[i+m][j+n][3] * kernel[m][n];
                }
            }
            dst[i][j][0] = (unsigned char)clamp(r, 0, 255);
            dst[i][j][1] = (unsigned char)clamp(g, 0, 255);
            dst[i][j][2] = (unsigned char)clamp(b, 0, 255);
//            dst[i][j][3] = (unsigned char)clamp(a, 0, 255);
        }
    }

    std::cout << "here#122: time used: " << (clock() - t1) / 1000.f << std::endl;

    for (int i = 0; i < pad_h; i++)
    {
        for (int j = 0; j < pad_w; j++)
        {
            delete [] pad[i][j];

        }
        delete [] pad[i];
    }
    delete [] pad;
}
