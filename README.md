# Craftify

## How to clone correctly
```bash
git clone https://github.com/XenonIsAwesome/craftify.git
cd craftify
git submodule update --init --recursive
```

## Before running
1. Make sure to install the required python requirements (`pip install -r requirements.txt`)
2. Download textures by running `python download_textures.py <version-tag>`
    <br>
    Examples:
    <br>
    a. `python download_textures.py 1.21.4`
    <br>
    b. `python download_textures.py 24w44a`
    <br>
    c. `python download_textures.py latest release`
    <br>
    d. `python download_textures.py latest snapshot`
    <br>
3. Run the `bake_blocks.py` script (`python bake_blocks.py`)
4. Install the build dependencies (`OpenCV`, `cmake`, `gcc`)
5. Run the following to build the C++ converter:
```bash
# First time only
mkdir -p build
cd build
cmake ..
cd ..

# Every change
./compile-cpp.sh
```

## Usage example

### mode=normal
![bad_apple_example.gif](bad_apple_example.gif)
Command used:
```bash
python -m craftify -i bad_apple.mp4 --scale-factor 2 bad_apple_example.mp4
```

### mode=lamp
![bad_apple_lamp_example.gif](bad_apple_lamp_example.gif)
Command used:
```bash
python -m craftify -i bad_apple.mp4 --mode lamp --scale-factor 2 bad_apple_lamp_example.mp4
```