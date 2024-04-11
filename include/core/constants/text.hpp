#ifndef TEXT_H
#define TEXT_H

#define TRAINER_NAMES { \
    "Misty", \
    "Brock", \
    "Lance", \
    "Blue", \
    "Cynthia", \
    "Steven", \
    "Giovanni", \
    "Sabrina", \
    "Erika", \
    "Koga", \
    "Lt. Surge", \
    "Jessie", \
    "James", \
    "May", \
    "Dawn", \
    "Serena", \
    "Lillie", \
    "Gladion", \
    "Hilda", \
    "Hilbert", \
    "Bianca", \
    "Cheren", \
    "Iris", \
    "Calem", \
    "Lysandre", \
    "Leon", \
    "Raihan" \
}

#define NUM_TRAINER_NAMES 27


// Looks like this:
//
//   ██████╗  ██████╗ ██╗  ██╗███████╗██████╗ ██████╗ ███████╗
//   ██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝╚════██╗╚════██╗╚════██║
//   ██████╔╝██║   ██║█████╔╝ █████╗   █████╔╝ █████╔╝    ██╔╝
//   ██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝   ╚═══██╗██╔═══╝    ██╔╝
//   ██║     ╚██████╔╝██║  ██╗███████╗██████╔╝███████╗   ██║
//   ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚══════╝   ╚═╝
//
// But with background and foreground colored differently.
// The string here is generated with a Python script (located in assets/external/ascii_color.py)
#define TITLE_POKE327_COLORED_GENERATED "\n   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m  \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m  \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\n   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m\n   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m    \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m\n   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m   \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m    \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m \n   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m     \x1b[38;5;26m╚\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m  \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╔\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m╗\x1b[0m   \x1b[38;5;227m█\x1b[0m\x1b[38;5;227m█\x1b[0m\x1b[38;5;26m║\x1b[0m  \n   \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m      \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m  \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m\x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m   \x1b[38;5;26m╚\x1b[0m\x1b[38;5;26m═\x1b[0m\x1b[38;5;26m╝\x1b[0m  \n"

#endif
