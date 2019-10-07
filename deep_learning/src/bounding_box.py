# bounding_box.py
# Extracts bounding box info from labeling XML files

import untangle

class Fumarole:
    """ Models a region for a fumarole in an image and its label """
    def __init__(self, x0, y0, x1, y1, class_label):
        self.x0 = x0
        self.y0 = y0
        self.x1 = x1
        self.y1 = y1
        self.class_label = class_label


def extract_bounding_box(xml_path):
    """ Extracts the bounding box info from the given XML file and returns bounding box info and class label """
    print("Parsing:", xml_path)
    tree = untangle.parse(xml_path)
    root = tree.annotation

    object = root.object
    class_label = object.name.cdata

    bounding_box = object.bndbox

    x0 = int(bounding_box.xmin.cdata)
    y0 = int(bounding_box.ymin.cdata)
    x1 = int(bounding_box.xmax.cdata)
    y1 = int(bounding_box.ymax.cdata)

    print(x0, y0, x1, y1)

    # package into object
    fumarole = Fumarole(x0, y0, x1, y1, class_label)

    return fumarole
