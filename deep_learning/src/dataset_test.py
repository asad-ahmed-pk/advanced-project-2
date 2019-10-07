# dataset_test.py
# Test file for testing out the dataset

import torch
import torch.optim as optim
import torch.nn as nn

from torch.utils import data

from network import ThermalImageCNN
from thermal_image_dataset import ThermalImageDataSet

# load the dataset
thermal_images_dir_train = "/data3/asahmed/advanced_project_2/project/deep_learning/data/CamThermal/train"
thermal_images_dir_test = "/data3/asahmed/advanced_project_2/project/deep_learning/data/CamThermal/test"

# CUDA support
use_cuda = torch.cuda.is_available()
if use_cuda:
    print("CUDA supported GPU detected")

device = torch.device("cuda:0" if use_cuda else "cpu")
cudnn.benchmark = True

# parameters
params = {"batch_size": 64, "shuffle": True, "num_workers": 16}
max_epochs = 100
classes = ["fumarole_steam", "fumarole_hot"]

# data generators
training_set = ThermalImageDataSet(thermal_images_dir_train)
training_generator = data.DataLoader(training_set, **params)

testing_set = ThermalImageDataSet(thermal_images_dir_test)
testing_generator = data.DataLoader(testing_set, **params)

# CNN
net = ThermalImageCNN()

# loss function and optimiser
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

# training (TODO)
"""
for epoch in range(2):

    running_loss = 0.0
    for i, data in enumerate(training_generator):
        optimizer.zero_grad()

        # get image and class labels from sample
        image, fumarole = data

        # forward + backward pass
        outputs = net(image)
        loss = criterion(outputs, )
"""
