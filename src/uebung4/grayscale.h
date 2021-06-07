#ifndef _GRAYSCALE_H
#define _SPARSE_MATRIX_H

using namespace std;

class GreyScale{
    private:
        int height;
        int width;

        //sparse_matrix pixels; ?
    public:
        GreyScale();
        GreyScale(int height, int width);
        void resize(int height, int width);
        int getHeight(){
            return height;
        }
        int getWidth(){
            return width;
        }
        

        void put(int i, int j, int x);   // Matrixelement schreiben
        int  get(int i, int j) const;
        int& operator () (int,int);         // Standard Zugriff auf GrayScaleelement
        int  operator () (int,int) const;   // Zugriff auf const GrayScale

        GreyScale& operator =  (const GreyScale&);   // Zuweisung
        GreyScale& operator += (const GreyScale&);   // Zuweisungen mit arithm. Operation
        GreyScale& operator -= (const GreyScale&);

        GreyScale &binarize(float c);
        GreyScale &clamp();
        GreyScale &contrast();
        GreyScale &linTrans(float a, float b);
        GreyScale &invert();














        GreyScale &convolve(const float mask[], int size=3);
        GreyScale &blur();
        GreyScale &kirsch();
        GreyScale &laplace();
        GreyScale &median();
        GreyScale &sobel();
        void error(const char str[]);
};


#endif