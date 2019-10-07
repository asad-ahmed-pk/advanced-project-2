# test.py

import torch

import skimage.io as io
import numpy as np

from torch.utils.data import DataLoader
from image_datatset import ImageDataset

# load trained net
net = torch.load("model_v2")
net.eval()

print(net)

# load test dataset
params = {"batch_size": 1, "shuffle": False, "num_workers": 0}
thermal_images_dir_test = "../data/CamThermal/test_unrelated/images"
test_ds = ImageDataset(thermal_images_dir_test)
data_loader = DataLoader(test_ds, **params)

for i, data in enumerate(data_loader):
    tensor_image, label = data
    output = net(tensor_image).cpu().detach().numpy()
    print(f"{i+1}: {np.argmax(output)}")
