# thermal_image_dataset.py
# Thermal image dataset for PyTorch

import torch

from torch.utils.data.dataset import Dataset
from skimage import io

from os import listdir
from os.path import isfile, join

import bounding_box


class ThermalImageSample:
    """ Represents a sample for this dataset """
    def __init__(self, image, fumarole):
        self.image = image
        self.fumarole = fumarole


class ThermalImageDataSet(Dataset):
    """ Thermal image dataset of Fumaroles """
    """ Samples are of the form: {image: the thermal image, 
        x: bounding box origin x, 
        y: bounding box origin y, 
        w: bounding box width
        h: bounding box height
        class: [thermal_hot OR thermal_steam]
    """

    def __init__(self, dir_path):

        self.dir_path = dir_path
        self.label_dir_path = dir_path + "/labels"
        self.image_dir_path = dir_path + "/images"

        self.label_files = [f for f in listdir(self.label_dir_path) if isfile(join(self.label_dir_path, f))]
        self.image_files = [f for f in listdir(self.image_dir_path) if isfile(join(self.image_dir_path, f))]

        print(len(self.label_files), len(self.image_files))

        # TODO: enable later when all images have been labelled
        #assert len(self.label_files) == len(self.image_files), "Count mismatch between labels and images"

        #self.total_samples = len(self.label_files)
        self.total_samples = len(self.image_files)

    def __len__(self):
        return self.total_samples

    def __getitem__(self, i):
        image_file_path = self.image_dir_path + "/" + self.image_files[i]
        image_file = io.imread(image_file_path)

        label_file_path = self.label_dir_path + "/" + self.label_files[i]
        fumarole = bounding_box.extract_bounding_box(label_file_path)

        return ThermalImageSample(image_file, fumarole)
