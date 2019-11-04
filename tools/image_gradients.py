# image_gradients.py
# Calculate and save image gradients

import os
import sys
import cv2
import numpy as np


def sobel(image):
    """ Sobel filter based gradient magnitude """
    Gx = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=3)
    Gy = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=3)
    G = np.sqrt(np.square(Gx) + np.square(Gy))
    return G


def DoG(image, k):
    """ Difference of gaussians based gradient magnitude """
    g1 = cv2.GaussianBlur(image, (1, 1), 0)
    g2 = cv2.GaussianBlur(image, (k, k), 0)
    return (g2 - g1)


def nms(image):
    """ Suppress non-max values """
    val, t = cv2.threshold(image, 80, 255, cv2.THRESH_TOZERO)
    return t



# args
if len(sys.argv) < 3:
    print("Usage: python image_gradients.py [input dir of images] [output_dir]")
    sys.exit(0)

# input dir of all images
images_dir = sys.argv[1]
output_dir = sys.argv[2]

# read in all images
image_files = [os.path.join(images_dir, f) for f in os.listdir(images_dir) if os.path.isfile(os.path.join(images_dir, f))]

# create output dir if not exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

for image_file in image_files:
    image = cv2.imread(image_file, cv2.IMREAD_GRAYSCALE)
    grad_image = sobel(image)
    #grad_image = nms(grad_image)

    copy = np.zeros(grad_image.shape)
    copy[grad_image == 0] = 255

    if (os.path.basename(image_file) == "CamThermal_001837380000.exr"):
        cv2.imshow("Gradient", copy)
        cv2.waitKey(0)

    filename =  os.path.splitext(os.path.basename(image_file))[0] + ".png"
    filepath = os.path.join(output_dir, filename)
    print("Saving gradient image:", filepath)

    cv2.imwrite(filepath, grad_image)
