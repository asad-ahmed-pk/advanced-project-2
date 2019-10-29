# overlay_thermal.py
# Overlays the thermal over the left cam image

import sys
import os

from PIL import Image

# settings
blend_factor = 0.5

# get args for left cam image dir, and thermal image dir, and output dir
if len(sys.argv) != 4:
    print("Usage: python overlay_thermal [left/right cam dir] [thermal image dir] [output dir]")
    sys.exit(0)

left_cam_dir = sys.argv[1]
thermal_cam_dir = sys.argv[2]
output_dir = sys.argv[3]

# read in images
left_cam_image_files = [os.path.join(left_cam_dir, f) for f in os.listdir(left_cam_dir) if os.path.isfile(os.path.join(left_cam_dir, f))]
thermal_cam_image_files = [os.path.join(thermal_cam_dir, f) for f in os.listdir(thermal_cam_dir) if os.path.isfile(os.path.join(thermal_cam_dir, f))]

# save extensions
cam_image_ext = os.path.splitext(left_cam_image_files[0])[1]
thermal_image_ext = os.path.splitext(thermal_cam_image_files[0])[1]

# create image and overlay each with thermal image
for i in range(len(left_cam_image_files)):

    cam_image = Image.open(left_cam_image_files[i])

    # get corresponding thermal image
    cam_image_file_names_only = [os.path.splitext(os.path.basename(f).split('_')[1])[0] for f in left_cam_image_files]
    thermal_image_file_names_only = [os.path.splitext(os.path.basename(f).split('_')[-1])[0] for f in thermal_cam_image_files]
    
    thermal_image_index = thermal_image_file_names_only.index(cam_image_file_names_only[i])
    thermal_image = Image.open(thermal_cam_image_files[thermal_image_index])

    # blend
    blended_image = Image.blend(cam_image, thermal_image, blend_factor)

    # save to output folder
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    output_path = os.path.join(output_dir, cam_image_file_names_only[i] + cam_image_ext)

    blended_image.save(output_path)
    print("Saving to:", output_path)

print("\nDone")
