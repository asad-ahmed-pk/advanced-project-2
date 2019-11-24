# copy_random_sample.py
# Copy a random number of files from the given folder to destination folder

import os
import sys
import random
import shutil

# args
if len(sys.argv) < 4:
    print("Usage: python copy_random_sample.py [count] [src_dir] [output_dir]")
    sys.exit(0)

# set all args
count = int(sys.argv[1])
src_dir = sys.argv[2]
dst_dir = sys.argv[3]

# get list of filenames in src dir
files = [f for f in os.listdir(src_dir) if os.path.isfile(os.path.join(src_dir, f))]
if count > len(files):
    print("count provided exceeds number of files in dir")
    sys.exit(1)

# create dst dir if needed
if not os.path.exists(dst_dir):
    os.makedirs(dst_dir)

# copy random sample of files
random_files = random.sample(files, count)
for f in random_files:
    src = os.path.join(src_dir, f)
    dst = os.path.join(dst_dir, f)
    shutil.copyfile(src, dst)
    print("Copied", f, "to", dst)
