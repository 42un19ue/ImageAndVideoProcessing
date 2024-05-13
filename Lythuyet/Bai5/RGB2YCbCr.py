from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.image import imsave

def rgb2ycbcr(rgb):
    cbcr = np.empty_like(rgb)
    r, g, b = rgb[:, :, 0], rgb[:, :, 1], rgb[:, :, 2]
    # Y
    cbcr[:, :, 0] = 16 + (65.738 * r + 129.057 * g + 25.064 * b) / 256
    # Cb
    cbcr[:, :, 1] = 128 - (37.945 * r + 74.494 * g - 112.439 * b) / 256
    # Cr
    cbcr[:, :, 2] = 128 + (112.439 * r - 94.154 * g - 18.285 * b) / 256
    return np.uint8(cbcr)
def downsample_422(ycbcr_444_img):
    height, width, _ = ycbcr_444_img.shape

    y_channel = ycbcr_444_img[:, :, 0]

    cb_channel = ycbcr_444_img[:, 0:width:2, 1]
    cr_channel = ycbcr_444_img[:, 0:width:2, 2]

    ycbcr_422_img = np.zeros((height, width, 3), dtype=np.uint8)

    ycbcr_422_img[:, :, 0] = y_channel

    ycbcr_422_img[:, :, 1] = np.repeat(cb_channel, 2, axis=1)
    ycbcr_422_img[:, :, 2] = np.repeat(cr_channel, 2, axis=1)

    return ycbcr_422_img

def ycbcr2rgb(ycbcr):
    height, width, _ = ycbcr.shape
    rgb_img = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(height):
        for j in range(width):
            y, cb, cr = ycbcr[i, j]
            r = y + 1.402 * (cr - 128)
            g = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128)
            b = y + 1.772 * (cb - 128)
            rgb_img[i, j] = np.clip([r, g, b], 0, 255).astype(np.uint8)

    return np.uint8(rgb_img)

def psnr(image1, image2):
    image1 = image1.astype(np.float32)
    image2 = image2.astype(np.float32)

    mse = np.mean((image1 - image2) ** 2)

    psnr = 10 * np.log10((255.0 ** 2) / mse)
    return psnr

def save_image(image, output_path):
    imsave(output_path, image)

def process_image(input_path):
    rgb_img = Image.open(input_path)
    rgb = np.array(rgb_img)

    img_1 = rgb2ycbcr(rgb)
    img_2 = downsample_422(np.array(img_1))
    img_3 = ycbcr2rgb(np.array(img_2))

    psnr_original_vs_rgb2ycbcr = psnr(rgb, np.array(img_1))
    psnr_original_vs_downsampled = psnr(rgb, np.array(img_2))
    psnr_original_vs_ycbcr2rgb = psnr(rgb, np.array(img_3))

    # In giá trị PSNR
    print(f'PSNR Original vs RGB2YCbCr: {psnr_original_vs_rgb2ycbcr:.2f} dB')
    print(f'PSNR Original vs Downsampled: {psnr_original_vs_downsampled:.2f} dB')
    print(f'PSNR Original vs YCbCr2RGB: {psnr_original_vs_ycbcr2rgb:.2f} dB')

    return rgb, img_1, img_2, img_3
def main():
    input_path = 'input/Musiala.jpg'
    rgb, img_1, img_2, img_3 = process_image(input_path)

    save_image(rgb, 'output/Original.png')
    save_image(np.array(img_1), 'output/RGB2YCbCr.png')
    save_image(np.array(img_2), 'output/YCbCr444.png')
    save_image(np.array(img_3), 'output/YCbCr422toRGB.png')

if __name__ == "__main__":
    main()