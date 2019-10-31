# create_label_list.py
# Creates a txt file with the file IDs for the given dir of labelImg xml files

import sys
import os

# arg check
if (len(sys.argv) < 3):
    print("Usage: python create_label_list [input_dir] [output_dir]")

# get args for dirs
input_dir = sys.argv[1]
output_dir = sys.argv[2]

# get file list in input dir
files = [f for f in os.listdir(input_dir) if os.path.isfile(os.path.join(input_dir, f))]
ids = [s.split('.')[0] for s in files if s.split('.')[1] == 'xml']

# create dir if not exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# write id file
with open(os.path.join(output_dir, 'fumarole_test_list.txt'), 'w') as f:
    for id in ids:
        f.write(id + '\n')

print("Total number of labels:", len(ids))