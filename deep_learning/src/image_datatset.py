# image_dataset.py

import torch
import torchvision

import numpy as np

from os import listdir
from os.path import isfile, join

from torch.utils.data import Dataset
from skimage import io

class ImageDataset(Dataset):
    def __init__(self, images_dir):
        self.images_dir = images_dir
        self.image_files = [join(self.images_dir, f) for f in listdir(self.images_dir) if isfile(join(self.images_dir, f))]
        self.image_transform = torchvision.transforms.ToTensor()

    def __len__(self):
        return len(self.image_files)

    def __getitem__(self, idx):
        image = io.imread(self.image_files[idx])
        return self.image_transform(image), 1
