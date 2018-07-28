#include <stdio.h>
#include <string.h>

char name[32];
void *listname[10];
int age;

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

void Addname(){
    int index;
    printf("Index: ");
    index = read_int();
    if (index >= 10) return puts("Out of list name (0 <= index < 10)!!!");
    listname[index] = malloc(0x20);
    printf("Input this name: ");
    secure_read(listname[index], 0x20);
    puts("Done!");
}

void Editname(){
    int index;
    printf("Index: ");
    index = read_int();
    if (index >= 10) return puts("Out of list name (0 <= index < 10)!!!");
    if (!listname[index]) return puts("None name");
    printf("Input new name: ");
    secure_read(listname[index], 0x20);
    puts("Done!");
}

void Delname(){
    int index;
    printf("Index: ");
    index = read_int();
    if (index >= 10) return puts("Out of list name (0 <= index < 10)!!!");
    if (!listname[index]) return puts("None name");
    free(listname[index]);
    puts("Done!");
}

void Showname(){
    int index;
    printf("Index: ");
    index = read_int();
    if (index >= 10) return puts("Out of list name (0 <= index < 10)!!!");
    if (!listname[index]) return puts("None name");
    printf("This name %d is: %s\n", index, listname[index]);
    puts("Done!");
}

void menu(){
    puts("***************Menu****************");
    puts("1 - Create Name");
    puts("2 - Edit Name");
    puts("3 - Delete Name");
    puts("4 - Show Name");
    puts("5 - Exit");
    puts("***************Menu****************");
    printf("Your choice: ");
}

void timeout(){
    puts("Time out!");
    exit(0);
}

int main(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
    signal(14, timeout);
    alarm(60);
    puts("***************************Welcome to easy heap***************************");
    puts("*                                                                        *");
    puts("*************************Challenge Created By CNV*************************");
    puts("*   Team: AceBear                                                        *");
    puts("*   My blog: https://chung96vn.blogspot.com/                             *");
    puts("**************************************************************************");
    printf("Give me your name: ");
    secure_read(name, 0x20);
    printf("Your age: ");
    age = read_int();
    printf("Wellcome: %s\n", name);
    while (1){
        menu();
        switch (read_int()){
            case 1:
                Addname();
                break;
            case 2:
                Editname();
                break;
            case 3:
                Delname();
                break;
            case 4:
                Showname();
                break;
            case 5:
                exit(0);
                break;
            default:
                puts("Invalid choice!");
        }
    }
}
