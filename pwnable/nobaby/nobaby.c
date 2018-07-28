/*gcc nobaby.c -o nobaby -s -Wl,-z,relro,-z,now*/
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
    alarm(300);
}

void menu(){
	puts("\n***************************************");
    puts("*                                     *");
    puts("* 1 - Add                             *");
	puts("* 2 - Show                            *");
	puts("* 3 - Del                             *");
    puts("* 4 - Exit                            *");
    puts("*                                     *");
    puts("***************************************");
    printf("Your choice: ");
}

char *NOTE[10];
int COUNT;

void Add(){
	int i;
	for (i = 0; i <= 10; i++){
		if (NOTE[i]) continue;
		char *note = malloc(0x60);
		printf("Content: ");
		secure_read(note, 0x60);
		NOTE[i] = note;
		COUNT++;
		puts("Done~");
		return;
	}
	puts("FULL~");
}

void Show(){
	printf("Index: ");
	int index = read_int();
	if (index > 10){
		puts("Out of bound~");
		return;
	}
	if (!NOTE[index]){
		puts("None NOTE~");
		return;
	}
	printf("Content: ");
	printf("%s\n", NOTE[index]);
}

void Del(){
	printf("Index: ");
	int index = read_int();
	free(NOTE[index]);
	NOTE[index] = 0;
	COUNT--;
}

int main(){
	init();
	printf("Name: ");
	char name[0x90];
	memset(name, 0, 0x90);
	secure_read(name, 0x80);
	printf("Hello %s\n", name);
	while (1){
        menu();
        switch (read_int()){
            case 1:
                Add();
                break;
			case 2:
                Show();
                break;
            case 3:
                Del();
                break;
            case 4:
				puts("Bye~~");
				exit(0);
            default:
                puts("Invalid choice!");
        }
	}
}