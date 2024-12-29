# Craftify

## How to clone correctly
```bash
git clone https://github.com/XenonIsAwesome/craftify.git
cd craftify
git submodule update --init --recursive
```

## Before running
1. Make sure to install the required requirements (`pip install -r requirements`)
2. Run the `bake_blocks.py` script (`python bake_blocks.py`)


## Building the cpp converter
1. Install `OpenCV`
2. Run the following:
```bash
mkdir build && cd build
cmake ..
make
```

## Example

### mode=normal
![bad_apple_example.gif](bad_apple_example.gif)
Command used:
```bash
python3 -m craftify -i bad_apple.mp4 --scale-factor 2 bad_apple_example.mp4
```

### mode=lamp
![bad_apple_lamp_example.gif](bad_apple_lamp_example.gif)
Command used:
```bash
python3 -m craftify -i bad_apple.mp4 --mode lamp --scale-factor 2 bad_apple_lamp_example.mp4
```