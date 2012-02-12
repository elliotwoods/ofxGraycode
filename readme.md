# Introduction
For use with encoding / decoding structured light (also for use with external sources).

You might want to check out this first:
https://github.com/YCAMInterlab/ProCamToolkit.git

# Why?
Q. I mean, ffs why? Kyle McDonald has already made loads of awesome tools in this space?

A. I know I know, but my requirements are pretty specific

# Examples

## EncodeGraycode

* Press any key to progress through frames (output from encoder)
* Graycode frames will be presented on the screen
* Payload resolution is 512x512

## DecodeGraycode

* Again, press any key to progress through frames (output from encoder and input into decoder)
* When you get to the end of the frames, the decoder will calculate the dataset, and visualise on the right
* Payload resolution is 512x512

## DecodeGraycodeOSC

* Send frames from an external source with OSC messages `/capture` and `/clear` to port `5588`
* Live camera is shown on left
* After all frames are received, preview of data will be shown on right
* Use [left]/[right] keys to look through data (preview, mean, capture1, capture2, ... , capturen)
* Payload resolution is 512x512