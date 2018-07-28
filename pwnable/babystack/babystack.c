/*gcc babystack.c -o babystack -s -Wl,-z,relro,-z,now*/
#include <stdio.h>
#include <time.h>

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

void initpw(char *password){
	srand(time(0));
	int i;
	for (i = 0; i < 0x18; i++){
		password[i] = rand()&0xff;
	}
}

void menu(){
	puts("\n***************************************");
    puts("*                                     *");
    puts("* 1 - Login                           *");
    puts("* 2 - Play                            *");
	puts("* 3 - Exit                            *");
    puts("*                                     *");
    puts("***************************************");
    printf("Your choice: ");
}

char user[0x20];

void Login(char *password){
	char username[0x30];
	char pw[0x30];
	memset(username, 0, 0x30);
	memset(pw, 0, 0x30);
	printf("User Name: ");
	secure_read(username, 0x20);
	if (!memcmp(username, "admin", 5)){
		printf("Password: ");
		int size = secure_read(pw, 0x20);
		if (!memcmp(password, pw, size)){
			memcpy(user, "admin", 5);
			puts("Logined~");
		}
		else{
			puts("Fail~");
		}
	}
	else{
		memcpy(user, username, strlen(username));
		puts("Logined~");
	}
}

void Play(){
	if (memcmp(user, "admin", 5)){
		puts("Test Version only support for admin~");
		return;
	}
	printf("Content: ");
	char buf[0x20];
	secure_read(buf, 0x80);
	puts("Every things is OK~~");
}

	
int main(int argc, char ** argv, char ** env){
	init();
	char password[0x18];
	memset(password, 0, 0x18);
	initpw(password);
	while (1){
        menu();
        switch (read_int()){
            case 1:
                Login(password);
                break;
			case 2:
                Play();
                break;
            case 3:
				puts("Bye~~");
				exit(0);
            default:
                puts("Invalid choice!");
        }
	}
}