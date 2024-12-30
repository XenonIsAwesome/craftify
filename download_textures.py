from PIL import Image
from tqdm import tqdm
import requests
import json
import sys
import zipfile
import os


VERSION_MANIFEST_V2_URL = "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json"

def get_json_file(url):
    with requests.get(url) as response:
        return response.json()

manifest_content = get_json_file(VERSION_MANIFEST_V2_URL)

version_tag = sys.argv[1]
if version_tag == "latest":
    version_type = sys.argv[2]
    version_tag = manifest_content["latest"][version_type]

print(f"Downloading textures for version {version_tag}")

version_content = None
for version in manifest_content["versions"]:
    if version["id"] == version_tag:
        version_content = get_json_file(version["url"])
        break

if version_content is None:
    print("Version not found")
    sys.exit(1)

# download client jar
client_jar_url = version_content["downloads"]["client"]["url"]
client_jar = requests.get(client_jar_url)
with open(f"client.jar", "wb") as f:
    f.write(client_jar.content)

# unzip assets/minecraft/textures/block/*.png
with zipfile.ZipFile(f"client.jar", "r") as zip_ref:
    filtered_filelist = [f for f in zip_ref.filelist if f.filename.startswith("assets/minecraft/textures/block/") and f.filename.endswith(".png")]
    for f in tqdm(filtered_filelist, desc="Extracting textures"):
        zip_ref.extract(f)

os.remove("client.jar")

for f in tqdm(filtered_filelist, desc="Filtering textures"):
    img = Image.open(f.filename)
    img = img.convert("RGBA")

    # Removing debug textures
    if "debug" in f.filename:
        try:
            os.remove(f.filename)
        except:
            pass
    
    # Removing non-16x16 textures
    if img.size != (16, 16):
        try:
            os.remove(f.filename)
        except:
            pass
    
    # Removing transparent blocks
    sum_alpha = 0
    for y in range(16):
        for x in range(16):
            _, _, _, a = img.getpixel((x, y))
            sum_alpha += a
    
    avg_alpha = sum_alpha / (16 * 16)
    if avg_alpha < 255:
        try:
            os.remove(f.filename)
        except:
            pass

print("Done.", len(os.listdir("assets/minecraft/textures/block")), "textures left after filtering.")