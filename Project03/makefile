build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

run_fast:
	./app.exe -f

run_fast_quiet:
	./app.exe -f -q

run_year_fast:
	./app.exe -y 2020 -f

valgrind:
	rm -f app.exe
	gcc -g app.c MinPopVote.c -o app.exe
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe -f

build_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe

run_test:
	./test.exe

build_timing:
	rm -f app_timing.exe
	gcc -pg app.c MinPopVote.c -o app_timing.exe

run_timing:
	./app_timing.exe -f

profile:
	gprof app_timing.exe gmon.out > analysis.txt

clean:
	rm -f app.exe test.exe app_timing.exe gmon.out analysis.txt

.PHONY: build run run_quiet run_fast run_fast_quiet run_year_fast valgrind build_test run_test build_timing run_timing profile clean