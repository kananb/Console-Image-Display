# Console Image Display
![](/images/main_image.png)
## Table of Contents
* [General Info](#general-info)
* [Features](#features)
* [Usage](#usage)
* [Examples](#examples)
* [Launch](#launch)

## General Info
An command line application that allows images to be displayed within the windows command prompt. This project was created to demonstrate the progression of my coding practices and ability.

## Features
### Image and font scaling
![](/images/scaling.png)
### Pixel and ascii viewing
![](/images/image_type.png)
### Multiple coloring algorithms
![](/images/coloring_algorithms.png)
### User-selected color sets
![](/images/custom_colors.png)

## Usage
See ![Launch](#launch) for setup instructions.

     *.exe "IMAGE" [OPTION]...


##### Options:
```
     -fs n              (Font size) [default: 1]
                        Displays the image in font size 'n'
```
```
     -sf n              (Scale factor) [default: 1.0]
                        Scale the image by 'n'
```
```
     -it [p|a|r]        (Image type) [default: p]
                        Changes the characters used to display the image

                        [p: pixel (ascii 219)]
                        [a: ascii]
                        [r: ascii (reduced characters)]
```
```
     -am [pre|post]     (Ascii mode) [default: pre]
                        Changes whether ascii characters are assigned before or
                        after image processing

                        [pre: pre-process]
                        [post: post-process]

                        NOTE: this option is ignored if the image type is pixel
```
```
     -cm [a|f|n]        (Color mode) [default: a]
                        Changes the algorithm used to process the image

                        [a: atkinson dithering]
                        [f: floyd-steinberg dithering]
                        [n: nearest value]
```
```
     -ic ["FILE"|$*]    (Image colors) [default: $default]
                        Choose the set of colors the image processing is
                        applied with
						
						["FILE": path to file containing colors]
                        [$*: predefined color sets]
                                -$grey: 16 shades of grey
                                -$default: default command prompt colors
                                -$bw: black and white
                                -$approx(n): n approximate colors
								
                        NOTE: colors specified in a file should be delimited
                        by new lines with r,g,b elements delimited by commas

                        
```
```
     -tc ["FILE"|$*]    (Text colors) [default: $default]
                        Choose the set of colors the text is displayed with

                        ["FILE": path to file containing colors]
                        [$*: predefined color sets]
                                -$grey: 16 shades of grey
                                -$default: default command prompt colors
                                -$bw: black and white
                                -$approx(n): n approximate colors
								
						NOTE: colors specified in a file should be delimited
                        by new lines with r,g,b elements delimited by commas
```
Approximate colors serve the purpose of reducing the total number of unique colors used in the image while preserving as much detail as possible. Use of approximate colors is necessary to reproduce images in the command prompt since only 16 colors are available for use when coloring the image.
###### Help information can be viewed by running the application with no parameters

## Examples

## Launch

* Visual C++ 2017
* [OpenCV 3.4.3](https://opencv.org/releases/page/3/)
* WinAPI

To run this program, download the executable from the bin folder or download the source files from the src folder and compile them with the Visual C++ compiler (found in the Windows SDK). If compiling locally, it is necessary to link opencv_world232.lib and include all of the required header files from the OpenCV download.
