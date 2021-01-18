.PHONY: tests

interpret: interpret.c
	gcc -Wall -o interpret interpret.c
test: interpret
	./interpret
machine: virtual.c
	gcc -Wall -o virtual virtual.c
machine_tests: machine
	./virtual


