/*gcc Dead_Note.c -o Dead_Note -z execstack -fPIE -pie -s*/
#include <stdio.h>

int secure_read(char *buf, int size){
    int a = read(0, buf, size);
    if (a <=0 ){
        puts("Go way, Hacker!!!");
        exit(0);
    }
    if (buf[a-1] == 10){
        buf[a-1] = 0;
    }
    return a;
}

int read_int(){
    char ptr[0x10];
    secure_read(ptr, 0x10);
    return atoi(ptr);
}

void timeout(){
    puts("Time out!");
    exit(0);
}

void init(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
    signal(14, timeout);
    alarm(60);
}

int check(char *buf){
	int i;
	for (i = 0; i < strlen(buf); i++){
		char c = buf[i];
		if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || (c == '=')){
			continue;
		}
		else {
			return 1;
		}
	}
	return 0;
}

char *NOTE[1000];
unsigned int COUNT = 1000;

void menu(){
    puts("\n********************Dead Note*******************");
    puts("*                                              *");
    puts("* 1 - Add Note                                 *");
    puts("* 2 - Del Note                                 *");
    puts("* 3 - Exit                                     *");
    puts("*                                              *");
    puts("************************************************");
    printf("Your choice: ");
}

void Add(){
	printf("Index: ");
	int index = read_int();
	printf("Number of Note: ");
	int number = read_int();
	if (number > COUNT || number <= 0){
		puts("Out of bound~~");
		return;
	}
	printf("Content: ");
	char buf[0x10];
	memset(buf, 0, 0x10);
	secure_read(buf, 8);
	if (strlen(buf) > 3){
		puts("Big size~~");
		return;
	}
	/*
	if (check(buf)){
		puts("Special characters~~");
		return;
	}*/
	int i;
	for (i = 0; i < number; i++){
		NOTE[index+i] = strdup(buf);
	}
	COUNT -= number;
	puts("Done~~");
}

void Del(){
	printf("Index: ");
	int index = read_int();
	if (NOTE[index]){
		puts("Can not delete blank note~~");
	}
	free(NOTE[index]);
	NOTE[index] = 0;
	COUNT++;
}

int main(){
	init();
	while (1){
        menu();
        switch (read_int()){
            case 1:
                Add();
                break;
            case 2:
                Del();
                break;
            case 3:
		puts("Bye~~");
		exit(0);
            default:
                puts("Invalid choice!");
            }
	}
}
