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

path = f"datfile/raw.dat"
with open(path, 'wb') as file:
    red_c.tofile(file)
    green_c.tofile(file)
    blue_c.tofile(file)
