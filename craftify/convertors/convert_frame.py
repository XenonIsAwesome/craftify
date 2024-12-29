from PIL import Image
from typing import Optional
import sys

from dataclasses import dataclass
import json


@dataclass
class PixelRGB:
    red: int
    green: int
    blue: int
    alpha: int


@dataclass
class BakedBlock:
    name: str
    texture_image: str
    game_id: str
    game_id_13: str
    block_id: int
    data_id: int
    luminance: bool
    transparency: bool
    falling: bool
    redstone: bool
    survival: bool
    version: int
    index: int
    red: int
    green: int
    blue: int
    h: int
    s: int
    l: int
    axis: str = ""
    deviation: int = 100e6


class BlocksDB:
    instance: 'BlocksDB' = None
    def __init__(self):
        self.blocks = []
        with open("static/baked_blocks.json", "r") as file:
            entries = json.load(file)
            for entry_idx in range(len(entries)):
                entries[entry_idx]["index"] = entries[entry_idx]["id"]
                del entries[entry_idx]["id"]

                self.blocks.append(BakedBlock(**entries[entry_idx]))
    
    @staticmethod
    def getInstance() -> 'BlocksDB':
        if BlocksDB.instance is None:
            BlocksDB.instance = BlocksDB()
        return BlocksDB.instance

    @staticmethod
    def copy():
        return BlocksDB.getInstance().blocks.copy()
    
    def __getitem__(self, index: int):
        return self.blocks[index]
    
    def __len__(self):
        return len(self.blocks)



def downscale_image(image_path: str, scale_factor: int = 16) -> Image:
    # Open an image file
    with Image.open(image_path) as img:
        # Calculate the new size
        new_size = (img.width // scale_factor, img.height // scale_factor)
        
        # Resize the image
        scaled_img = img.resize(new_size, Image.LANCZOS)
        return scaled_img


def convert_image_to_minecraft_blocks(img: Image) -> Image:
    img.convert("RGBA")

    # Load the blocks database
    bdb = BlocksDB.getInstance()

    # Creating the output 'canvas'
    output = Image.new("RGBA", (img.width * 16, img.height * 16))

    # Iterate over the pixels
    for y in range(img.height):
        for x in range(img.width):
            # Initialize the closest item and key
            closest_item: Optional[BakedBlock] = None
            closest_key = 0
            
            # Get the pixel RGB values
            pixel_rgb = PixelRGB(*img.getpixel((x, y)), 255)

            # Check if the pixel is not transparent
            if pixel_rgb.alpha > 70:
                
                # Iterate over the blocks database to find the closest match
                for key in range(len(bdb)):
                    item = bdb[key]
                    # Calculate the deviation between the pixel and the block color
                    dev = abs(pixel_rgb.red - item.red) + abs(pixel_rgb.green - item.green) + abs(pixel_rgb.blue - item.blue)
                    # Update the closest item if a closer match is found
                    if closest_item is None or dev < closest_item.deviation:
                        closest_key = key
                        closest_item = item
                        closest_item.deviation = dev
            
            # Get the closest block and its texture
            block = bdb[closest_key]
            block_texture = Image.open("static/textures/" + block.texture_image)
            
            # Paste the block texture onto the output image
            output.paste(block_texture, (x * 16, y * 16))

    return output


if __name__ == "__main__":
    input_image_path = sys.argv[1]
    output_image_path = sys.argv[2]

    # Parsing scale factor as 16/x
    scale_factor = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    downscale_factor = 16 // scale_factor

    # print("[FrameConverter] Converting", input_image_path, "to", output_image_path)

    downscaled_result = downscale_image(input_image_path, scale_factor=downscale_factor)
    converted_result = convert_image_to_minecraft_blocks(downscaled_result)
    converted_result.save(output_image_path)

    # print("[FrameConverter] Converted", input_image_path, "to", output_image_path)