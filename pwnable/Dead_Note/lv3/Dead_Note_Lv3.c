/*gcc Dead_Note.c -o Dead_Note_Lv3 -s*/
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
	puts("* 2 - Edit                            *");
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
		char *note = malloc(0x88);
		printf("Content: ");
		secure_read(note, 0x88);
		NOTE[i] = note;
		COUNT++;
		puts("Done~");
		return;
	}
	puts("FULL~");
}

void Edit(){
	printf("Index: ");
	unsigned int index = read_int();
	if (index > 10){
		puts("Out of bound~");
		return;
	}
	if (!NOTE[index]){
		puts("None NOTE~");
		return;
	}
	printf("Content: ");
	secure_read(NOTE[index], 0x88);
}

void Del(){
	printf("Index: ");
	unsigned int index = read_int();
	if (index > 10){
		puts("Out of bound~");
		return;
	}
	free(NOTE[index]);
	NOTE[index] = 0;
	COUNT--;
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
                Edit();
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