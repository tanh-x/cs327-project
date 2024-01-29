all:
	@echo -e "  ┌───────────"
	@echo -e "  │ Starting build..."
	@echo -e "  └──"
	make main
	chmod a+rx main
	@echo -e "\n  ┌───────────"
	@echo -e "  │ Finished all Makefile rules. Binary should be \"main\""
	@echo -e "  │ See README file for optional flags (--color, --verbose)."
	@echo -e "  └──"

main: src/main.o src/graphics/artist.o
	$gcc -o $@ $^ -g
	@echo -e "  ┌───────────"
	@echo -e "  │ Finished compile jobs, cleaning object files..."
	@echo -e "  └──"
	make clean_objects

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
