/*gcc mini-game.c -o mini-game -s*/
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

struct BOSS{
	char NAME[0x10];
	int DAMAGE;
	int HP;
};

struct BOSS *Hero;
struct BOSS *Boss;
char STATUS[0x90];

void Show(struct BOSS* boss){
	printf("* NAME: %s\n", boss->NAME);
	printf("* DAMAGE: %u\n", boss->DAMAGE);
	printf("* HP: %u\n", boss->HP);
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
	Boss = malloc(0x18);
	Boss->DAMAGE = 0x1f0;
	Boss->HP = 0x2100;
	Hero = malloc(0x18);
	Hero->DAMAGE = 0x180;
	Hero->HP = 0x1700;
}

int main(){
	init();
	printf("Name of your Hero: ");
	secure_read(Hero->NAME, 0x10);
	puts("Choice Boss You Want to Kill~");
	puts("1 - Dark Lord");
	puts("2 - Assassins Hunter");
	printf("Your Choice: ");
	switch (read_int()){
        case 1:
            strcpy(Boss->NAME, "Dark Lord");
            break;
		case 2:
            strcpy(Boss->NAME, "Assassins Hunter");
            break;
        default:
            puts("Invalid choice!");
			exit(0);
    }
	puts("***************Hero****************");
	Show(Hero);
	puts("***************Boss****************");
	Show(Boss);
	while((Hero->HP > 0) && (Boss->HP > 0)){
		Hero->HP -= Boss->DAMAGE;
		Boss->HP -= Hero->DAMAGE;
	}
	if (Hero->HP <= 0){
		puts("You Lose!");
		exit(0);
	}
	puts("Congratulation! You Win!");
	char status[0x90];
	printf("Do you want to share with your friends?(Y/N) ");
	secure_read(status, 8);
	if (*status != 'Y'){
		puts("See you again~");
		exit(0);
	}
	memset(status, 0, 0x90);
	printf("Status: ");
	secure_read(status, 0x90);
	snprintf(STATUS, 0x90, status);
	system("echo See your status in www.game-of-me.com");
}