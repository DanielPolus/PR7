task1: task1.c
	gcc -o task1 task1.c
task12: task12.c
	gcc -o task12 task12.c
parent: parent.c
	gcc -o parent parent.c
sum: sum.c
	gcc -o sum sum.c
server: server.c
	gcc -o server server.c
client: client.c
	gcc -o client client.c			
task3_1: task3_1.c
	gcc -o task3_1 task3_1.c
task3_2: task3_2.c
	gcc -o task3_2 task3_2.c
task4: task4.c
	gcc -o task4 task4.c	
clean:
	rm -f *.o task3_1 task3_2 task4 parent sum server client task1 task12
