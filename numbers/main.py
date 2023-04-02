## Want to crop the image and convert to bmp
## Then save the image to a folder

import cv2
import numpy as np
import os

for i in range(10):

    # Read the image
    path = str(i) + '.png'
    img = cv2.imread(path)

    # Crop the image
    #crop_img = img[160:530, 420:1018]
    crop_img = img[350:1700, 670:1730]

    # Convert to bmp
    final_path = str(i)+'.bmp'
    cv2.imwrite(final_path, crop_img)

