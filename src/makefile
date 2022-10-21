CC=gcc
FLAGS=-Wall -Werror -Wextra -lstdc++ -std=c++17 -lgtest
SRC=logic/unit_tests.cpp
TESTS=logic/tests
all: install

tests: clean
	$(CC) $(FLAGS) $(SRC) -o $(TESTS)
	./$(TESTS)

clean:
	rm -rf $(TESTS)

install:
	cd ../build && make install && make clean && open .

uninstall: 
	cd ../build && make distclean && rm -rf smartcalc2.app

dist:
	cd ../build && make dist

distclean:
	cd ../build && make distclean

dvi:
	open instructions.html

style_test: clean
	clang-format -n logic/*
	clang-format -n controller/*
	clang-format -n graphic_interface/main.cpp
	clang-format -n graphic_interface/mainwindow.h
	clang-format -n graphic_interface/mainwindow.cpp


