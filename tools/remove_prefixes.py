# remove_prefixes.py
# Remove prefixes from images (only leave image number and extension)

import os
import sys
import random
import shutil

# args
if len(sys.argv) < 2:
    print("Usage: python remove_prefixes.py [directory]")
    sys.exit(0)

# set all args
src_dir = sys.argv[1]

# get list of filenames in src dir
files = [f for f in os.listdir(src_dir) if os.path.isfile(os.path.join(src_dir, f))]

# rename all files
for f in files:
    # split name - char is _
    name = os.path.splitext(f)[0]
    name_number_only = name.split('_')[2]

    # extension
    ext = os.path.splitext(f)[1]

    filename = name_number_only + ext
    src = os.path.join(src_dir, f)
    dst = os.path.join(src_dir, filename)

    print("Renamed from", src, "to", dst)
    shutil.move(src, dst)
