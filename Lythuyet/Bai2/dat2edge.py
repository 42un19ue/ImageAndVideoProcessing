from PIL import Image
import numpy as np

WIDTH = 320
HEIGHT = 320
f = open("datfile/edge.dat", "rb")
data = f.read()
f.close()
data = np.frombuffer(data, dtype = np.uint8)
img = Image.fromarray(data.reshape(HEIGHT, WIDTH), mode = "L")
img.save("output/edge.jpg")
