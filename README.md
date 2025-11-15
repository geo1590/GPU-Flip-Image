# GPU-Flip-Image
Project: GPU - Horizontally Flip an Image
Author: George Barrinuevo
Date: 11/15/2025

## Purpose
The purpose of this repository is to demonstrate how GPU programming using CUDA NPP libraries can be used on JPG images. This code will flip a directory of images and write out the resulting flipped image. This code was used as a graded lab for an online course.

## Description

The code will read in an image file. It will then use the nppiMirror_8u_C3R() library function. The 8u means 8-bit unsigned per pixel. The C3R means 3 channels for RGB color and R for ROI input (region of interest) which operates on an image rectangle. The nppiMirror means it will perform a mirror/flip operation.

 - Input Data
 The input data of the image is represented as a flat array with the size of width * height * 3-bytes. The 3-bytes are for the RGB color.
 
 - Input/Output Files
 The /data directory will contain the JPG image file used for input. The /out_data directory will contain the flipped JPG image files for the output.
 - Lessons Learned
	- The initial code was using the older NPP method which I had to convert to use the latest method to call these library functions.
- Development Environment
	- I used Google Colab to develop this code. In Colab, I used the A100 GPU device. The Colab is normally used for Python development. But, if you just use it's CLI Terminal, then you can use it like a Linux Ubuntu environment to install packages, edit files, and run CUDA NPP code. You can of course use your own development environment.
- Input Image Files
	-	I found the imput JPG input files from this URL: https://susanqq.github.io/UTKFace/. I only used about 95 of these images. For running the code, do not download the files from that link. The below step by step procedures will provide the URL link to use.

## Setup and Run the Code
This is how to run the code in your environment. Only the source codes and makefiles are stored in this github repository. The input JPG image files are stored in a downloadable URL link. Optionally, you can provide your own JPG image input files.
```bash
git clone https://github.com/geo1590/GPU-Flip-Image.git
mv GPU-Flip-Image myFlip
cd myFlip

mkdir data
mkdir out_data

cd data
wget https://od.lk/d/NDdfMTAxMDg2NTE1Xw/jpg_images.zip
unzip jpg_images.zip

cd ..
mkdir include
cd include
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

cd ..
git clone https://github.com/NVIDIA/cuda-samples.git

apt-get update
apt-get install -y libfreeimage-dev libopencv-dev python3-opencv
```
Run the code.
```bash
cd myFlip
make clean
make all
make run
head -100 output.txt
```


 
