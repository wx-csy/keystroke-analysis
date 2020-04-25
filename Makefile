SRCS = $(shell find files -name '*_keystrokes.txt')
TARGETS = $(SRCS:files/%.txt=processed/%.txt)
.DEFAULT_GOAL = gen

.PHONY : gen clean

process: process.c
	gcc -Wall -fsanitize=undefined,address -ggdb -std=c11 -o process process.c

processed/%.txt: files/%.txt process
	@mkdir -p $(dir $@)
	@echo \(`find processed -name '*_keystrokes.txt' | wc -l` of `find files -name '*_keystrokes.txt' | wc -l`\) [GEN]\\t$@
	@./process $< > $@ || ( rm $@ && false )

gen : $(TARGETS)

clean :
	rm -rf processed process

.DELETE_ON_ERRORS: $(TARGETS)
