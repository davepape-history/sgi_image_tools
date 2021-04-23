# sgi_image_tools
by Dave Pape (dave.pape@acm.org)

This is a collection of small programs for doing various things with images in the SGI image format (https://en.wikipedia.org/wiki/Silicon_Graphics_Image).  I created them as a supplement to the standard SGI programs by Paul Haeberli et al.  They were used for many projects at the NASA/GSFC Scientific Visualization Studio in the early 1990s.
To recompile some of the code you will need my libdave library.



addover underimage overimage outimage [xpos ypos]
    A cross between /usr/sbin/add and /usr/sbin/over, this allows you to add two images which are of different sizes. overimage is added to underimage with its lower left corner at pixel location xpos,ypos. outimage will be the same size as underimage. 

colorover inimage alphaimage outimage red green blue
    Similar to matte (see below), but simplified to overlay a fixed color instead of a second image. red,green,blue specifies the color to overlay; each channel should have values between 0 and 255. The blending is done using the greyscale image alphaimage to define the overlay color's opacity (0 alpha yields the color of inimage, 255 alpha yields the overlay color). 

colorunder inimage alphaimage outimage red green blue
    Same as colorover, except that the color is added under the image (or, the image is added over the constant color). alphaimage in this case defines the opacity of inimage - 0 yields the underlay color, 255 yields the color of inimage. 

dissolve inpattern1 inpattern2 outpattern startframe endframe [step]
    Creates a dissolve between two sequences of images. inpattern1 and inpattern2 are printf format strings which, with the frame number, will generate the file names for the frames of the two input sequences (via a sprintf(filename,pattern,framenum). For example, "Frame.%04d.sgi" will cause dissolve to read the file "Frame.0001.sgi" for frame number 1. outpattern is a similar format string for the output file names. The frame numbers go from startframe to endframe, optionally stepping by step. The dissolve blends linearly between the two sequences; at frame startframe, the output image will be identical to the input image from sequence 1; at frame endframe, the output will be identical to the input from sequence 2. A constant image can be used for either input sequence by just giving the exact file name of the image. 

fade inimage1 inimage2 outimage-basename frames
    Similar to dissolve, but creates a dissolve between two fixed images rather than two sequences (it's faster than dissolve for this special case). inimage1 and inimage2 are the two input images. The output images will all have the name outimage-basename followed by the frame number (zero padded to 4 digits). frames is the number of frames the dissolve should take. For example, fade foo.sgi bar.sgi baz 10 will generate the files baz0000, baz0001, ... baz0009. 

fill-holes inimage.bw outimage.bw [num-passes]
    A little hack to fill in "holes" in a greyscale image. Every pixel whose intensity value is 0 is replaced by the average of its non-zero neighbors. If num-passes is greater than one, the process is repeated this many times, allowing larger gaps to be filled in. This program is handy for removing fine details such as grid lines from a map. Note: only supports 1 band images; all remaining bands in the image will be ignored. 

fromfloat indata outimage.sgi xdim ydim [min max]
    Similar to /usr/sbin/frombin; converts a file of raw floating point data into an SGI image. indata should be a file of unformatted data - basically an array of xdim*ydim floats. The data will all be scaled to the range 0 to 255 and written out as an xdim by ydim greyscale image (outimage.sgi). If min and max are not given, the data will be scaled so that the smallest value in indata will map to pixel value 0, and the largest will map to 255. If min and max are given, any input values less than or equal to min will map to 0, and any input values greater than or equal to max will map to 255. Values between the minimum and maximum map linearly to the range 0 to 255. 

fromhsv inimage.sgi outimage.sgi
    Reverses the conversion of tohsv. inimage.sgi is a three band SGI image; band 0 is interpreted as hue, band 1 as saturation, and band 2 as value. The pixel colors of the image are converted back to RGB space and written out as outimage.sgi. 

fromlong inimage.32 outimage.sgi xdim ydim [min max]
    Similar to fromfloat, except that the input data (inimage.32) are signed long integers. 

fromshort inimage.16 outimage.sgi xdim ydim [min max]
    Similar to fromfloat, except that the input data (inimage.16) are signed short integers. 

get-matte inimage.sgi outmatte.sgi bg-r bg-g bg-b [dist]
    Creates an alpha channel for an image which leaves out the given background color. inimage.sgi is the input image; outmatte.sgi is the one band output matte (alpha) image; bg-r,bg-g,bg-b is the RGB color (values from 0 to 255) of the background. For any pixel of inimage.sgi which is the background color, the value of the corresponding pixel of outmatte.sgi is 0; otherwise, the output value is 255. If dist is given, any pixel whose color's Euclidean distance (in RGB space) from bg-r,bg-g,bg-b is less than or equal to dist is considered part of the background. 

imgcmp image1.sgi image2.sgi
    A /bin/cmp for images. Only the dimensions and pixel values of the two images are compared (cmp might say that two image files are different only due to meaningless header differences, or because one is run-length-encoded and the other is not). A message is printed indicating whether the two images are identical or if they are different (and where they first differ). 

imggrad inimage outimage
    Creates a gradient image of the (greyscale) input image. For each input pixel, the corresponding output pixel has an value proportional to the magnitude of the color gradient at that point. The output values are scaled to cover the range 0 to 255. This can be used as a very simple edge-detector. 

imgrange in-image out-image new-min new-max
    The dual of /usr/sbin/imgexp. This will rescale an image's pixel values so that they will cover the range new-min to new-max. 

matte underimage overimage matteimage outimage [xpos ypos]
    Composites one image on top of another. overimage is blended over underimage using matteimage as the alpha channel. The output color of a pixel is a linear combination of the colors of the corresponding pixels in the two input images; the value of the pixel in the alpha image is the blending factor. When the alpha is 0, the result is the color from underimage; when the alpha is 255, the result is the color from overimage. If xpos and ypos are given, overimage is composited with its lower left corner at pixel coordinates (xpos,ypos) of underimage. 

minmax [-<type>] file
    Finds the minimum and maximum values in a file of raw data (not an SGI image). type is the type of the data in file; it can be float, long, ulong (unsigned long), short, ushort, char, or uchar. This can be useful in conjunction with fromfloat, fromlong, and fromshort to choose the min & max values they use. 

remapimg inimage outimage map.txt
    Remaps the pixel values of a single band image. map.txt is a text file containing 256 numbers. The first number is the new value for any pixels whose input value is 0; the second is the new value for pixels with input value 1; etc. 

replay [-s] [image ...]
    Displays a sequence of images in a GL window, similar to /usr/sbin/movie. The image file names can either be given on the command line, or on standard input. The -s flag runs replay in "slideshow" mode, where each image is displayed until the left mouse button or the return key is pressed; otherwise, the images are flipped through as quickly as they can be read. The escape key will exit from the program at any time. This is useful for examining a series of frames once, without loading up memory with all the images. 

smear inimage smearimage outimage [scale]
    "Smears" the colors in an image based on the gradient of another (greyscale) image. For each pixel of outimage, the gradient vector of the corresponding pixel in smearimage is computed; this vector is added to the pixel's position, and the color at the resulting position in inimage is used for the output color. The gradient vectors are multiplied by scale if it is given. 

tohsv inimage.sgi outimage.sgi
    Converts an image from RGB color space to HSV. For each pixel in the input image, its RGB color values are converted to HSV values and written to the output image; band 0 of outimage.sgi is the hue, band 1 is the saturation, and band 2 is the value. This can be useful if you wish to apply other image tools to the HSV values of an image; the final result can then be converted back to RGB using fromhsv. 

wipe inpattern1 inpattern2 wipepattern outpattern startframe endframe [step]
    A time-sequence versions of matte, for creating animated wipes or other transitions between two sequences of images. inpattern1 and inpattern2 are printf format strings for the two input sequences, as in dissolve; wipepattern is a format string for the alpha channel; outpattern is a format string for the output images. The frame numbers run from startframe to endframe, optionally stepping by step. For each frame, the image from the second sequence is composited over the image from the first sequence, using the "wipe" image as the alpha channel, as in matte. 

