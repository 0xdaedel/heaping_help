#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct tagOBJ{
	struct tagOBJ* fd;
	char buf[8];
	struct tagOBJ* bk;

}OBJ;

void shell(){
	system("/bin/sh");
}

void unlink(OBJ* P){
	OBJ* tBkwd;
	OBJ* tFwd;
	tBkwd=P->bk;
	tFwd=P->fd;
	tFwd->bk=tBkwd;
	tBkwd->fd=tFwd;
}
int main(int argc, char* argv[]){
	malloc(1024);
	OBJ* A = (OBJ*)malloc(sizeof(OBJ));
	OBJ* B = (OBJ*)malloc(sizeof(OBJ));
	OBJ* C = (OBJ*)malloc(sizeof(OBJ));

	// double linked list: A <-> B <-> C
	A->fd = B;
	B->bk = A;
	B->fd = C;
	C->bk = B;

	printf("here is stack address leak: %p\n", &A);
	printf("here is heap address leak: %p\n", A);
	printf("now that you have leaks, get shell!\n");
	// heap overflow!
	gets(A->buf);

	// exploit this unlink!
	unlink(B);
	return 0;
}

