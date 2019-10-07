# train.py

import torch
import torch.optim as optim
import torch.nn as nn

from torch.utils.data import DataLoader
from image_datatset import ImageDataset
from network import ThermalImageCNN

# CUDA support check
use_cuda = torch.cuda.is_available()
if use_cuda:
    print("CUDA supported GPU detected")

thermal_images_dir_train = "../data/CamThermal/train/images"

# training parameters
params = {"batch_size": 1, "shuffle": True, "num_workers": 2}
max_epochs = 5

# load datasets
ds_train = ImageDataset(thermal_images_dir_train)
train_loader = DataLoader(ds_train, **params)

net = ThermalImageCNN()

# note: turned off because GPU CUDA is not supported
use_cuda = False
if use_cuda:
    net.cuda()

# optimiser
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

# model_save_path
save_path = "model_v2"

# train loop
for epoch in range(max_epochs):

    running_loss = 0.0
    for i, data in enumerate(train_loader):

        image_tensor, label = data

        optimizer.zero_grad()
        outputs = net(image_tensor)
        print(outputs)
        loss = criterion(outputs, label)
        loss.backward()
        optimizer.step()

        running_loss += loss.item()
        print(f"Running loss ({epoch + 1}):", running_loss)
    
print("Training finished")

torch.save(net, save_path)
print("Model saved")
