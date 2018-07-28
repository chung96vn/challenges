//gcc -fPIE -pie -s -g -Wl,-z,relro,-z,now max_heap.c -o max_heap
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

struct Comic{/*Size = 0x10*/
    char *name;
    unsigned int price;
    unsigned int quantity;
};

struct Cart{/*Size = 0x18*/
    struct Comic *comic;
    struct Cart *prev;
    struct Cart *next;
};

struct Customer{/*Size = 0xc*/
    char *name;
    unsigned int money; 
};

struct Feedback{/*Size = 0x1c*/
    char *content[3];
    unsigned int count;
};

struct Order{/*Size = 0x28*/
    struct Customer *customer;
    struct Cart **listcart;
    struct Feedback *feedback;
    unsigned int *quantity;
    unsigned int total;
    unsigned int count;
};

int secure_read(char *buf, int size){
    int a = read(0, buf, size);
    if (a <= 0 ){
        error();
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
    memset(ptr, 0, 0x10);
    secure_read(ptr, 0x10);
    return atoi(ptr);
}

char *newstr(char *buf){
    int len = strlen(buf);
    char *result = malloc(len+1);
    if (!result) error();
    memcpy(result, buf, len);
    return result;
}

void timeout(){
    puts("Time out!");
    exit(0);
}

void error(){
    puts("error");
    exit(0);
}

struct Cart *listCart;

struct Cart* createCart(char *name, unsigned int price, unsigned int quantity){
    struct Comic *comic = calloc(1, 0x10);
    if (!comic) error();
    comic->name = name;
    comic->price = price;
    comic->quantity = quantity;
    struct Cart *cart = calloc(1, 0x18);
    if (!cart) error();
    cart->comic = comic;
    return cart;
}

void addtolistCart(char *name, unsigned int price, unsigned int quantity){
    if (!listCart){
        listCart = createCart(name, price, quantity);
        return;
    }
    else{
        struct Cart *cart = listCart; 
        while (cart->next){
            cart = cart->next;
        }
        cart->next = createCart(name, price, quantity);
        cart->next->prev = cart;
        return;
    }
}

void createlistCart(){
    addtolistCart("Conan", 31000, 1000000);
    addtolistCart("Dragon Ball", 30000, 1000000);
    addtolistCart("Doraemon", 26000, 1000000);
    addtolistCart("One Piece", 23000, 1000000);
    addtolistCart("Inuyasha", 22000, 1000000);
    addtolistCart("Naruto", 23000, 1000000);
    addtolistCart("Death Note", 21000, 1000000);
    addtolistCart("Kattobi Itto", 19000, 1000000);
    addtolistCart("Kyou Kara Ore Wa", 25000, 1000000);
    addtolistCart("Ninja Rantaro", 24000, 1000000);
}

void checklistCart(){
    struct Cart *cart = listCart;
    while(cart){
        struct Cart *checkcart = cart;
        cart = cart->next;
        if(checkcart->comic->quantity == 0){
            if (checkcart->prev) checkcart->prev->next = checkcart->next;
            if (checkcart->next) checkcart->next->prev = checkcart->prev;
            if (checkcart == listCart) listCart = listCart->next;
            free(checkcart);
        }
    }
}

void showCart(struct Cart *list){
    puts("\n******************************List********************************");
    puts("*                          Name *           Price *     Quantity *");
    struct Cart *cart = list;
    while(cart){
        puts("******************************************************************");
        printf("*%30s *%12u VND *%13u *\n", cart->comic->name, cart->comic->price, cart->comic->quantity);
        cart = cart->next;
    }
    puts("******************************End*********************************");
}

struct Cart *findCart(struct Cart *list, char *name){
    struct Cart *cart = list;
    while(cart){
        if(!strcmp(cart->comic->name, name)){
            return cart;
        }
        cart = cart->next;
    }
    return 0;
}

void menu(){
    puts("\n********************Comic Store*****************");
    puts("*                                              *");
    puts("* 1 - Register                                 *");
    puts("* 2 - Show list comic                          *");
    puts("* 3 - Add comic to your cart                   *");
    puts("* 4 - Customer's management                    *");
    puts("* 5 - Order's management                       *");
    puts("* 6 - Checkout                                 *");
    puts("* 7 - Exit                                     *");
    puts("*                                              *");
    puts("************************************************");
    printf("Your choice: ");
}

int registed;
struct Order *order;

void initOrder(){
    order = calloc(1, 0x28);
    if (!order) error();
    order->customer = calloc(1, 0x10);
    if (!order->customer) error();
    order->feedback = calloc(1, 0x1c);
    if (!order->feedback) error();
    order->total = 0;
}
void addToOrder(struct Cart *cart, unsigned int quantity){
    int i;
    for (i = 0; i < order->count; i++){        
        if(!strcmp(cart->comic->name, order->listcart[i]->comic->name)){
            cart->comic->quantity -= quantity;
            order->quantity[i] += quantity;
            return;
        }
    }
    order->listcart[order->count++] = cart;
    cart->comic->quantity -= quantity;
    order->quantity[i] += quantity;
    return;
}

void Register(){
    if (!registed){
        char name[0x110];
        printf("Customer's name: ");
        memset(name, 0, 0x110);
        secure_read(name, 0x100);
        order->customer->name = newstr(name);
        memset(name, 0, 0x110);
        order->customer->money = 300000;
        order->listcart = calloc(1, 0x30);
        if (!order->listcart) error();
        order->quantity = calloc(1, 0x18);
        if (!order->quantity) error();
        registed = 1;
        puts("Done!");
    }
    else{
        puts("You are registed!");
    }
}

void addTocusCart(){
    showCart(listCart);
    printf("Name of comic: ");
    char name[0x40];
    memset(name, 0, 0x40);
    secure_read(name, 0x30);
    printf("Quantity: ");
    unsigned int quantity = read_int();
    struct Cart *cart = findCart(listCart, name);
    if (!cart){
        printf("We don't have %s\n", name);
        return;
    }
    if (quantity <= cart->comic->quantity){
        unsigned int money = cart->comic->price*quantity;
        if ((order->total + money) <= order->customer->money){
            if (order->count == 6){
                puts("Your cart is full!");
                return;
            }
            addToOrder(cart, quantity);
            order->total += money;
            puts("Done!");
        }
        else{
            puts("Not enough money!");
        }
    }
    else{
        puts("Not enough comic!");
    }
}

void showCusinfo(){
    puts("******************************Info********************************");
    printf("* Name:        * %47s *\n", order->customer->name);
    puts("******************************************************************");
    printf("* Money:       * %47u *\n", order->customer->money);
    puts("******************************************************************");
    puts("*                          Feedback                              *");
    if (order->feedback){
        int i;
        for (i = 0; i < order->feedback->count; i++){
            puts("******************************************************************");
            printf("* Content %u:   * %47s *\n", i, order->feedback->content[i]);
        }
    }
    puts("******************************End*********************************");
}

int renamed = 0;
void renameCus(){
    free(order->customer->name);
    char name[0x110];
    printf("Customer's name: ");
    memset(name, 0, 0x110);
    secure_read(name, 0x100);
    order->customer->name = newstr(name);
    memset(name, 0, 0x110);
    renamed = 1;
}

void Addfeedback(){
    puts("********************List size*******************");
    puts("*                                              *");
    puts("* 1 - Small                                    *");
    puts("* 2 - Big                                      *");
    puts("*                                              *");
    puts("************************************************");
    printf("Your choice: ");
    switch(read_int()){
        char *content;
        case 1:
            printf("Please give me feedback about some small problem that annoy u: ");
            content = malloc(0x18);
            if (!content) error();
            secure_read(content, 0x18);
            order->feedback->content[order->feedback->count++] = content;
            puts("Done!");
            break;
        case 2:
            printf("Please give me your feedback that make you feel that need to be fixed immediately: ");
            content = malloc(0x100);
            if (!content) error();
            secure_read(content, 0x100);
            order->feedback->content[order->feedback->count++] = content;
            puts("Done!");
            break;
        default:
            puts("Invalid choice!");
            break;
    }
}

void CusManager(){
    int check = 1;
    while (check){
        puts("****************Customer Management*************");
        puts("*                                              *");
        puts("* 1 - Info                                     *");
        puts("* 2 - Rename                                   *");
        puts("* 3 - Feedback                                 *");
        puts("* 4 - Back                                     *");
        puts("*                                              *");
        puts("************************************************");
        printf("Your choice: ");
        switch (read_int()){
            case 1:
                showCusinfo();
                break;
            case 2:
                if (!renamed){
                    renameCus();
                }
                else{
                    puts("You can't rename!");
                }
                break;
            case 3:
                if(order->feedback->count < 3) Addfeedback();
                else puts("Can not add more feedback!");
                break;
            case 4:
                check = 0;
                break;
            default:
                puts("Invalid choice!");
                break;
        }
    }
}

void ViewOrder(){
    puts("\n******************************Order*******************************");
    printf("* Customer                      * %30s *\n", order->customer->name);
    puts("******************************************************************");
    printf("* Total money                   * %26u VND *\n", order->total);
    puts("******************************************************************");
    puts("*                          Name *           Price *     Quantity *");
    int i;
    for (i = 0; i < 6; i++){
        if (!order->listcart[i]){
            break;
        }
        puts("******************************************************************");
        printf("*%30s *%12u VND *%13u *\n", order->listcart[i]->comic->name, order->listcart[i]->comic->price, order->quantity[i]);
    }
    puts("*******************************End********************************");
}

void popcart(){
    printf("Name of comic: ");
    char name[0x40];
    memset(name, 0, 0x40);
    secure_read(name, 0x30);
    printf("Quantity: ");
    unsigned int quantity = read_int();
    int i;
    for (i = 0; i <6; i++){
        if(!order->listcart[i]) break;
        if(!strcmp(order->listcart[i]->comic->name, name)){
            if (order->quantity[i] < quantity){
                puts("Exceed the quantity!");
                return;
            }
            order->quantity[i] -= quantity;
            order->listcart[i]->comic->quantity += quantity;
            order->total -= quantity*order->listcart[i]->comic->price;
            puts("Done!");
            return;
        }
    }
    puts("Comic isn't in your cart!");
}

void OrdManager(){
    int check = 1;
    while (check){
        puts("\n******************Order Management**************");
        puts("*                                              *");
        puts("* 1 - View Order                               *");
        puts("* 2 - Take the comic out of the cart           *");
        puts("* 3 - Back                                     *");
        puts("*                                              *");
        puts("************************************************");
        printf("Your choice: ");
        switch (read_int()){
            case 1:
                ViewOrder();
                break;
            case 2:
                popcart();
                break;
            case 3:
                check = 0;
                break;
            default:
                puts("Invalid choice!");
                break;
        }
    }
}


void Checkout(){
    puts("\n******************************Order*******************************");
    printf("* Customer                      * %30s *\n", order->customer->name);
    puts("******************************************************************");
    printf("* Total money                   * %26u VND *\n", order->total);
    puts("******************************************************************");
    printf("* Customer's money before       * %26u VND *\n", order->customer->money);
    puts("******************************************************************");
    printf("* Customer's money after        * %26u VND *\n", order->customer->money - order->total);
    puts("******************************************************************");
    puts("*                          Name *           Price *     Quantity *");
    int i;
    for (i = 0; i < 6; i++){
        if (!order->listcart[i]){
            break;
        }
        puts("******************************************************************");
        printf("*%30s *%12d VND *%13d *\n", order->listcart[i]->comic->name, order->listcart[i]->comic->price, order->quantity[i]);
    }
    puts("*******************************End********************************");
    order->customer->money -= order->total;
    for (i = 0; i < 6; i++){
        if(!order->listcart[i]) break;
        if(!order->listcart[i]->comic) break;
        if(!order->listcart[i]->comic->quantity){
            if(order->listcart[i] == listCart) listCart = order->listcart[i]->next;
            if (order->listcart[i]->prev) order->listcart[i]->prev->next = order->listcart[i]->next;
            if (order->listcart[i]->next) order->listcart[i]->next->prev = order->listcart[i]->prev;
            free(order->listcart[i]->comic);
            free(order->listcart[i]);
        }
    }
    free(order->listcart);
    order->listcart = 0;
    order->count = 0;
    printf("Do you want to buy more?(1:yes or 0:no) ");
    int choice = read_int();
    if (choice == 1){
        order->total = 0;
        order->listcart = malloc(0x30);
        memset(order->quantity, 0, 0x18);
        return;
    }
    else {
        printf("Thank you!\n See you again!\n");
        exit(0);
    }
}

void init(){
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);
    signal(14, timeout);
    alarm(60);
    puts("\n**************************Welcome to Comic Sotre**************************");
    puts("*                                                                        *");
    puts("*************************Challenge Created By CNV*************************");
    puts("*   Team: AceBear                                                        *");
    puts("*   My blog: https://chung96vn.blogspot.com/                             *");
    puts("**************************************************************************");
    createlistCart();
    initOrder();
}


int main(){
    init();
    while (1){
        menu();
        switch (read_int()){
            case 1:
                Register();
                break;
            case 2:
                if (registed) showCart(listCart);
                else puts("Please register!");
                break;
            case 3:
                if (registed) addTocusCart();
                else puts("Please register!");
                break;
            case 4:
                if (registed) CusManager();
                else puts("Please register!");
                break;
            case 5:
                if (registed) OrdManager();
                else puts("Please register!");
                break;
            case 6:
                if (registed) Checkout();
                else puts("Please register!");
                break;
            case 7:
                exit(0);
                break;
            default:
                puts("Invalid choice!");
        }
    }
}

// gcc -fPIE -pie -s -Wl,-z,relro,-z,now comic_store.c -o comic_store