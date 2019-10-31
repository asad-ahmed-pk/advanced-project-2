# image_gradients.py
# Calculate and save image gradients

import os
import sys
import cv2

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
    Gx = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=3)
    Gy = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=3)
    grad_image = Gx * Gy

    filename =  os.path.splitext(os.path.basename(image_file))[0] + ".png"
    filepath = os.path.join(output_dir, filename)
    print("Saving gradient image:", filepath)

    cv2.imwrite(filepath, grad_image)
