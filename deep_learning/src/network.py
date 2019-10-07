# network.py
# Neural network models

import torch.nn as nn
import torch.nn.functional as F

IMAGE_WIDTH = 382
IMAGE_HEIGHT = 288

class ThermalImageCNN(nn.Module):
    """ CNN that recognizes fumaroles in thermal images """
    def __init__(self):
        super(ThermalImageCNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.avg = nn.AdaptiveAvgPool2d(360)
        self.fc1 = nn.Linear(16 * 360 * 360, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 2)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = self.avg(x)
        x = x.view(-1, 16 * 360 * 360)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x
