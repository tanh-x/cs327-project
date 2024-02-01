all:
	@echo -e "  ┌───────────"
	@echo -e "  │ Starting build..."
	@echo -e "  └──"
	make main
	@echo -e "  ┌───────────"
	@echo -e "  │ Finished compile jobs, cleaning object files..."
	@echo -e "  └──"
	make clean_objects
	chmod a+rx main
	@echo -e "\n  ┌───────────"
	@echo -e "  │ Finished all Makefile rules. Binary should be \"main\""
	@echo -e "  │ "
	@echo -e "  │ See README file for optional flags (--color, --badapple)."
	@echo -e "  └──\n"

main: src/graphics/artist.o src/graphics/parse_frame.c src/utils/voronoi_noise.c src/utils/mathematics.c src/world/mapbuilder.c src/main.o
	$gcc -o $@ $^ -g -lm -lpng

%.o: %.c
	gcc -c -o $@ $< -Iinclude -Wall -g

clean_objects:
	find . -name '*.o' -exec rm {} +

clean:
	@echo -e "  ┌───────────"
	@echo -e "  │ Cleaning object and binary files..."
	@echo -e "  └──"
	make clean_objects
	rm -vf main
