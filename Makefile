C_COMPILER     = gcc
C_OPTIONS      = -Wall -Wextra -Wpedantic -g
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit
GCOV_FLAG	   = -coverage

DATA_STRUCTURES = hash_table.c linked_list.c

#BUSSINESSLOGIC#
bl: businesslogic.c businesslogic_tests.c $(DATA_STRUCTURES)
	$(C_COMPILER) $(C_OPTIONS) $^ -o $@ $(C_LINK_OPTIONS) $(CUNIT_LINK)

testbl: bl
	./bl

memtestbl: bl
	valgrind --leak-check=full ./bl

gdbtestbl: bl
	gdb ./bl

#USERINTERFACE#
ui: businesslogic.c utils.c user_interface.c $(DATA_STRUCTURES)
	$(C_COMPILER) $(C_OPTIONS) $^ -o $@ $(C_LINK_OPTIONS) $(CUNIT_LINK)

testui: ui
	./ui

testauto: ui
	./ui < webstoretest.txt

memtestauto: ui
	valgrind --leak-check=full ./ui < webstoretest.txt