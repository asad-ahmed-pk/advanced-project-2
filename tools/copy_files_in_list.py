# copy_files_in_list.py
# Copy the given files in the file list to the dst folder

import os
import sys
import shutil

# args
if len(sys.argv) < 4:
    print("Usage: python copy_files_in_list.py [file_list_file] [src dir] [output_dir]")
    sys.exit(0)

file_list_file_path = sys.argv[1]
src_dir = sys.argv[2]
output_dir = sys.argv[3]

# get all file IDs
with open(file_list_file_path, 'r') as f:
    file_list = f.readlines()
file_list = [line.strip() for line in file_list]

# file list to copy from
files_to_copy = [f for f in os.listdir(src_dir) if os.path.isfile(os.path.join(src_dir, f))]

# create output dir if needed
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

count = 0
for f in file_list:
    # get corresponding file from src dir
    for f2 in files_to_copy:
        if f in f2:
            src = os.path.join(src_dir, f2)
            dst = os.path.join(output_dir, f2)
            shutil.copyfile(src, dst)
            print("Copied", f, "to", dst)
            count += 1

print("\nDone. Copied", count, "files.")
