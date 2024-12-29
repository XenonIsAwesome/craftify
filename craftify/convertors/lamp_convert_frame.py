from PIL import Image
import sys

from dataclasses import dataclass


@dataclass
class PixelRGB:
    red: int
    green: int
    blue: int
    alpha: int


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

    # Creating the output 'canvas'
    output = Image.new("RGBA", (img.width * 16, img.height * 16))

    # Iterate over the pixels
    for y in range(img.height):
        for x in range(img.width):
            # Get the pixel RGB values
            pixel_rgb = PixelRGB(*img.getpixel((x, y)), 255)
            avg = (pixel_rgb.red + pixel_rgb.green + pixel_rgb.blue) // 3
            
            lamp_on = avg > 128
            
            # Get the block and its texture
            block = "redstone_lamp_on.png" if lamp_on else "redstone_lamp_off.png"
            block_texture = Image.open("static/textures/" + block)
            
            # Paste the block texture onto the output image
            output.paste(block_texture, (x * 16, y * 16))

    return output


if __name__ == "__main__":
    input_image_path = sys.argv[1]
    output_image_path = sys.argv[2]

    # Parsing scale factor as 16/x
    scale_factor = int(sys.argv[3]) if len(sys.argv) > 3 else 1
    downscale_factor = 16 // scale_factor

    downscaled_result = downscale_image(input_image_path, scale_factor=downscale_factor)
    converted_result = convert_image_to_minecraft_blocks(downscaled_result)
    converted_result.save(output_image_path)
