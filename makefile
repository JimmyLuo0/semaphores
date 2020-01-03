all: sem_control.c sem_user.c 
	gcc -o control sem_control.c 
	gcc -o user sem_user.c
	

