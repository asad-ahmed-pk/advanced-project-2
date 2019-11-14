# thermal_interpolate.py
# Interpolates thermal images that have been overlayed onto camera

import sys
import os
import cv2

import numpy as np  

def process_image(image, window_size):
    n = int(window_size / 2)
    output = np.zeros(image.shape)
    for y in range(n, image.shape[0]-n):
        for x in range(n, image.shape[1]-n):

            # slice indices for 8-neighbourhood
            y0 = y - n
            y1 = y + n + 1
            x0 = x - n
            x1 = x + n + 1

            region = image[y0:y1, x0:x1]
            blurred = cv2.medianBlur(region, window_size)

            output[y, x] = blurred[int(blurred.shape[0] / 2), int(blurred.shape[1] / 2)]
    return output


# get image folder paths
if len(sys.argv) < 3:
    print("Usage: python thermal_interpolate [mapped thermal images dir] [output_dir]")
    sys.exit(0)

thermal_images_dir = sys.argv[1]
output_dir = sys.argv[2]

# get all thermal images
thermal_images = [f for f in os.listdir(thermal_images_dir) if os.path.isfile(os.path.join(thermal_images_dir, f))]

# process each
for image_file in thermal_images:
    path = os.path.join(thermal_images_dir, image_file)
    ext = os.path.splitext(image_file)[1]

    print("Processing", image_file)

    image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    output = process_image(image, 9)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    cv2.imwrite(os.path.join(output_dir, image_file), output)
    print("Processed", image_file)
    print()
