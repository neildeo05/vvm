.PHONY: tests

interpret: interpret.c
	gcc -Wall -o interpret interpret.c
test: interpret
	./interpret
test_suite: test_suite.c
	gcc -Wall -o test_suite test_suite.c
suite_tests: test_suite
	./test_suite


