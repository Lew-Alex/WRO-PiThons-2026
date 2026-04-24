from PIL import Image
import numpy as np

# Load original image in grayscale
img = Image.open("WRO Robomission Field.png").convert("RGB")


# Resize to smaller dimensions
new_width, new_height = int(2362 * 0.2), int(1143 * 0.2)  # adjust as needed
img_small = img.resize((new_width, new_height), Image.ANTIALIAS)

# Save the downscaled image
img_small.save("map_downscaled.png")

# Optional: convert to NumPy array for further processing
map_array = np.array(img_small)

print("Downscaled map shape:", map_array.shape)

for y in range(228):
    print("{", end='')
    for x in range(472):
        print(int(map_array[y][x] / 2),  ",", end=' ', sep='')
    print("},")