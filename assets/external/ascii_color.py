title = """
   ██████╗  ██████╗ ██╗  ██╗███████╗██████╗ ██████╗ ███████╗
   ██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝╚════██╗╚════██╗╚════██║
   ██████╔╝██║   ██║█████╔╝ █████╗   █████╔╝ █████╔╝    ██╔╝
   ██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝   ╚═══██╗██╔═══╝    ██╔╝ 
   ██║     ╚██████╔╝██║  ██╗███████╗██████╔╝███████╗   ██║  
   ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═════╝ ╚══════╝   ╚═╝  
"""

foreground = "\033[38;5;221m"
background = "\033[38;5;25m"
reset = "\033[0m"

def colorize_ascii(ascii_art):
    colored_art = ""
    for char in ascii_art:
        if char == '█':
            colored_art += foreground + char + reset
        elif char in " \n":
            colored_art += char
        else:
            colored_art += background + char + reset
    return colored_art

colored_title = colorize_ascii(title)
# print(colored_title)
print(repr(colored_title))