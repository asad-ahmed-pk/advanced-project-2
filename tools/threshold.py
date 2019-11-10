# threshold.py
# Removes cold data from the thermal image

test_image = "../../Data/0008/CamThermal/CamThermal_000503014000.exr"

import cv2
import os
import sys

import numpy as np


def cluster(img, K):
    """ K-means image segmentation """

    Z = img.reshape((-1, 1))

    # convert to np.float32
    Z = np.float32(Z)

    # define criteria, number of clusters(K) and apply kmeans()
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
    ret, label, center = cv2.kmeans(Z, K, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)

    # Now convert back into uint8, and make original image
    center = np.uint8(center)
    res = center[label.flatten()]
    res2 = res.reshape((img.shape))

    return res2


# args
if len(sys.argv) < 3:
    print("Usage: python threshold.py [input dir of images] [output_dir]")
    sys.exit(0)

# input dir of all images
images_dir = sys.argv[1]
output_dir = sys.argv[2]

# read in all images
image_files = [os.path.join(images_dir, f) for f in os.listdir(images_dir) if os.path.isfile(os.path.join(images_dir, f))]

# create output dir if not exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# threshold and save all
for image_file in image_files:

    # threshold
    image = cv2.imread(image_file, cv2.IMREAD_GRAYSCALE)
    ret, t = cv2.threshold(image, 80, 255, cv2.THRESH_TOZERO)

    # cluster
    clustered_image = cluster(t, 2)

    filename = os.path.splitext(os.path.basename(image_file))[0] + ".png"
    filepath = os.path.join(output_dir, filename)
    print("Processing", image_file)

    cv2.imwrite(filepath, t)
