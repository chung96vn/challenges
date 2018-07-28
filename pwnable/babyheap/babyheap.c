/*gcc babyheap.c -o babyheap -s*/
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

void menu(){
	puts("\n***************************************");
    puts("*                                     *");
    puts("* 1 - Add                             *");
    puts("* 2 - Edit                            *");
	puts("* 3 - Show                            *");
	puts("* 4 - Del                             *");
    puts("* 5 - Exit                            *");
    puts("*                                     *");
    puts("***************************************");
    printf("Your choice: ");
}

char NAME[0x20] = "\x00";
char* NOTE[10];


void Add(){
	int i;
	for (i = 0; i < 10; i++){
		if (!NOTE[i]) break;
	}
	if (i == 10){
		puts("FULL~");
		return;
	}
	NOTE[i] = malloc(0x60);
	printf("Content: ");
	secure_read(NOTE[i], 0x60);
	puts("Done~");
}

void Edit(){
	printf("Index: ");
	int index = read_int();
	if (index < 0 || index >= 10){
		puts("Out of bound~");
		return;
	}
	if (!NOTE[index]){
		puts("None NOTE~");
		return;
	}
	printf("Content: ");
	secure_read(NOTE[index], 0x60);
	puts("Done~");
}

void Show(){
	printf("Index: ");
	int index = read_int();
	if (index < 0 || index >= 10){
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
	if (index < 0 || index >= 10){
		puts("Out of bound~");
		return;
	}
	if (!NOTE[index]){
		puts("None NOTE~");
		return;
	}
	free(NOTE[index]);
}

int main(){
	init();
	printf("Name: ");
	secure_read(NAME, 0x20);
	while (1){
        menu();
        switch (read_int()){
            case 1:
                Add();
                break;
			case 2:
                Edit();
                break;
			case 3:
                Show();
                break;
            case 4:
                Del();
                break;
            case 5:
				puts("Bye~~");
				exit(0);
            default:
                puts("Invalid choice!");
        }
	}
}