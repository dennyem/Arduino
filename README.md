# Arduino Adafruit_RA8875 5 inch TFT display on STM32F401RE Nucleo

I purchased the 5" TFT from Core Electronics (Australia) a few months ago and have been working
on getting it working.  It works on a simple UNO example fine but on a
more powerful MCU like the STM32F401RE Nucleo forget it.

After some weeks of effort I have the solution.  The Adafruit_RA8875
library is lacking in some areas and does not handle the faster
processors at all well.  Most graphics functions will work but not
bitmap drawing.  Also getting an external SD Card to work (on the same
SPI port) is finicky at best.

I have attached my solution which consists of a replacement RA8875
library (RA8875_SPI2) which allows an external SD card to be attached to
the Nucleo's SPI1 port and dedicate SPI2 to the TFT. I have also modified
the drawPixel() function to work with the faster processor.
The SPI2 pins can be configured within the library (setMOSI() etc.) so it
should work with other processors, this has not been tested however.

The example sketch included also drives the image drawing capabilities
to the fullest by sending an entire scan line of pixel data at once. It
would be interesting to see if anyone can speed it up further (reader
challenge...).  In order to use the library it needs to be moved to the
Arduino's "library" path.  The images (both 24 bit and 8 bit examples)
in the "Data" folder should be copied to an SD card.
