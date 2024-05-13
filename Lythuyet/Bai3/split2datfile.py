import numpy as np
import matplotlib.pyplot as plt

import PIL
from PIL import Image

image_path = "input/Musiala.jpg"
test_image = Image.open(image_path)

arr = np.array(test_image)
red_c = arr[:,:,0]
green_c = arr[:,:,1]
blue_c = arr[:,:,2]

path = f"dat/Musiala.dat"
with open(path, 'wb') as file:
    red_c.tofile(file)
    green_c.tofile(file)
    blue_c.tofile(file)

image_path = "input/Wirtz.jpg"
test_image = Image.open(image_path)

arr = np.array(test_image)
red_c = arr[:,:,0]
green_c = arr[:,:,1]
blue_c = arr[:,:,2]

path = f"dat/Wirtz.dat"
with open(path, 'wb') as file:
    red_c.tofile(file)
    green_c.tofile(file)
    blue_c.tofile(file)

image_path = "input/Who3.jpg"
test_image = Image.open(image_path)

arr = np.array(test_image)
red_c = arr[:,:,0]
green_c = arr[:,:,1]
blue_c = arr[:,:,2]

path = f"dat/who.dat"
with open(path, 'wb') as file:
    red_c.tofile(file)
    green_c.tofile(file)
    blue_c.tofile(file)