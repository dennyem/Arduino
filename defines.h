#ifndef DEFINES_H
#define DEFINES_H

// create a structure representing the bmp file contents
struct __attribute__ ((packed)) BITMAPFILEHEADER
{
  uint16_t bfType;           //specifies the file type
  uint32_t bfSize;           //specifies the size in bytes of the bitmap file
  uint16_t bfReserved01;     //reserved; must be 0
  uint16_t bfReserved02;     //reserved; must be 0
  uint32_t bfOffBits;        //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
};

// create a structure representing the dib contents
struct __attribute__ ((packed)) BITMAPINFOHEADER
{
  uint32_t biSize;           //specifies the number of bytes required by the struct
  int32_t  biWidth;          //specifies width in pixels
  int32_t  biHeight;         //specifies height in pixels
  uint16_t biPlanes;         //specifies the number of color planes, must be 1
  uint16_t biBitCount;       //specifies the number of bits per pixel
  uint32_t biCompression;    //specifies the type of compression
  uint32_t biSizeImage;      //size of image in bytes
  int32_t  biXPelsPerMeter;  //number of pixels per meter in x axis
  int32_t  biYPelsPerMeter;  //number of pixels per meter in y axis
  uint32_t biClrUsed;        //number of colors used by the bitmap
  uint32_t biClrImportant;   //number of colors that are important
};

struct __attribute__ ((packed)) COLORMAP
{
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t p;      // Padding
};

#endif