from PIL import Image
import numpy as np

pico8_to_ansi = {
    0x000000: 0 << 4, # 0
    0x1D2B53: 1 << 4, # 236
    0x7E2553: 2 << 4, # 89
    0x008751: 3 << 4, # 29
    0xAB5236: 4 << 4, # 131
    0x5F574F: 5 << 4, # 240
    0xC2C3C7: 6 << 4, # 251
    0xFFF1E8: 7 << 4, # 255
    0xFF004D: 8 << 4, # 197
    0xFFA300: 9 << 4, # 214
    0xFFEC27: 10 << 4, # 11
    0x00E436: 11 << 4, # 41
    0x29ADFF: 12 << 4, # 39
    0x83769C: 13 << 4, # 103
    0xFF77A8: 14 << 4, # 211
    0xFFCCAA: 15 << 4, # 223
    # Special case
    0x3EB2F8: 255
}


def hex_to_rgb(hex_color):
    return tuple(int(hex_color[i : i + 2], 16) for i in (1, 3, 5))


def color_dist(rgb1, rgb2):
    return np.sqrt(sum((c1 - c2) ** 2 for c1, c2 in zip(rgb1, rgb2)))


def find_nearest_ansi(hex_color, ansi_colors):
    min_distance = float("inf")
    closest_code = None
    input_rgb = hex_to_rgb(hex_color)

    for index, ansi_hex in enumerate(ansi_colors):
        ansi_rgb = hex_to_rgb(ansi_hex)
        distance = color_dist(input_rgb, ansi_rgb)
        if distance < min_distance:
            min_distance = distance
            closest_code = index

    return closest_code


def to_greysacle_img(ansi_array):
    height = len(ansi_array)
    width = len(ansi_array[0])

    img_output = Image.new("RGB", (width, height))

    for y in range(height):
        for x in range(width):
            ansi_code = ansi_array[y][x]

            rgb_value = int((ansi_code / 255) * 255)
            img_output.putpixel((x, y), (rgb_value, rgb_value, rgb_value))

    return img_output


def image_to_ansi(image_path, color_map):
    img = Image.open(image_path).convert("RGB")

    ansi_codes = []

    for y in range(img.height):
        row = []
        for x in range(img.width):
            r, g, b = img.getpixel((x, y))
            hex_color = (r << 16) + (g << 8) + b
            
            if hex_color in color_map: row.append(color_map[hex_color])
            else: raise ValueError(f"{hex_color:06x} @ ({x}, {y}) not found.")
        ansi_codes.append(row)
    
    return ansi_codes


result = to_greysacle_img(image_to_ansi("input.png", pico8_to_ansi))
result.save("output.png")
