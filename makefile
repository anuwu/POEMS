output : oems.c
	mpicc -o oems.exe oems.c 
	mpiexec -n $(NP) oems.exe
	make clean

clean :
	rm *.exe