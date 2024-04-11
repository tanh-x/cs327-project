#ifndef TEXT_H
#define TEXT_H


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
#define TITLE_POKE327_COLORED_GENERATED "\n   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m  \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m  \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\n   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m\n   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m    \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m\n   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m   \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m    \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m \n   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m     \x1b[38;5;25m╚\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m  \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╔\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m╗\x1b[0m   \x1b[38;5;221m█\x1b[0m\x1b[38;5;221m█\x1b[0m\x1b[38;5;25m║\x1b[0m  \n   \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m      \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m  \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m\x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m   \x1b[38;5;25m╚\x1b[0m\x1b[38;5;25m═\x1b[0m\x1b[38;5;25m╝\x1b[0m  \n"

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

#define LOCATION_NAME_PREFIXES { \
    "PEWTER", \
    "CERULEAN", \
    "LAVENDER", \
    "VIRIDIAN", \
    "SAFFRON", \
    "GOLDENROD", \
    "ECRUTEAK", \
    "FORTREE", \
    "LILYCOVE", \
    "SUNYSHORE", \
    "CASTELIA", \
    "LUMIOSE", \
    "MOTOSTOKE", \
    "VIRIDIAN", \
    "CINNABAR", \
    "FUCHSIA", \
    "OLIVINE", \
    "AZALEA", \
    "BLACKTHORN", \
    "MOSSDEEP", \
    "SOOTOPOLIS", \
    "VEILSTONE", \
    "HEARTHOME", \
    "NIMBASA", \
    "DRIFTVEIL", \
    "OPELUCID", \
    "SNOWPOINT", \
    "PACIFIDLOG", \
    "FLOAROMA", \
    "CANALAVE", \
    "MYSTIC", \
    "OMINOUS", \
    "FLOWERY", \
    "MYSTERIOUS", \
    "LUSH", \
    "LAVENDER", \
    "GRASSY" \
}

#define LOCATION_NAME_SUFFIXES { \
    "WOODLAND", \
    "LAKES", \
    "WOODS", \
    "FOREST", \
    "HILLS", \
    "ZONE", \
    "ROADS", \
    "PATH", \
    "MARSH", \
    "PRAIRIE", \
    "GROVE", \
    "GARDEN", \
    "FIELDS", \
    "PLAINS", \
    "CLIFFS", \
    "ORCHARD", \
    "CLEARING" \
}


#endif
