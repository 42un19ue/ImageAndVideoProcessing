import numpy as np
import matplotlib.pyplot as plt

import PIL
from PIL import Image

image_path = "input/Musiala.jpg"
test_image = Image.open(image_path)
plt.figure(figsize=(20, 20))

arr = np.array(test_image)
red_c = arr[:,:,0]
green_c = arr[:,:,1]
blue_c = arr[:,:,2]

channels = [red_c, green_c, blue_c]
names = ["red", "green", "blue"]

for i in range(3):
    path = f"datfile/{names[i]}_image.dat"
    with open(path, 'wb') as file:
        channels[i].tofile(file)

    print(f"Mảng {names[i]} đã được ghi vào file {path}")