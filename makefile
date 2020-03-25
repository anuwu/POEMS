output : oems.c
	mpicc -o oems.exe oems.c 
	mpiexec -n $(NP) oems.exe $(TESTCASE)
	make clean

clean :
	rm *.exe