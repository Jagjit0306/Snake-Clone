#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<math.h>

int playground;

typedef struct segment {
    int loc[2];
} segment;
segment* Part[100];

int size = 1;
int sizepg;
int dir = 1;
bool collision = false;
bool eaten = true;
int food[] = {0,0};
bool increase = false;
bool border;
COORD coord;

int powInt(int x, int y) {return pow(x,y);} // modified version of pow() which return an integer only

void Leaderboard(int sizel, bool border, int score){ // Manages the entire game ending shenanigans
    //chronology -> borderless - 0 1 2    bordered - 0 1 2

    printf("Your score : \033[1;32m%d\n\033[1;0m",score);

    char newname[20]; 
    bool write = false; int place=0;
    FILE *f;
    f = fopen("Snake.txt","r");
    for(int i = 9; i >= 0;i--){ // determines which rank you take in the leaderboard based on your score
        fseek(f, (240*sizel) + (i*24) + ((!border)?0:720),SEEK_SET);
        int hscore = 0; char c;
        for(int i = 0; i < 3; i++) { //get that score
            fscanf(f,"%c",&c);
            hscore += powInt(10,2-i) * (c - 30-18);
        }
        if(hscore < score){
            place = i; write = true;
        }
    }
    fclose(f);
    
    int scores[10]; char names[10][25];
    int deletion = 9; // default value for the rank of the person eliminated from the leaderboard

    if(write){ // if you have scored more than the top 10 ranks on the leaderboard
        f = fopen("Snake.txt","r+");

        printf("\nYOU MADE IT INTO THE TOP 10 !!\n\n");
        printf("\nEnter your name : "); gets(newname);
        for(int i = strlen(newname); i<20;i++) newname[i] = '#'; newname[20] = '\0'; // change names into the 20 character string
        // fill empty spaces with #
        for(int x = 0; x < 10; x++){
            fseek(f, (sizel*240) + (x*24) + ((!border)?0:720),SEEK_SET);
            char c;
            scores[x] = 0;
            for(int i = 0; i < 3; i++) {
                fscanf(f,"%c",&c);
                scores[x] += powInt(10,2-i) * (c - 30-18);
            }
            for(int j = 0; j < 20; j++) names[x][j] = fgetc(f);
            newname[20] = '\0'; names[x][20] = '\0';
            if(!strcmp(names[x],newname)){ // if both the names match which means u have to compare with your own score
                if(x < place || (x == place && scores[x] >= score)) write = false; // if you dont beat your highscore
                else if (x > place || (x == place && scores[x] <score)) { // when you beat your own highscore
                    write = true; // write your new rank
                    deletion = x; // delete your old rank
                }
            }
            if (!write) break;            
        }
    }
    if(write){ // this screen is shown when you beat your PR
        {printf("                                   \033[1;36m.''.\033[1;0m\n");
        printf("       \033[1;35m.''      \033[1;0m.         \033[1;32m*''*    \033[1;36m:_\\/_:     \033[1;31m.\033[1;0m\n");
        printf("      \033[1;35m:_\\/_:   \033[1;0m_\\(/_  \033[1;33m.:.\033[1;32m*_\\/_*   \033[1;36m: /\\ :  \033[1;31m.'.:.'.\033[1;0m\n");
        printf("  \033[1;33m.''.\033[1;35m: /\\ :    \033[1;0m/)\\   \033[1;33m':'\033[1;32m* /\\ *  \033[1;35m: \033[1;36m'..'.  \033[1;31m-=:o:=-\033[1;0m\n");
        printf(" \033[1;33m:_\\/_:'\033[1;34m.:::.    \033[1;0m' \033[1;31m*''*    \033[1;32m* '\033[1;35m.\\'/.'\033[1;0m_\\(/_ \033[1;31m'.':'.\033[1;0m\n");
        printf(" \033[1;33m: /\\ : \033[1;34m:::::     \033[1;31m*_\\/_*     \033[1;35m-= o =- \033[1;0m/)\\     \033[1;31m'  *\033[1;0m\n");
        printf("  \033[1;33m'..'  \033[1;34m':::'     \033[1;31m* /\\ *     \033[1;35m.'/\\.\\'.  \033[1;0m' .\033[1;0m\n");
        printf("      \033[1;0m*            \033[1;31m*..*    \033[1;0m\n");

        Beep (262,100);Sleep(125);
        Beep (330,100);Sleep(125);
        Beep (392,100);Sleep(125);
        Beep (523,100);Sleep(125);
        Beep (660,100);Sleep(125);
        Beep (784,100);Sleep(575);
        Beep (660,100);Sleep(575);
        Beep (207,100);Sleep(125);
        Beep (262,100);Sleep(125);
        Beep (311,100);Sleep(125);
        Beep (415,100);Sleep(125);
        Beep (523,100);Sleep(125);
        Beep (622,100);Sleep(125);
        Beep (830,100);Sleep(575);
        Beep (622,100);Sleep(575);
        Beep (233,100);Sleep(125);
        Beep (294,100);Sleep(125);
        Beep (349,100);Sleep(125);
        Beep (466,100);Sleep(125);
        Beep (587,100);Sleep(125);
        Beep (698,100);Sleep(125);
        Beep (932,100);Sleep(575);
        Beep (932,100);Sleep(125);
        Beep (932,100);Sleep(125);
        Beep (932,100);Sleep(125);
        Beep (1046,675);}

        f = fopen("Snake.txt","r+");

        for(int i = 0; i < place; i++){
            if(i != deletion){ // save the ranks of the people above you in the leaderboard
                fseek(f,(240*sizel) + (24*i) + ((!border)?0:720),SEEK_SET);
                fprintf(f,"%d%d%d",((scores[i])/100)%10,((scores[i])/10)%10,(scores[i])%10);
                fprintf(f,"%s",names[i]);
            }
        }
        // save your rank
        fseek(f,(240*sizel) + (24*place) + ((!border)?0:720),SEEK_SET);
        fprintf(f,"%d%d%d",((score)/100)%10,((score)/10)%10,(score)%10);
        fprintf(f,"%s",newname);

        for(int i = place; i < 10; i++){ // save the rank of the people below you in the leaderboard
            if(i != deletion){ // ignore the printing of the rank which is eliminated
                fseek(f,(240*sizel) + (24*(i+1 - ((i>deletion)?1:0))) + ((!border)?0:720),SEEK_SET);
                fprintf(f,"%d%d%d",((scores[i])/100)%10,((scores[i])/10)%10,(scores[i])%10); 
                fprintf(f,"%s",names[i]);
            }
        }
        fclose(f);
    }
    else { // this screen is displayed if you dont make it into the top 10, or when you dont beat your own highscore
        printf("You died........\n\n\033[1;34m:(\n\n\033[1;31m</3\033[1;0m\n\n");
        printf("             ___          \n");
        printf("            /   \\\\        \n");
        printf("       /\\\\ | \033[1;31m. .\033[1;0m \\\\       \n");
        printf("     ////\\\\|     ||       \n");
        printf("   ////   \\\\ ___//\\       \n");
        printf("  ///      \\\\      \\      \n");
        printf(" ///       |\\\\      |     \n");
        printf("//         | \\\\  \\   \\    \n");
        printf("/          |  \\\\  \\   \\   \n");
        printf("           |   \\\\ /   /   \n");
        printf("           |    \\/   /    \n");
        printf("           |     \\/|     \n");
        printf("           |      \\\\|     \n");
        printf("           |       \\\\     \n");
        printf("           |        |     \n");
        printf("           |_________\\    \n");
        Beep (311,300);Sleep(300);
        Beep (296,300);Sleep(300);
        Beep (262,300);Sleep(1300);
    }
    printf("\n\033[1;33mLEADERBOARD\033[1;0m\n\n");
    f = fopen("Snake.txt","r");
    for(int i = 0; i < 10; i++){
        fseek(f, (sizel*240) + (i*24) + ((!border)?0:720),SEEK_SET);
        printf("-> ");
        int hscore = 0; char c;
        for(int i = 0; i < 3; i++) {
            fscanf(f,"%c",&c);
            hscore += powInt(10,2-i) * (c - 30-18);
        }
        char name[20];
        for(int i = 0; i < 20; i++) fscanf(f, "%c",&name[i]);
        for(int i = 0; i < 20; i++) printf("%c",(name[i]!='#')?name[i]:' ');
        printf(" \033[1;34m%d\033[1;0m\n",hscore);
    }
    fclose(f);
    if(write) printf("\n\nCongratulations and Good Luck with your endeavours.....\n\n");
    else printf("\n\nbetter luck next time.....\n\n");
}

void Canvas(){ // Draws an empty maze
    system("cls");
    for (int i = 0; i < playground + 2; i++) printf("\033[1;34m%c%c\033[1;0m",-37,-37); printf("\n");
    for(int i = 0; i < playground; i++){
        printf("\033[1;34m%c%c\033[1;0m",-37,-37);
        for(int j = 0; j < playground; j++) printf("__");
        printf("\033[1;34m%c%c\033[1;0m\n",-37,-37);
    }
    for (int i = 0; i < playground + 2; i++) printf("\033[1;34m%c%c\033[1;0m",-37,-37); printf("\n");
}

char input(){ // improves the prebuilt _getch() function and makes it more efficient when taking input from arrow keys
    char j = _getch();
    if (j == -32) j = _getch();
    return j;
}

void Setup(){ // The pre-game screen that lets you configure your game settings and also saves them for when you play next time
    FILE *f;
    f = fopen("Snake.txt","r");  // Access the saved information for saved preferences
    fseek(f, -2, SEEK_END);
    char c;
    c = fgetc(f); sizepg = c - 48; 
    c = fgetc(f); border = c - 48; 
    fclose(f);

    bool pos = true; //true when editing size, and false when editing borders
    system("cls");
    printf("CONFIGURE YOUR GAME.....\n\n\nSize of playground   :        \033[1;33m%c\033[1;0m\n\nPass-through borders : %s",!sizepg?'S':(sizepg == 1?'M':'L'), border?"\033[1;31mDisabled\033[1;0m":"\033[1;32m Enabled\033[1;0m");
    printf("\n\nUse \033[1;33mArrow keys\033[1;0m to change settings.....\n");
    printf("\nPress \033[1;33mSPACEBAR\033[1;0m to Play SNAKE......");
    char ch = input();

    while(ch != 32){
        system("cls");
        printf("CONFIGURE YOUR GAME.....\n\n\nSize of playground   :        \033[1;33m%c\033[1;0m\n\nPass-through borders : %s",!sizepg?'S':(sizepg == 1?'M':'L'), border?"\033[1;31mDisabled\033[1;0m":"\033[1;32m Enabled\033[1;0m");
        printf("\n\nUse \033[1;33mArrow keys\033[1;0m to change settings.....\n");
        printf("\nPress \033[1;33mSPACEBAR\033[1;0m to Play SNAKE......");
        
        if(!pos){ // Change the size of the maze
            coord.X = 32; coord.Y = 5;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("\b");
            coord.X = 23+2+7; coord.Y = 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("\033[1;31m%c\033[1;0m",-37);
            ch = input();
            if(ch == 'H' || ch == 'P') pos = true;
            else if(ch == 'M') sizepg = sizepg==2?0:++sizepg;
            else if(ch == 'K') sizepg = !sizepg?2:--sizepg;
        }
        else if(pos){ // Toggle pass-through borders
            coord.X = 23+2+7; coord.Y = 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("\b");
            coord.X = 32; coord.Y = 5;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            printf("\033[1;31m%c\033[1;0m",-37);
            ch = input();
            if(ch == 'H' || ch == 'P') pos = false;
            else if(ch == 'M' || ch == 'K') border = border?false:true;
        }
    }
    f = fopen("Snake.txt","r+");
    fseek(f, -2, SEEK_END);
    fprintf(f,"%d%d",sizepg,border); // Save the updated preferences
    fclose(f);
    playground = !sizepg?10:(sizepg==1?20:30);

}

void Repaint(int x, int y, int type){ // Redraw individual blocks instead of refreshing entire screen
    //1 to delete snake and apple and 2 for to be apple and 3 for to be snake
    coord.X = 2*x + 2; coord.Y = y + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //element deleted
    if(type == 3) printf("\033[1;33m%c%c\033[1;0m",-37,-37);
    else if (type == 1) printf("__");
    else if (type == 2) printf("\033[1;31m[]\033[1;0m");
    coord.X = 0 ; coord.Y = playground+5; // Set cursor location to below the maze so that cursor is out of the way
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsASnake(int x, int y){ // check whether a block is occupied by Snake
    for (int i = 0; i < size; i++) if (x == Part[i]->loc[0] && y == Part[i]->loc[1]) return true;
    return false;
}

void kitchen(){ // determine the new random location of food and make it spawn
    int x,y;
    do{
        x = rand()%playground;
        y = rand()%playground;} while(IsASnake(x,y));
    food[0] = x; food[1] = y;
    eaten = false;
    Repaint(food[0],food[1],2);
}

char get_char_with_timeout(int timeout_ms) { // Wait for input and if no input is given in specified time, return null
    clock_t start_time = clock();
    char ch = 0;

    while ((clock() - start_time) < (timeout_ms * CLOCKS_PER_SEC / 1000)) {
        if (_kbhit()) {
            ch = input();
            break;
        }
        Sleep(10);
    }
    return ch;
}

bool IsFood(int x, int y){ // Checks whether the particular block space is occupied by Food
    if(x == food[0] && y == food[1]) return true;
    return false;
}

void Generator(bool var){ //generate a new block and assign it the location of its parent block
    if (var) { 
        Part[size] = (segment*)malloc(sizeof(segment));
        Part[size]->loc[0] = Part[size - 1]->loc[0]; Part[size]->loc[1] = Part[size - 1]->loc[1];
        size++;
    }
}

void Mover(int dir){
    Generator(increase); // generate a new block by dynamic memory allocation if food is consumed in the previous loop iteration
    int loctemp[] = {0,0};
    for (int i = 0; i < size;i++ ){
        if (i+1 < size){
        loctemp[0] = Part[i+1]->loc[0]; loctemp[1] = Part[i+1]->loc[1];
        Part[i+1]->loc[0] = Part[i]->loc[0]; Part[i+1]->loc[1] = Part[i]->loc[1];
        }
        if(!i){
            int temp = 0;
            if(dir == 1){
                if(Part[i]->loc[1]-1 == -1) {
                    if(!border) temp = playground - 1;
                    else {collision = true; break;}
                    }
                else temp = Part[i]->loc[1]-1;
                if(IsASnake(Part[i]->loc[0],temp)) collision = true;
                Part[i]->loc[1] = temp;
            }
            else if (dir == 3){
                if(Part[i]->loc[1]+1 == playground) {
                    if(!border) temp = 0;
                    else {collision = true; break;}
                }
                else temp = Part[i]->loc[1]+1;
                if(IsASnake(Part[i]->loc[0],temp)) collision = true;
                Part[i]->loc[1] = temp;
            }
            if (dir == 2){
                if(Part[i]->loc[0] +1 == playground) {
                    if(!border) temp = 0;
                    else {collision = true; break;}
                }
                else temp = Part[i]->loc[0]+1;
                if(IsASnake(temp, Part[i]->loc[1])) collision = true;
                Part[i]->loc[0] = temp;
            }
            else if (dir == 4){
                if(Part[i]->loc[0]-1 == -1) {
                    if(!border) temp = playground - 1;
                    else {collision = true; break;}
                }
                else temp = Part[i]->loc[0]-1;
                if(IsASnake(temp,Part[i]->loc[1])) collision = true;
                Part[i]->loc[0] = temp;
            }
            
        }
        else {
            if(i+1 < size){
            Part[i]->loc[0] = loctemp[0];
            Part[i]->loc[1] = loctemp[1];
            loctemp[0] = 0; loctemp[1] = 0;}
        }
        
    }
    for(int i = 0; i < size; i++) Repaint(Part[i]->loc[0],Part[i]->loc[1],3); // Redraw entire Snake

    increase = false;
    if(Part[0]->loc[0] == food[0] && Part[0]->loc[1] == food[1]){ //if the location of the head coincides with food
        Beep (196,100);Sleep(150);
        Beep (330,100);Sleep(150);
        Beep (392,100);Sleep(150);
        Beep (440,100);
        increase = true;
        eaten = true;

        coord.X = 0 ; coord.Y = playground+3;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("Score : \033[1;32m%d\033[1;0m", size); //update the score every time food is consumed
        coord.X = 0 ; coord.Y = playground+5;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
}

int main(){
    srand(time(0));
    Part[0] = (segment*)malloc(sizeof(segment));
    system("cls");
    {printf("Welcome to SNAKE\n\n\n"); Sleep(500);
    printf("by \033[1;34mJagjit0306 :)\033[1;0m\n23104040\n\n\n"); Sleep(800);
    printf("\033[1;32m           \033[1;0m/^\\/^\n\033[1;32m"
           "         _\033[1;0m|\033[1;32m__\033[1;0m|  \033[1;31mO\033[1;32m|\n"
           "\033[1;31m\\/\033[1;32m     /~     \\_/ \\\n"
           " \033[1;31m\\____\033[1;32m|__________/  \\\n"
           "        \\_______      \\\n"
           "                `\\     \\                 \\\n"
           "                  |     |                  \\\n"
           "                 /      /                    \\\n"
           "                /     /                       \\\\\n"
           "              /      /                         \\ \\\n"
           "             /     /                            \\  \\\n"
           "           /     /             _----_            \\   \\\n"
           "          /     /           _-~      ~-_         |   |\n"
           "         (      (        _-~    _--_    ~-_     _/   |\n"
           "          \\      ~-____-~    _-~    ~-_    ~-_-~    /\n"
           "            ~-_           _-~          ~-_       _-~\n"
           "               ~--______-~                ~-___-~\033[1;0m\n");
    Beep (932,100);Sleep(125);Beep (932,100);Sleep(125);Beep (932,100);Sleep(125);Beep (1046,675);Sleep(1000);
    printf("\n\nUse \033[1;33mArrow keys\033[1;0m to control the SNAKE\n");
    printf("Eat the \033[1;31mAPPLES []\033[1;0m to gain size\nDO NOT BITE YOURSELF !!\n");
    printf("Press \033[1;33mSPACEBAR\033[1;0m anytime to pause your game.....\n"); 
    Sleep(200);
    printf("\n\n\nPress Any Key to continue...");} 
    _getch();

    Setup();
    Part[0]->loc[0] = playground/2; Part[0]->loc[1] = playground/2;
    Canvas(); Repaint(Part[0]->loc[0],Part[0]->loc[1],3);
    while(!collision){
        char ch;
        if(eaten) kitchen();
        Sleep(80);
    
        ch = get_char_with_timeout((500*exp(-0.06437751649*(size - 1)>2)?500*exp(-0.06437751649*(size-1)):2));
        if (ch == 32){
            if(ch == 32){
                printf("Press \033[1;33mSPACEBAR\033[1;0m to continue your game....."); 
                
            }
            ch = 0 ;
            while (ch != 32){
                ch = input();
                if (ch == 32)
                    printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                                         "); 
            }
        }
        if(dir%2 == 0){
            if(ch == 'H') dir = 1;
            else if(ch == 'P') dir = 3;
        }
        else{
            if(ch == 'M') dir = 2;
            else if(ch == 'K') dir = 4;
        }

        for(int i = 0; i < size; i++) Repaint(Part[i]->loc[0],Part[i]->loc[1],1); // delete entire snake

        Mover(dir);
        ch = 0;
    }
    Beep(900,1200);Sleep(1500);
    system("cls");
    Sleep(250);

    Leaderboard(sizepg, border, size-1);
    
    _getch(); _getch();
}