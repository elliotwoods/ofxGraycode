# Introduction
For use with encoding / decoding structured light (also for use with external sources).

You might want to check out this first:
https://github.com/YCAMInterlab/ProCamToolkit.git

# Examples

## EncodeGraycode

* Press any key to progress through frames (output from encoder)
* Press any key to progress through frames, or [r] to reset
* Payload resolution is 1024x768

## DecodeGraycode

* Again, press any key to progress through frames (output from encoder and input into decoder)
* When you get to the end of the frames, the decoder will calculate the dataset, and visualise on the right
* Payload resolution is 1024x768

## DecodeGraycodeOSC

* Capture frames from camera, trigger captures with incoming OSC messages `/capture` and `/clear` to port `5588`
* Live camera is shown on left
* After all frames are received, preview of data will be shown on right
* Use [left]/[right] keys to look through data (preview, mean, capture1, capture2, ... , capturen)
* Payload resolution is 1024x768

## EncodeGraycodeOSC

* Send graycode encoded frames and OSC messages `/capture` and `/clear` to port `5588` of `localhost`
* Press any key to progress through frames, or [r] to reset
* Payload resolution is 1024x768