#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct memo{
    char *name;
    int size;
    int state;
};

int secure_read(char *buf, int size){
    int a = read(0, buf, size);
    if (a < 0 ){
        puts("Go way, Hacker!!!");
        exit(0);
    }
    else{
        if (buf[a-1] == 10){
            buf[a-1] = 0;
        }
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
    puts("***************Menu****************");
    puts("1 - Create memo");
    puts("2 - Edit memo");
    puts("3 - Show memo");
    puts("4 - Delete memo");
    puts("5 - Exit");
    puts("***************Menu****************");
    printf("Your choice: ");
}

struct memo *listmeno[10];

int Create(){
    int i;
    for(i = 0; i < 10; i++){
        if (listmeno[i] == 0){
            break;
        }
    }
    if (i == 10){
        puts("Full memo!");
        return 0;
    }
    struct memo *mm = malloc(0x10);
    printf("What is size of memo you want to create? ");
    mm->size = read_int();
    mm->name = malloc(mm->size);
    if (!mm->name){
        puts("Can not malloc! Please report this bug!");
        exit(0);
    }
    printf("Name of memo: ");
    secure_read(mm->name, mm->size);
    mm->state = 1;
    listmeno[i] = mm;
    puts("Done!");
    return 0;
}

int Edit(){
    printf("Index of memo you want to edit: ");
    int index = read_int();
    if (index < 0 || index >= 10){
        puts("You are hacker???");
        exit(0);
    }
    if (listmeno[index]){
        struct memo *mm = listmeno[index];
        if (!mm->state){
            puts("Dead memo!");
            return 0;
        }
        mm->state--;
        char *name = realloc(mm->name, mm->size);
        if (name){
            mm->name = name;
            printf("New name of memo: ");
            secure_read(mm->name, mm->size);
            puts("Done!");
            return 0;
        }
        else{
            puts("Can not edit this memo!");
            return 0;
        }
    }
    else{
        puts("None memo!");
        return 0;
    }
}

void Show(){
    printf("Index of memo you want to show: ");
    int index = read_int();
    if (index < 0 || index >= 10){
        puts("You are hacker???");
        exit(0);
    }
    if (listmeno[index]){
        struct memo *mm = listmeno[index];
        if (!mm->state){
            puts("Dead memo!");
            return 0;
        }
        puts("/n***********Meno***********");
        printf("Memo of index %d:\n", index);
        printf("Name: %s\nSize: %d\nState: %d\n", mm->name, mm->size, mm->state);
        puts("Done!");
        puts("***********Meno***********/n");
        return;
    }
    else{
        puts("None memo!");
        return;
    }
}

int Delete(){
    printf("Index of memo you want to delete: ");
    int index = read_int();
    if (index < 0 || index >= 10){
        puts("You are hacker???");
        exit(0);
    }
    if (listmeno[index]){
        struct memo *mm = listmeno[index];
        free(mm->name);
        mm->name = 0;
        free(mm);
        listmeno[index] = 0;
        puts("Done!");
    }
    else{
        puts("None memo!");
        return;
    }
}

int main(){
    init();
    puts("***************************Welcome to memo heap***************************");
    puts("*                                                                        *");
    puts("*************************Challenge Created By CNV*************************");
    puts("*   Team: AceBear                                                        *");
    puts("*   My blog: https://chung96vn.blogspot.com/                             *");
    puts("**************************************************************************");
    puts("\n");
    while(1){
        menu();
        switch (read_int()){
            case 1:
                Create();
                break;
            case 2:
                Edit();
                break;
            case 3:
                Show();
                break;
            case 4:
                Delete();
                break;
            case 5:
                exit(0);
                break;
            default:
                puts("Invalid choice!");
        }
    }
}