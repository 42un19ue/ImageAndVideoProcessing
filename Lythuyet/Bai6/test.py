import cv2
import numpy as np

def preprocess_image(file_path):
    image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
    return image

def background_subtraction(image1, image2):
    subtractor = cv2.createBackgroundSubtractorMOG2()

    fg_mask1 = subtractor.apply(image1)
    fg_mask2 = subtractor.apply(image2)

    result = cv2.absdiff(fg_mask1, fg_mask2)

    return result

def main():
    image_path1 = 'image1.png'
    image_path2 = 'image2.png'

    gray_image1 = preprocess_image(image_path1)
    gray_image2 = preprocess_image(image_path2)

    result_image = background_subtraction(gray_image1, gray_image2)

    cv2.imshow('Original Image 1', gray_image1)
    cv2.imshow('Original Image 2', gray_image2)
    cv2.imshow('Motion Detection Result', result_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
