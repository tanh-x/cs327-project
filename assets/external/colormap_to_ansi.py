import numpy as np


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


with open("ansi.txt", "r") as file:
    ansi_colors = file.read().strip().split()

print(
    [
        find_nearest_ansi(color, ansi_colors)
        for color in [
            "#d73027"
            "#f46d43"
            "#fdae61"
            "#fee08b"
            "#ffffbf"
            "#d9ef8b"
            "#a6d96a"
            "#66bd63"
            "#1a9850"
        ]
    ]
)
