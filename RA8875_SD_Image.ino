#include <SPI.h>
#include <SD.h>
#include "defines.h"
#include "RA8875_SPI2.h"

// Connect SD SCLK to Digital #13 (Hardware SPI clock)
// Connect SD MISO to Digital #12 (Hardware SPI MISO)
// Connect SD MOSI to Digital #11 (Hardware SPI MOSI)
// Connect RA8875 MOSI to PB15
// Connect RA8875 MISO to PB14
// Connect RA8875 SCLK to PB13

#define RA8875_INT PB9
#define RA8875_CS 10
#define RA8875_RESET 9

// set up variables using the SD utility library functions:
#define chipSelect PB8

RA8875_SPI2 tft = RA8875_SPI2(RA8875_CS, RA8875_RESET);

void setup () {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1);
  }
  Serial.println("initialization done.");

  Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  Serial.print("Found RA8875 ");
  Serial.print("(");
  Serial.print(tft.width());
  Serial.print(", ");
  Serial.print(tft.height());
  Serial.println(")");

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV8); // PWM output for backlight
  tft.PWM1out(100);
  tft.fillScreen(RA8875_BLACK);
  tft.textMode();
  tft.textColor(RA8875_RED, RA8875_BLACK);
  for(int x = 1; x < 10; x++) {
    tft.textEnlarge(x / 3);
    tft.textSetCursor(x * 30, x * 30);
    tft.textWrite("Hello World");
  }

  tft.graphicsMode();     // go back to graphics mode
  tft.fillRect(680, 450, 200, 100, RA8875_BLUE);
  tft.drawRect(680, 450, 200, 100, RA8875_GREEN);
  tft.fillRoundRect(450, 50, 200, 100, 10, RA8875_RED);

  delay(2000);

  tft.fillScreen(RA8875_BLACK);
  tft.graphicsMode();
}

void loop()
{
  // DisplayBitmap("pl1.bmp", 0, 0);
  // delay(2000);
  // tft.fillScreen(RA8875_BLACK);
  // DisplayBitmap("pl2.bmp", 0, 0);
  // delay(2000);
  // tft.fillScreen(RA8875_BLACK);
  // DisplayBitmap("pl3.bmp", 0, 0);
  // delay(2000);
  // tft.fillScreen(RA8875_BLACK);
  // DisplayBitmap("pl4.bmp", 0, 0);
  // delay(2000);
  // tft.fillScreen(RA8875_BLACK);
  // writeText("24bbp Bitmap image", "Angel.bmp", 100, 40);
  // DisplayBitmap("Angel.bmp", 10, 10);
  // delay(2000);
  // tft.fillScreen(RA8875_BLACK);
  writeText("24bbp Bitmap image", "venom.bmp", 100, 40);
  DisplayBitmap("venom.bmp", 100, 100);
  delay(2000);
  writeText("24bbp Bitmap image", "triton.bmp", 100, 40);
  DisplayBitmap("triton.bmp", 100, 100);
  delay(2000);
  writeText("8bbp Bitmap image", "yabby.bmp", 100, 40);
  DisplayBitmap("yabby.bmp", 100, 100);
  delay(5000);
  writeText("24bbp Bitmap images", "parrot.bmp",100, 40);
  DisplayBitmap("parrot.bmp", 10, 100);
  DisplayBitmap("parrot.bmp", 150, 100);
  DisplayBitmap("parrot.bmp", 300, 100);
  delay(5000);
}

void writeText(const char * text, const char * title,  int x, int y)
{
  char buf[64];

  tft.fillScreen(RA8875_BLACK);
  tft.textMode();
  tft.textEnlarge(1);     // Set text back to normal size
  tft.textColor(RA8875_RED, RA8875_BLACK);
  tft.textSetCursor(x, y);
  sprintf(buf, "%s, %s", text, title);
  tft.textWrite(buf);
  tft.graphicsMode();   // put back to graphics mode
}

void DisplayBitmap(const char *filename, int xCoord, int yCoord)
{
  File filePtr;
  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
  COLORMAP        *clrTable;    // Used for 8 bit dibs only at this stage

  unsigned char *bitmapImage;   // image data of 1, 3 or 4 bytes per image pixel
  uint16_t      *lineBuffer;    // 565 Color data by line
  int imageIdx;                 // image index counter
  int bufSize;
  int currentLine;
  int numLines; 
  uint8_t b, g, r, w;

  Serial.printf("Loading image: %s\r\n", filename);

  if ((filePtr = SD.open(filename)) == false) {
    Serial.printf("File %s not found\r\n", filename);
    return;
  }

  //read the bitmap file header
  filePtr.read(&fileHeader, sizeof(BITMAPFILEHEADER));

  //verify that this is a .BMP file by checking bitmap id
  if (fileHeader.bfType != 0x4D42)  {
    Serial.printf("File %s is not a valid bmp\r\n", filename);
    filePtr.close();
    return;
  }

  //read the bitmap info header
  filePtr.read(&infoHeader, sizeof(BITMAPINFOHEADER)); 
  
  fileHeader.bfSize         = bswap32(fileHeader.bfSize);     // Swap endianness for each element
  fileHeader.bfOffBits      = bswap32(fileHeader.bfOffBits);

  infoHeader.biSize         = bswap32(infoHeader.biSize);
  infoHeader.biBitCount     = bswap16(infoHeader.biBitCount);
  infoHeader.biPlanes       = bswap16(infoHeader.biPlanes);
  infoHeader.biCompression  = bswap32(infoHeader.biCompression);
  infoHeader.biSizeImage    = bswap32(infoHeader.biSizeImage);
  infoHeader.biWidth        = bswap32(infoHeader.biWidth);
  infoHeader.biHeight       = bswap32(infoHeader.biHeight);
  infoHeader.biClrUsed      = bswap32(infoHeader.biClrUsed);
  infoHeader.biClrImportant = bswap32(infoHeader.biClrImportant);

  Serial.printf("biBitCount: 0x%X (%d)\r\n", infoHeader.biBitCount, infoHeader.biBitCount);
  Serial.printf("Compression: 0x%X\r\n", infoHeader.biCompression);
  Serial.printf("biClrUsed: 0x%X (%d)\r\n", infoHeader.biClrUsed, infoHeader.biClrUsed);

  if(infoHeader.biBitCount == 8 && infoHeader.biClrUsed > 0) {                   // Get the colur pallette for 8 bit dib's
    clrTable = (COLORMAP *)malloc((sizeof(COLORMAP) * infoHeader.biClrUsed));    // create an array of 4 byte colors
    filePtr.read(clrTable, sizeof(COLORMAP) * infoHeader.biClrUsed); 
  }

  //move file pointer to the beginning of bitmap data
  filePtr.seek(fileHeader.bfOffBits);

  bufSize = infoHeader.biSizeImage / infoHeader.biHeight;  // enough Ram for one line (could be 1, 3 or 4 bytes per pixel)

  bitmapImage = (unsigned char*)malloc(bufSize);
  lineBuffer  = (uint16_t *)malloc(infoHeader.biWidth * 2);

  currentLine = infoHeader.biHeight;      //DIB's are bottom up coordinate system

  for(numLines = 0; numLines < infoHeader.biHeight; numLines++) {
    filePtr.read(bitmapImage, bufSize);       // read in bufSize chunks of bitmap image data

    imageIdx = 0;

    for(int x = 0; x < infoHeader.biWidth; x++) {
      switch(infoHeader.biBitCount) {
      case 32:  w = bitmapImage[imageIdx++];  // Alpha bits are ignored at this time
                b = bitmapImage[imageIdx++];
                g = bitmapImage[imageIdx++];
                r = bitmapImage[imageIdx++];
                break;
      case 24:  b = bitmapImage[imageIdx++];
                g = bitmapImage[imageIdx++];
                r = bitmapImage[imageIdx++];
                break;
      case 8:   b = clrTable[bitmapImage[imageIdx]].b;
                g = clrTable[bitmapImage[imageIdx]].g;
                r = clrTable[bitmapImage[imageIdx++]].r;
                break;
      }
      lineBuffer[x] = color565(r, g, b);
    }

    tft.drawPixels(lineBuffer, infoHeader.biWidth, xCoord, currentLine + yCoord);
    currentLine--;
  }

  // Free up dynamically allocated ram
  Serial.printf("In - Ram available %d\r\n", freeRam());
  free(clrTable);
  free(bitmapImage);
  free(lineBuffer);
  filePtr.close();
  Serial.printf("Out - Ram available %d\r\n", freeRam());
}

uint16_t bswap16(uint16_t a)
{
  uint8_t tmp_a = (a & 0xff00) >> 8 ;
  uint8_t tmp_b = (a & 0x00ff);
  uint16_t b = (tmp_b | (tmp_a << 8));
  return b;
}

uint32_t bswap32(uint32_t a)
{
  uint8_t tmp_a = (a & 0xff000000) >> 24;
  uint8_t tmp_b = (a & 0x00ff0000) >> 16;
  uint8_t tmp_c = (a & 0x0000ff00) >> 8 ;
  uint8_t tmp_d = (a & 0x000000ff);
  uint32_t b = (tmp_d | (tmp_c << 8) | (tmp_b << 16) | (tmp_a << 24));
  return b;
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

extern "C" char* sbrk(int incr);

int freeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}