C_COMPILER     = gcc
C_OPTIONS      = -Wall -Wextra -Wpedantic -g
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit
GCOV_FLAG	   = -coverage
AUTO_UI		   = webstoretestown2.txt

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

memtestui: ui
	valgrind --leak-check=full ./ui

testauto: ui
	./ui < $(AUTO_UI)

memtestauto: ui
	valgrind --leak-check=full ./ui < $(AUTO_UI)

#COVERAGE#
bl_coverage: businesslogic.c businesslogic_tests.c $(DATA_STRUCTURES)
	$(C_COMPILER) $(GCOV_FLAG) $(C_OPTIONS) $^ -o $@ $(C_LINK_OPTIONS) $(CUNIT_LINK)

covtestbl: bl_coverage
	./bl_coverage
	gcov -b bl_coverage-businesslogic.gcda

#HTML COVERAGE REPORT#
covreport: bl_coverage
	./bl_coverage
	lcov --directory . --capture --output-file app.info --rc lcov_branch_coverage=1
	genhtml -o html_report app.info --branch-coverage

.PHONY: clear

#CLEAR#
clear:
	rm -rf html_report *.gcda *.gcno bl bl_coverage *.info ui