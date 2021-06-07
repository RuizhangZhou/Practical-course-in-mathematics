#ifndef _GRAYSCALE_H
#define _SPARSE_MATRIX_H

using namespace std;

class GreyScale{
    private:
        int height;
        int width;
    public:
        GreyScale();
        GreyScale(int height, int width);
        void resize(int height, int width);
        int getWidth(){
            return width;
        }
        int getHeight(){
            return height;
        }
        GreyScale &convolve(const float mask[], int size=3);
        GreyScale &blur();
        GreyScale &kirsch();
        GreyScale &laplace();
        GreyScale &median();
        GreyScale &sobel();
        void error(const char str[]);
};


#endif