import os
from PIL import Image
import json


BASE_PATH = 'minecraft-artifier-js/static/textures/'
images = sorted(os.listdir(BASE_PATH))
blocks_db = []

for i, img_path in enumerate(images):
    img = Image.open(BASE_PATH + img_path)
    img.convert("RGBA")

    sum_r = 0
    sum_g = 0
    sum_b = 0

    for y in range(img.height):
        for x in range(img.width):
            rgb = img.getpixel((x, y))
            sum_r += rgb[0]
            sum_g += rgb[1]
            sum_b += rgb[2]

    total_pixels = img.width * img.height
    avg_r = sum_r // total_pixels
    avg_g = sum_g // total_pixels
    avg_b = sum_b // total_pixels

    image_dict = {
        "name": img_path.rsplit('.', maxsplit=1)[0],
        "r": avg_r,
        "g": avg_g,
        "b": avg_b,
        "texture_image": img_path,
        "index": i
    }

    blocks_db.append(image_dict)

with open('blocks_db.json', 'w') as f:
    json.dump(blocks_db, f)
