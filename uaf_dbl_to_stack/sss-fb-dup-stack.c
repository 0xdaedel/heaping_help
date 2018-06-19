#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int g_lasterror=0;
int P_INV_MAX;
char *BOLD_RED = "\033[1;31m";
char *BOLD_GREEN = "\033[1;32m";
char *BOLD_YELLOW = "\033[1;33m";
char *BOLD_BLUE = "\033[1;34m";
char *BOLD_PURPLE = "\033[1;35m";
char *BOLD_CYAN = "\033[1;36m";
char *BOLD_GRAY = "\033[1;37m";
char *BOLD_WHITE = "\033[1;37m";
char *BOLD_DEFAULT = "\033[1;39m";
char *BG_WHITE = "\033[1;40m";
char *BG_RED = "\033[1;41m";
char *BG_GREEN= "\033[1;42m";
char *BG_YELLOW = "\033[1;43m";
char *BG_BLUE = "\033[1;44m";
char *BG_PURPLE = "\033[1;45m";
char *BG_CYAN = "\033[1;46m";
char *BG_GRAY = "\033[1;47m";
char *BG_BLACK = "\033[1;48m";
char *BG_DEFAULT = "\033[1;49m";
char *BOLD_BLACK = "\033[1;90m";

char *PURCHASE_PROMPT = "Enter name to purchase:> ";

int FIRST_HURDLE = 9945326;
int SECOND_HURDLE = 2675874;
int THIRD_HURDLE = 334588;
int crash_track = 0;
int crash_loop = 0;

int BUF_SIZE = 48;

char *NONE = "\033[0m\033[49m";

unsigned long long *items[50];
int item_cnt = 0;
int bank = 2000;
unsigned long long secret = 0;
unsigned long long my_temp_store, *head_of_free;
//char ibuf[24];
void shell(){
    execve("/bin//sh",NULL,NULL);
    //system();
}
//read a line
int readLine( int fd, char *buffer, size_t maxlen )
{
    int pos;
    for ( pos = 1; pos < maxlen; pos++ )
    {
        int val = fgetc(stdin);
        if (val == EOF){
            break;
        }

        if (val == 0xA || val == 0xC) {
            break;
        }
        //printf("%c (%x)\n", val, val);
        buffer[0] = val;
        buffer++;
    }

    buffer[0] = '\0';
    //printf("exiting\n");
    return pos;
}

int char_to_num(char buf[]){
    char *endptr;
    long num = strtol(buf, &endptr, 10);

    if (endptr == buf){
        return -1;
    }
    if (num > INT_MIN && num < INT_MAX) {
        return num;
    }
    return -1;
}

int read_single_num(char *prompt){

    char buf[5];

    printf("%s",prompt);

    while (1){

        readLine(stdin, buf, 4);

        int num = char_to_num(buf);

        if (num > -1){
            return num;
        } else{
            printf("%s",prompt);
        }

    }
    return -1;

}

void hello(){

    printf("The man behind the counter with blueish square glasses greets you with a warm 'HELLO'\n");
    if (secret != 0){
        printf("Since you're nice enough to say hello, I'll give you a hint try using %s%s %p %s to beat me!\n", BG_BLUE, BOLD_YELLOW, secret, NONE);
    }
    printf("How can I help you?\n");
}

void do_it(char *gbuf){
    unsigned long long the_size = BUF_SIZE+16;

    secret = (unsigned long long) &the_size;
    my_temp_store = malloc(BUF_SIZE);
    if (head_of_free != 0){
        head_of_free = *head_of_free + 0x10;
    }

    printf("\n%sHead of Free List = 0x%llx %s\n", BOLD_YELLOW, head_of_free, NONE );
    items[item_cnt] = my_temp_store;

    memcpy(items[item_cnt], gbuf, BUF_SIZE);
    item_cnt++;

}

void to_hex(char c, char res[]){
    int i =0;
    sprintf(res,"%02x",c);
}

void purchase(){
    char gbuf[48];
    int ret = 0;

    if (item_cnt >= 50){
        printf("%sI'm sorry you cannot buy any more names, you're full.%s\n", BOLD_RED, NONE);
    }

    printf(PURCHASE_PROMPT);
    ret = readLine( stdin, gbuf, sizeof(gbuf) );

    // arg of buf, messed up, using global
    do_it(gbuf);

    char hex[strlen(gbuf)*3];

    printf("\n");
    printf("Purchase # %d\n", item_cnt);
    printf("You purchased the name, '%s'. Which is ", gbuf);
    for (int i =0; i < strlen(gbuf); i++){
        to_hex(gbuf[i], hex);
        printf("%s", hex);
        if (i > 0 && (((i+1) % 8) == 0)){
            printf(" ");
        }
    }
    printf(" in Shellphisan\n");

}

void print_items(){
    int i = 0;

    for (i=0; i < item_cnt;i++){
        if (items[i] != 0){
            if (*items[i] == 0) {
                printf("[%d] %-48s\t(heap loc=%s0x%llx%s, reuse loc= %sTOP%s ) \n", (i+1), items[i], BOLD_PURPLE, items[i], NONE , BOLD_GREEN, NONE);
            } else {
                if (strlen(items[i]) < 45) {
                    printf("[%d] %-48s\t(heap loc=%s0x%llx%s, %sreuse loc=0x%llx%s) \n", (i+1), items[i], BOLD_PURPLE, items[i], NONE, BOLD_GREEN, *items[i], NONE );
                } else{
                    printf("[%d] %-48s\t(heap loc=%s0x%llx%s, reuse loc= ----- ) \n", (i+1), items[i], BOLD_PURPLE, items[i], NONE );
                }
            }

        }
    }
    printf("--------- End of List ----------");

}

void sell(){

    print_items();
    int var_num = read_single_num("Enter number of name to sell :>");
    head_of_free = items[var_num-1];
    printf("\n%sHead of Free List = 0x%llx %s\n", BOLD_YELLOW, head_of_free, NONE );
    free(items[var_num-1]);
}


int banner(){
    printf("%s\n", BOLD_WHITE);
    printf("      \033[1;34m___\033[0m\033[49m           \033[1;34m___\033[0m\033[49m           \033[1;34m___\033[0m\033[49m           \033[1;34m___\033[0m\033[49m           \033[1;34m___\033[0m\033[49m           \033[1;34m___\033[0m\033[49m \n");
    printf("     \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m         \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m         \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m         \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m         \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m         \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m \n");
    printf("    \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m       \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m       \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m       \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m       \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m        \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m \n");
    printf("   \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m     \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m     \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m     \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m     \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m        \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m  \n");
    printf("  \033[1;34m_\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m~\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m   \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m\033[1;34m~\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m   \033[1;34m/\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m\033[1;34m~\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m   \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m   \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m  \033[1;34m\033[1;34m___   _\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m~\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m \n");
    printf(" \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m|\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m|\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m \n");
    printf(" \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m/_\033[1;34m|\033[0m\033[49m::\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m\\\033[0m\033[49m \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m\n");
    printf("  \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m        \033[1;34m\\\033[0m\033[49m::\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m     \033[1;34m|\033[0m\033[49m:\033[1;34m|\033[0m\033[49m::\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m   \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m   \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m  \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m   \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m\\\033[0m\033[49m  \n");
    printf("   \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m        \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m      \033[1;34m|\033[0m\033[49m:\033[1;34m|\033[0m\033[49m\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m     \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m     \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m     \033[1;34m\\\033[0m\033[49m:\033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \n");
    printf("    \033[1;34m\\\033[0m\033[49m::\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m        \033[1;34m/\033[0m\033[49m:\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m       \033[1;34m|\033[0m\033[49m:\033[1;34m|\033[0m\033[49m  \033[1;34m|\033[0m\033[49m        \033[1;34m\\\033[0m\033[49m::\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m       \033[1;34m\\\033[0m\033[49m::\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m       \033[1;34m\\\033[0m\033[49m::\033[1;34m/\033[0m\033[49m  \033[1;34m/\033[0m\033[49m \n");
    printf("     \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m         \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m         \033[1;34m\\\033[0m\033[49m\033[1;34m|\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m|\033[0m\033[49m         \033[1;34m--\033[0m\033[49m            \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m         \033[1;34m\\\033[0m\033[49m\033[1;34m/\033[0m\033[49m\033[1;34m__\033[0m\033[49m\033[1;34m/\033[0m\033[49m \n");
    printf("%s",NONE);

}

int main(void)
{
    char buf[20];
    int ret = 0;
    // Print banner
    banner();

    char *cmds = "[buy, sell, list, hello, quit] ";
    printf( "Welcome to Sardus's Swap Shop.\n\n%s\n", cmds );
    printf("> ");
    ret = readLine( stdin, buf, sizeof(buf) );

    printf("%s\n", buf);

    while (ret != -1) {

        // crash is currently, "buy, hello, hello" from main menu
        if (strcmp(buf,"hello") == 0) {
            hello();
        } else if (strcmp(buf,"list") == 0) {
            print_items();
        } else if (strcmp(buf,"buy") == 0) {
            purchase();
        } else if (strcmp(buf,"sell") == 0) {
            sell();
        } else if (strcmp(buf,"quit") == 0) {
            printf("Goodbye!\n");
            return 0;
        } else {
            printf("Invalid command.");
        }

        printf( "\n%s\n", cmds );
        printf("> ");
        ret = readLine( stdin, buf, sizeof(buf) );
        printf("%s\n", buf);
    }


    printf("%s",buf);


    // Exit
    printf( "Goodbye\n" );

    return 0;

}