#!/usr/bin/env python3

import requests

with requests.get("https://ia802905.us.archive.org/19/items/TouhouBadApple/Touhou - Bad Apple.mp4") as response:
    with open("test_input.mp4", "wb") as file:
        file.write(response.content)