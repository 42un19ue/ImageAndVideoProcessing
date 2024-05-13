from PIL import Image
import numpy as np

file_path = "datfile/grayscale.dat"
width = 2048
height = 1365

with open(file_path, "rb") as file:
    grayscale_data = np.fromfile(file, dtype=np.int8, count=-1)

image_matrix = grayscale_data.reshape((height, width))

image = Image.fromarray(image_matrix.astype(np.uint8))

image.save("output/output_image.png")

print(f"Image saved to output/output_image.png")