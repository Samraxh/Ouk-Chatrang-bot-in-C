//Please use font Consolata for best experience
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

typedef char (*Table)[8][24];
typedef char BOARD[8][8][24];
typedef char Data[100][8][8][24];
typedef char **Arr;

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define PURPLE  "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#define ANG    42069
#define TREY     100
#define NEANG    210
#define KOUL     320
#define SES      330
#define TUUK     550

typedef struct Piece{
    int value;
    int Table[63];
} Piece;
Piece trey_w, neang_w, koul_w, ses_w, tuuk_w, ang_w, treyB, koul_b, koul_b, ses_b, tuuk_b, ang_b;

#ifdef _WIN32 
    #include <windows.h>
    //#include <fileapi.h> // if on windows get "compilation terminated", then commment out this line For modern computer include windows.h is enough
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define MKDIR(s) CreateDirectoryA((s), NULL)
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #define GetCurrentDir getcwd
    #define MKDIR(s) mkdir((s), 0777)
#endif
char black[13][24] = {GREEN"\u265C "RESET, GREEN"\u265E "RESET, GREEN"\u265D "RESET, GREEN"\u265B "RESET, GREEN"\u265A "RESET, GREEN"\u265F "RESET,
                    CYAN"\u265C "RESET, CYAN"\u265E "RESET, CYAN"\u265D "RESET, CYAN"\u265B "RESET, CYAN"\u265A "RESET, CYAN"\u265F "RESET, PURPLE"\u265A "RESET,
                    };
char white[13][24] = {"\u265C ", "\u265E ", "\u265D ", "\u265A ", "\u265B ", "\u265F ", RED"\u265A "RESET,
                    YELLOW"\u265C "RESET, YELLOW"\u265E "RESET, YELLOW"\u265D "RESET, YELLOW"\u265B "RESET, YELLOW"\u265A "RESET, YELLOW"\u265F "RESET
                    };
char all[26][24] = {"\u265C ", "\u265E ", "\u265D ", "\u265A ", "\u265B ", "\u265F ", RED"\u265A "RESET, PURPLE"\u265A "RESET,
                GREEN"\u265C "RESET, GREEN"\u265E "RESET, GREEN"\u265D "RESET, GREEN"\u265B "RESET, GREEN"\u265A "RESET, GREEN"\u265F "RESET,
                CYAN"\u265C "RESET, CYAN"\u265E "RESET, CYAN"\u265D "RESET, CYAN"\u265B "RESET, CYAN"\u265A "RESET, CYAN"\u265F "RESET,
                YELLOW"\u265C "RESET, YELLOW"\u265E "RESET, YELLOW"\u265D "RESET, YELLOW"\u265B "RESET, YELLOW"\u265A "RESET, YELLOW"\u265F "RESET
                };
BOARD board = {
        {"\u265C ", "\u265E ", "\u265D ", "\u265A ", "\u265B ", "\u265D ", "\u265E ", "\u265C "},
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
        {"\u265F ", "\u265F ", "\u265F ", "\u265F ", "\u265F ", "\u265F ", "\u265F ", "\u265F "},
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
        {GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET, GREEN"\u265F "RESET},
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "},
        {GREEN"\u265C "RESET, GREEN"\u265E "RESET, GREEN"\u265D "RESET, GREEN"\u265B "RESET, GREEN"\u265A "RESET, GREEN"\u265D "RESET, GREEN"\u265E "RESET, GREEN"\u265C "RESET}
        };
char from[3], to[3];
char move[5] = "NULL";
char rep_move1[5] = "NULL";
char rep_move2[5] = "NULL";

enum pos {
    a8=55, b8, c8, d8, e8, f8, g8, h8,
    a7=47, b7, c7, d7, e7, f7, g7, h7,
    a6=39, b6, c6, d6, e6, f6, g6, h6,
    a5=31, b5, c5, d5, e5, f5, g5, h5,
    a4=23, b4, c4, d4, e4, f4, g4, h4,
    a3=15, b3, c3, d3, e3, f3, g3, h3,
    a2=7, b2, c2, d2, e2, f2, g2, h2,
    a1=0, b1, c1, d1, e1, f1, g1, h1};

int mvv_lva[][6] = {
{45, 46, 52, 53, 54, 55},
{35, 36, 42, 43, 44, 45},
{25, 26, 32, 33, 34, 35},
{15, 16, 22, 23, 24, 25},
{ 5,  6,  7, 12, 13, 14},
};
int trey_w_Table[] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
-11,  6,  7,  3,  3,  7,  6,-11,
-18, -2, 19, 24, 24, 19, -2,-18,
-17, -9, 20, 35, 35, 20, -9,-17,
 -6,  5,  3, 21, 21,  3,  5, -6,
 -6, -8, -6, -2, -2, -6, -8, -6,
 -4, 20, -8, -4, -4, -8, 20, -4 };

 int trey_b_Table[] = {
 -4, 20, -8, -4, -4, -8, 20, -4,
 -6, -8, -6, -2, -2, -6, -8, -6,
 -6,  5,  3, 21, 21,  3,  5, -6,
-17, -9, 20, 35, 35, 20, -9,-17,
-18, -2, 19, 24, 24, 19, -2,-18,
-11,  6,  7,  3,  3,  7,  6,-11,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0 };

int neang_w_Table[] = {
-25,-16,-16, -9, -9,-16,-16,-25,
-21, -8, -3,  0,  0, -3, -8,-21,
-21, -9, -4,  2,  2, -4, -9,-21,
-22, -6, -1,  2,  2, -1, -6,-22,
-22, -7,  0,  1,  1,  0, -7,-22,
-21, -7,  0,  2,  2,  0, -7,-21,
-12,  4,  8, 12, 12,  8,  4,-12,
-23,-15,-11, -5, -5,-11,-15,-23 };

int neang_b_Table[] = {
-23,-15,-11, -5, -5,-11,-15,-23, 
-12,  4,  8, 12, 12,  8,  4,-12,
-21, -7,  0,  2,  2,  0, -7,-21,
-22, -7,  0,  1,  1,  0, -7,-22,
-22, -6, -1,  2,  2, -1, -6,-22,
-21, -9, -4,  2,  2, -4, -9,-21,
-21, -8, -3,  0,  0, -3, -8,-21,
-25,-16,-16, -9, -9,-16,-16,-25 };

int koul_w_Table[] = {
-25,-16,-16, -9,-16,-16,-9,-25,
-21, -8, -3,  0, -8, -3, 0,-21,
-21, -9, -4,  2, -9, -4, 2,-21,
-22, -6, -1,  2, -6, -1, 2,-22,
-22, -7,  0,  1, -7,  0, 1,-22,
-21, -7,  0,  2, -7,  0, 2,-21,
-12,  4,  8, 12,  4,  8,12,-12,
-23,-15,-11, -5,-15,-11,-5,-23 };

int koul_b_Table[] = {
-23,-15,-11, -5,-15,-11,-5,-23,
-12,  4,  8, 12,  4,  8,12,-12,
-21, -7,  0,  2, -7,  0, 2,-21,
-22, -7,  0,  1, -7,  0, 1,-22,
-22, -6, -1,  2, -6, -1, 2,-22,
-21, -9, -4,  2, -9, -4, 2,-21,
-21, -8, -3,  0, -8, -3, 0,-21,
-25,-16,-16, -9,-16,-16,-9,-25 };

int ses_w_Table[] = {
-161,-96,-80,-73,-73,-80,-96,-161,
 -83,-43,-21, 10, 10,-21,-43, -83,
 -71,-22,  0,  9,  9,  0,-22, -71,
 -25, 18, 43, 47, 47, 43, 18, -25,
 -26, 16, 38, 50, 50, 38, 16, -26,
 -11, 37, 56, 65, 65, 56, 37, -11,
 -63,-19,  5, 14, 14,  5,-19, -63,
-195,-67,-42,-29,-29,-42,-67,-195 };

int ses_b_Table[] = {
-195,-67,-42,-29,-29,-42,-67,-195,
 -63,-19,  5, 14, 14,  5,-19, -63,
 -11, 37, 56, 65, 65, 56, 37, -11,
 -26, 16, 38, 50, 50, 38, 16, -26,
 -25, 18, 43, 47, 47, 43, 18, -25,
 -71,-22,  0,  9,  9,  0,-22, -71,
 -83,-43,-21, 10, 10,-21,-43, -83,
-161,-96,-80,-73,-73,-80,-96,-161 };

int tuuk_w_Table[] = {
-25,-16,-16,-9,-9,-16,-16,-25,
-21, -8, -3, 0, 0, -3, -8,-21, 
-21, -9, -4, 2, 2, -4, -9,-21,
-22, -6, -1, 2, 2, -1, -6,-22,
-22, -7,  0, 1, 1,  0, -7,-22,
-21, -7,  0, 2, 2,  0, -7,-21,
-12,  4,  8,12,12,  8,  4,-12, 
-23,-15,-11,-5,-5,-11,-15,-23 };

int tuuk_b_Table[] = {
-23,-15,-11, -5,-15,-11,-5,-23,
-12,  4,  8, 12,  4,  8,12,-12,
-21, -7,  0,  2, -7,  0, 2,-21,
-22, -7,  0,  1, -7,  0, 1,-22,
-22, -6, -1,  2, -6, -1, 2,-22,
-21, -9, -4,  2, -9, -4, 2,-21,
-21, -8, -3,  0, -8, -3, 0,-21,
-25,-16,-16, -9,-16,-16,-9,-25 };

int trey_b_Table2[] = {
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,
 5,  5, 10, 20, 20,  5,  5,  5,
 5,  5, 10, 25, 25, 10,  5,  5,
10, 10, 20, 30, 30, 20, 10, 10,
50, 50, 50, 50, 50, 50, 50, 50,
 0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0};

int trey_w_Table2[] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
 50, 50, 50, 50, 50, 50, 50, 50,
 10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 25, 25, 10,  5,  5,
  5,  5, 10, 20, 20,  5,  5,  5,
  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0};

int ses_b_Table2[] = {
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  5,  5,  0,-20,-40,
-30,  5, 10, 15, 15, 10,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 10, 15, 15, 10,  0,-30,
-40,-20,  0, 10, 10,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50};

int ses_w_Table2[] = {
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0, 10, 10,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50};

int koul_b_Table2[] = {
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  5,  0,  0,  0,  0,  5,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
-20,-10,-10,-10,-10,-10,-10,-20};

int koul_w_Table2[] = {
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  5,  5,  5,  5,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20};

int tuuk_b_Table2[] = {
  0,  0,  5,  5,  5,  5,  0,  0,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5,  5, 10, 10, 10, 10,  5,  5,
  0,  0,  0,  0,  0,  0,  0,  0};

int tuuk_w_Table2[] = {
  0,  0,  0,  0,  0,  0,  0,  0,
  5,  5, 10, 10, 10, 10,  5,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  5,  5,  5,  5,  0,  0,};

int neang_b_Table2[] = {
-10, -5, -5, -5, -5, -5, -5,-10,
 -5,  5,  0,  0,  0,  0,  5, -5,
 -5, 10, 10, 10, 10, 10, 10, -5,
 -5,  0, 10, 10, 10, 10,  0, -5,
 -5,  5,  5, 10, 10,  5,  5, -5,
 -5,  0,  5, 10, 15,  5,  0, -5,
 -5,  0,  5,  5,  5,  5,  0, -5,
-10, -5, -5, -5, -5, -5, -5,-10};

int neang_w_Table2[] = {
-10, -5, -5, -5, -5, -5, -5,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
 -5,  0,  5, 10, 15,  5,  0, -5,
 -5,  5,  5, 10, 10,  5,  5, -5,
 -5,  0, 10, 10, 10, 10,  0, -5,
  5, 10, 10, 10, 10, 10, 10, -5,
 -5,  5,  0,  0,  0,  0,  5, -5,
-10, -5, -5, -5, -5, -5, -5,-10};

int ang_b_Table2[] = {
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20, 10,-20, 10,-20,-20,-10,
 20, 30, 10, 10, 10, 30, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20};

int ang_w_Table2[] = {
 20, 30, 10,  0,  0, 10, 30, 20,
 20, 30, 10, 10, 10, 30, 20, 20,
-10,-20, 10,-20, 10,-20,-20,-10,
-20,-30,-30,-40,-40,-30,-30,-20,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30};

int ang_w_Table[] = {
267,320,270,195,195,270,320,267,
264,304,238,180,180,238,304,264,
200,245,176,110,110,176,245,200,
177,185,148,110,110,148,185,177,
149,177,115, 66, 66,115,177,149,
118,159, 84, 41, 41, 84,159,118,
 87,128, 63, 20, 20, 63,128, 87,
 63, 88, 47,  0,  0, 47, 88, 63 };

int ang_b_Table[] = {
 63, 88, 47,  0,  0, 47, 88, 63,
 87,128, 63, 20, 20, 63,128, 87,
118,159, 84, 41, 41, 84,159,118,
149,177,115, 66, 66,115,177,149,
177,185,148,110,110,148,185,177,
200,245,176,110,110,176,245,200,
264,304,238,180,180,238,304,264,
267,320,270,195,195,270,320,267 };
const int size_b = 230400;
const int size_bb = 1536;
const int size_bbb = 8;
int num[] = {0, 8, 7, 6, 5, 4, 3, 2, 1};
char abc[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char number[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
int first_w_ang= 0;
int first_b_ang = 0;
int first_w_neang = 0;
int first_b_neang = 0;

struct count{
    int moves;
    int count;
} rule1, rule2, rule3, rule4, rule5, rule6, rule7, rule8;

void clear();
int lens(Arr s);
int lenz(char (*b)[24]);
bool valueIn(char ele[], Arr arr);
void Free(Arr arr);
bool check_move(char pos[]);
bool check_element(char s[][24], char pos[], int len);
bool alphanum(char x);
int pos_from_str(char *str);
char *returnNotation(int a, int b);
Arr returnMove(Arr arr, char pos[]);
Arr sub1(Arr arr);
Arr sub2(Arr arr);
Arr trey_pseudo(char pos[], char color);
Arr trey_can_move(char pos[], char color);
Arr neang_can_ove(char pos[], char color);
Arr koul_can_move(char pos[], char color);
Arr ses_can_move(char pos[], char color);
Arr tuuk_can_move(char pos[], char color);
Arr ang_can_move(char pos[], char color);
Arr all_pseudo(int mode, char color);
Arr checkKing(char color);
bool checkK(char color);
void check_piece_move(char move[], char piece[], char color);
bool king_safe(char move[], char color);
Arr all_legal(char color);
int mate(char color);
int checkmate(char color);
Table findPiece();
int sum_over_board(char piece, Table arr, int type);
int eval_board(int type);
int quiesce(int alpha, int beta, char player, int type);
int quiesce_Search(int depth, int alpha, int beta, char player, int type);
int minimax(int depth, int alpha, int beta, char player, int type);
char *selectMove(int depth, int alpha, int beta, char color, int type);
char *select_Move(int depth, int alpha, int beta, int type);
Arr all_priority(Arr arr, char color);
Table copy_board();
bool isnum(char x[]);
void display();
void change_color(char color);
void twoplayers(FILE *file);
void vsBot(FILE *file);
void botFun(FILE *file);
void print_board();
int minimax_test(int depth, char player);

int main(){
    srand(time(NULL));
    #ifdef _WIN32 
        SetConsoleOutputCP(CP_UTF8);
    #endif
    clear();
    MKDIR("data");
    FILE *f;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[30];
    char name[FILENAME_MAX];

    GetCurrentDir(name, FILENAME_MAX);
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,64,"/data/%d-%m-%y_%Hx%Mx%S.txt",timeinfo);
    strcat(name, buffer);
    
    f = fopen(name, "w");
    char nani;
    do
    {
        printf("*********************************\n");
        printf("*     Welcome to Chess Game     *\n");
        printf("*********************************\n");
        printf("* 1. Two Players                *\n");
        printf("* 2. VS Bot                     *\n");
        printf("* 3. Bot Fun                    *\n");
        printf("*********************************\n");
        printf("\nChoose an option: ");
        scanf(" %c", &nani);
        switch (nani)
        {
        case '1':
            twoplayers(f);
            break;
        case '2':
            vsBot(f);
            break;
        case '3':
            botFun(f);
        default:
            printf("\nInvalid\n");
            break;
        }
    } while (nani != 'x');
    fclose(f);
    
}
void clear(){
    #if defined(__linux__) || defined(__unix__)
        system("clear");
    #endif

    #if defined(_WIN32)
        system("cls");
    #endif
}

int lens(Arr s){
    int len =0;
    for (len=0; strcmp(s[len], "69")!=0; len++);
    return len;
}

int lenz(char (*b)[24]){
    int len =0; 
    for (len=0; strcmp(b[len], "69")!=0; len++);
    return len;
}

bool isnum(char x[]){
    for (int i=0; i<strlen(x); i++){
        if (isdigit(x[i])==false){
            return false;
        }
    }
    return true;
}

bool valueIn(char ele[], Arr arr){
    int i=0;
    while (strcmp(arr[i], "69")!=0){
        if (strcmp(ele, arr[i])==0){
            return true;
        } else {
            i++; 
        }
    } return false;
}
void Free(Arr arr){
    int i=0;
    while (strcmp(arr[i], "69")!=0){
        free(arr[i++]);
    }
    free(arr[i]);
    free(arr);
}
bool check_move(char pos[]){
    if (strlen(pos) == 4){
        if ((pos[0] >= 'a' && pos[0] <= 'h') && (pos[2] >= 'a' && pos[2] <= 'h')){
            if ((pos[1] >= '1' && pos[1] <= '8') && (pos[3] >= '1' && pos[3] <= '8')){
                return true;
            }
        }
    }
    return false;
}
bool check_element(char s[][24], char pos[], int len){
    for (int k=0; k<=len-1; k++){
        if (strcmp(s[k], board[pos[1]-'1'][pos[0]-'a'])==0){
            return true;
        }
    }
    return false;
}
bool alphanum(char x) {
    return (x >= 'a' && x <= 'h') || (x >= '1' && x <= '8');
}
int pos_from_str(char *str) {
    return (str[0] - 'a') + (str[1] - '1') * 8;
}
char *returnNotation(int a, int b) {
    char* word = malloc(sizeof(char) * 3);
    word[0] = 'a' + b;
    word[1] = '1' + a;
    word[2] = '\0';
    return word;
}
Arr returnMove(Arr arr, char pos[]){
    int len;
    for (len=0; strcmp(arr[len], "69")!=0; len++);
    char var[5];
    strcpy(var, pos);
    Arr move = malloc((len+1) * sizeof(char*));
    for (int i=0; i<=len-1; i++){
        move[i] = malloc(5 * sizeof(char));
        var[2] = arr[i][0];
        var[3] = arr[i][1];
        var[4] = '\0';
        strcpy(move[i], var);
        free(arr[i]);
    }
    free(arr[len]);
    move[len] = malloc(5 * sizeof(char));
    strcpy(move[len], "69");
    free(arr);
    return move;
}
Arr sub1(Arr arr){
    int len = lens(arr);
    Arr subArr = malloc((len+1) * sizeof(char*));
    for (int i=0; strcmp(arr[i], "69")!=0; i++){
        subArr[i] = malloc(3 * sizeof(char));
        strncpy(subArr[i], &arr[i][2], 3);
    }
    subArr[len] = malloc(3 * sizeof(char));
    strcpy(subArr[len], "69");
    return subArr;
}
Arr sub2(Arr arr){
    int len = lens(arr);
    Arr subArr = malloc((len+1) * sizeof(char*));
    for (int i=0; strcmp(arr[i], "69")!=0; i++){
        subArr[i] = malloc(3 * sizeof(char));
        strncpy(subArr[i], &arr[i][0], 2);
        subArr[i][3] = '\0';
    }
    subArr[len] = malloc(3 * sizeof(char));
    strcpy(subArr[len], "69");
    return subArr;
}

Arr trey_pseudo(char pos[], char color){
    Arr trey = malloc(4 * sizeof(char*));
    int index=0;
    char check3[3], check2[3], check1[3];
    if (color=='w'){
        check1[0] = pos[0] + 1;
        check1[1] = pos[1] + 1;
        check1[2] = '\0';
        check2[0] = pos[0] - 1;
        check2[1] = pos[1] + 1;
        check2[2] = '\0';
        check3[0] = pos[0];
        check3[1] = pos[1] + 1;
        check3[2] = '\0';
        if (alphanum(check1[0]) && alphanum(check1[1]) && check_element(white, check1,13)==false){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check1);
        }
        if (alphanum(check2[0]) && alphanum(check2[1])&& check_element(white, check2, 13)==false){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check2);
        }
        if (alphanum(check3[0]) && alphanum(check3[1])){
            if (check_element(black, check3, 13)==false && check_element(white, check3, 13)==false){
                trey[index] = malloc(5 * sizeof(char));
                strcpy(trey[index++], check3);
            }
        }
    } else if (color=='b'){
        check1[0] = pos[0] - 1;
        check1[1] = pos[1] - 1;
        check1[2] = '\0';
        check2[0] = pos[0] + 1;
        check2[1] = pos[1] - 1;
        check2[2] = '\0';
        check3[0] = pos[0];
        check3[1] = pos[1] - 1;
        check3[2] = '\0';
        if (alphanum(check1[0]) && alphanum(check1[1])&& check_element(black, check1, 13)==false){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check1);
        }
        if (alphanum(check2[0]) && alphanum(check2[1]) && check_element(black, check2, 13)==false){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check2);
        }
        if (alphanum(check3[0]) && alphanum(check3[1])){
            if (check_element(white, check3, 13)==false && check_element(black, check3, 13)==false){
                trey[index] = malloc(5 * sizeof(char));
                strcpy(trey[index++], check3);
            }
        }
    }
    trey[index] = malloc(5 * sizeof(char));
    strcpy(trey[index], "69");
    return trey;
}
Arr trey_can_move(char pos[], char color){
    Arr trey = malloc(4 * sizeof(char*));
    int index=0;
    char check3[3], check2[3], check1[3];
    if (color=='w'){
        check1[0] = pos[0] + 1;
        check1[1] = pos[1] + 1;
        check1[2] = '\0';
        check2[0] = pos[0] - 1;
        check2[1] = pos[1] + 1;
        check2[2] = '\0';
        check3[0] = pos[0];
        check3[1] = pos[1] + 1;
        check3[2] = '\0';
        if (alphanum(check1[0]) && alphanum(check1[1]) && check_element(black, check1,13)){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check1);
        }
        if (alphanum(check2[0]) && alphanum(check2[1])&& check_element(black, check2, 13)){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check2);
        }
        if (alphanum(check3[0]) && alphanum(check3[1])){
            if (check_element(white, check3, 13)==false && check_element(black, check3, 13)==false){
                trey[index] = malloc(5 * sizeof(char));
                strcpy(trey[index++], check3);
            }
        }
    } else if (color=='b'){
        check1[0] = pos[0] - 1;
        check1[1] = pos[1] - 1;
        check1[2] = '\0';
        check2[0] = pos[0] + 1;
        check2[1] = pos[1] - 1;
        check2[2] = '\0';
        check3[0] = pos[0];
        check3[1] = pos[1] - 1;
        check3[2] = '\0';
        if (alphanum(check1[0]) && alphanum(check1[1])&& check_element(white, check1, 13)){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check1);
        }
        if (alphanum(check2[0]) && alphanum(check2[1]) && check_element(white, check2, 13)){
            trey[index] = malloc(5 * sizeof(char));
            strcpy(trey[index++], check2);
        }
        if (alphanum(check3[0]) && alphanum(check3[1])){
            if (check_element(white, check3, 13)==false && check_element(black, check3, 13)==false){
                trey[index] = malloc(5 * sizeof(char));
                strcpy(trey[index++], check3);
            }
        }
    }
    trey[index] = malloc(5 * sizeof(char));
    strcpy(trey[index], "69");
    return trey;
}
Arr neang_can_move(char pos[], char color){
    Arr neang_move = malloc(6 * sizeof(char*));
    int arr[] = {1, 1, 1, -1, -1, -1, -1, 1, 1};
    char tmp[3];
    int index = 0;
    if (color=='w'){
        for (int i=0; i<=3; i++){
            if (alphanum(pos[0] + arr[2*i]) && alphanum(pos[1] + arr[2*i+1])){
                tmp[0] = pos[0] + arr[2*i];
                tmp[1] = pos[1] + arr[2*i+1];
                tmp[2] = '\0';
                if (check_element(white, tmp, 13)==false){
                    neang_move[index] = malloc(5 * sizeof(char));
                    strcpy(neang_move[index++], tmp);
                }
            }
        }
        if (strcmp(pos, "e1")==0 && first_w_neang==0){
            tmp[0] = pos[0];
            tmp[1] = pos[1] + 2;
            tmp[2] = '\0';
            if (check_element(white, tmp, 13)==false){
                neang_move[index] = malloc(5 * sizeof(char));
                strcpy(neang_move[index++], tmp);
            }
        }
    } else if (color=='b'){
        for (int i=0; i<=3; i++){
            if (alphanum(pos[0] + arr[2*i]) && alphanum(pos[1] + arr[2*i+1])){
                tmp[0] = pos[0] + arr[2*i];
                tmp[1] = pos[1] + arr[2*i+1];
                tmp[2] = '\0';
                if (check_element(black, tmp, 13)==false){
                    neang_move[index] = malloc(5 * sizeof(char));
                    strcpy(neang_move[index++], tmp);
                }
            }
        }
        if (strcmp(pos, "d8")==0 && first_b_neang==0){
            tmp[0] = pos[0];
            tmp[1] = pos[1] - 2;
            tmp[2] = '\0';
            if (check_element(black, tmp, 13)==false){
                neang_move[index] = malloc(5 * sizeof(char));
                strcpy(neang_move[index++], tmp);
            }
        }
    }
    neang_move[index] = malloc(5 * sizeof(char));
    strcpy(neang_move[index], "69");
    return neang_move;
}
Arr koul_can_move(char pos[], char color){
    Arr koul_move = malloc(6 * sizeof(char*));
    int arr1[] = {1, 1, 1, -1, -1, -1, -1, 1, 0, 1};
    int arr2[] = {1, 1, 1, -1, -1, -1, -1, 1, 0, -1};
    char tmp[3];
    int index = 0;
    if (color=='w'){
        for (int i=0; i<=4; i++){
            if (alphanum(pos[0] + arr1[2*i]) && alphanum(pos[1] + arr1[2*i+1])){
                tmp[0] = pos[0] + arr1[2*i];
                tmp[1] = pos[1] + arr1[2*i+1];
                tmp[2] = '\0';
                if (check_element(white, tmp, 13)==false){
                    koul_move[index] = malloc(5 * sizeof(char));
                    strcpy(koul_move[index++], tmp);
                }
            }
        }
    } else if (color=='b'){
        for (int i=0; i<=4; i++){
            if (alphanum(pos[0] + arr2[2*i]) && alphanum(pos[1] + arr2[2*i+1])){
                tmp[0] = pos[0] + arr2[2*i];
                tmp[1] = pos[1] + arr2[2*i+1];
                tmp[2] = '\0';
                if (check_element(black, tmp, 13)==false){
                    koul_move[index] = malloc(5 * sizeof(char));
                    strcpy(koul_move[index++], tmp);
                }
            }
        }
    }
    koul_move[index] = malloc(5 * sizeof(char));
    strcpy(koul_move[index], "69");
    return koul_move;
}
Arr ses_can_move(char pos[], char color){
    Arr ses_move = malloc(9 * sizeof(char*));
    int arr[] = {1, 2, -1, 2, -2, 1, -2, -1, -1, -2, 1, -2, 2, -1, 2, 1};
    char tmp[3];
    int index = 0;
    if (color=='w'){
        for (int i=0; i<=7; i++){
            if (alphanum(pos[0] + arr[2*i]) && alphanum(pos[1] + arr[2*i+1])){
                tmp[0] = pos[0] + arr[2*i];
                tmp[1] = pos[1] + arr[2*i+1];
                tmp[2] = '\0';
                if (check_element(white, tmp, 13)==false){
                    ses_move[index] = malloc(5 * sizeof(char));
                    strcpy(ses_move[index++], tmp);
                }
            }
        }
    } else if (color=='b'){
        for (int i=0; i<=7; i++){
            if (alphanum(pos[0] + arr[2*i]) && alphanum(pos[1] + arr[2*i+1])){
                tmp[0] = pos[0] + arr[2*i];
                tmp[1] = pos[1] + arr[2*i+1];
                tmp[2] = '\0';
                if (check_element(black, tmp, 13)==false){
                    ses_move[index] = malloc(5 * sizeof(char));
                    strcpy(ses_move[index++], tmp);
                }
            }
        }
    }
    ses_move[index] = malloc(5*sizeof(char));
    strcpy(ses_move[index], "69");
    return ses_move;
}
Arr tuuk_can_move(char pos[], char color){
    int i, j;
    char tuuk_move[4][15][3];
    Arr tuuk = malloc(15 * sizeof(char*));
    int m=0,n=0,p=0,q=0,index=0;
    for (int i=7; i>=0; i--){
        if (pos[1] != number[i]){
            if (pos[1] > number[i]){
                tuuk_move[0][m][0] = pos[0];
                tuuk_move[0][m][1] = number[i];
                tuuk_move[0][m++][2] = '\0';
            }
        }
        if (pos[0] != abc[i]){
            if (pos[0] > abc[i]){
                tuuk_move[2][p][0] = abc[i];
                tuuk_move[2][p][1] = pos[1];
                tuuk_move[2][p++][2] = '\0';
            }
        }
    }
    for (int i=0; i<=7; i++){
        if (pos[1] != number[i]){
            if (pos[1] < number[i]){ 
                tuuk_move[1][n][0] = pos[0];
                tuuk_move[1][n][1] = number[i];
                tuuk_move[1][n++][2] = '\0';
            }
        }
        if (pos[0] != abc[i]){
            if (pos[0] < abc[i]){
                tuuk_move[3][q][0] = abc[i];
                tuuk_move[3][q][1] = pos[1];
                tuuk_move[3][q++][2] = '\0';
            }
        }
    }
    strcpy(tuuk_move[0][m], "69");
    strcpy(tuuk_move[1][n], "69");
    strcpy(tuuk_move[2][p], "69");
    strcpy(tuuk_move[3][q], "69");
    if (color=='w'){
        for (int i=0; i<=3; i++){
            for (j=0; strcmp(tuuk_move[i][j], "69")!=0; j++){
                if (check_element(all, tuuk_move[i][j], 26)==false){
                    tuuk[index] = malloc(5 * sizeof(char));
                    strcpy(tuuk[index++], tuuk_move[i][j]);
                } else {
                    if (check_element(black, tuuk_move[i][j], 13)){
                        tuuk[index] = malloc(5 * sizeof(char));
                        strcpy(tuuk[index++], tuuk_move[i][j]);
                        break;
                    } else {
                        break;
                    }
                }
            }
        } 
    } else if (color=='b'){
        for (int i=0; i<=3; i++){
            for (j=0; strcmp(tuuk_move[i][j], "69")!=0; j++){
                if (check_element(all, tuuk_move[i][j], 24)==false){
                    tuuk[index] = malloc(5 * sizeof(char));
                    strcpy(tuuk[index++], tuuk_move[i][j]);
                } else {
                    if (check_element(white, tuuk_move[i][j], 13)){
                        tuuk[index] = malloc(5 * sizeof(char));
                        strcpy(tuuk[index++], tuuk_move[i][j]);
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    tuuk[index] = malloc(5 * sizeof(char));
    strcpy(tuuk[index], "69");
    return tuuk;
}

Arr ang_can_move(char pos[], char color){
    Arr ang_move = malloc(11 * sizeof(char*));
    int arr1[] = {1, 1, 1, -1, -1, -1, -1, 1, 0, 1, -1, 0, 1, 0, 0, -1, 2, 1, -2, 1, 1, 2, -1, 2};
    int arr2[] = {1, 1, 1, -1, -1, -1, -1, 1, 0, 1, -1, 0, 1, 0, 0, -1, 2, -1, -2, -1, 1, -2, -1, -2};
    char tmp[3];
    int index = 0;
    if (color=='w'){
        for (int i=0; i<=7; i++){
            if (alphanum(pos[0] + arr1[2*i]) && alphanum(pos[1] + arr1[2*i+1])){
                tmp[0] = pos[0] + arr1[2*i];
                tmp[1] = pos[1] + arr1[2*i+1];
                tmp[2] = '\0';
                if (check_element(white, tmp, 13)==false){
                    ang_move[index] = malloc(5 * sizeof(char));
                    strcpy(ang_move[index++], tmp);
                }
            }
        }
        if (strcmp(pos, "d1")==0 && first_w_ang==0 && strcmp(board[pos[1]-'1'][pos[0]-'a'], RED"\u265A "RESET)!=0){
            for (int i=8; i<=11; i++){
                tmp[0] = pos[0] + arr1[2*i];
                tmp[1] = pos[1] + arr1[2*i+1];
                tmp[2] = '\0';
                if (check_element(white, tmp, 13)==false){
                    ang_move[index] = malloc(5 * sizeof(char));
                    strcpy(ang_move[index++], tmp);
                }
            }
        }
    } else if (color=='b'){
        for (int i=0; i<=7; i++){
            if (alphanum(pos[0] + arr2[2*i]) && alphanum(pos[1] + arr2[2*i+1])){
                tmp[0] = pos[0] + arr2[2*i];
                tmp[1] = pos[1] + arr2[2*i+1];
                tmp[2] = '\0';
                if (check_element(black, tmp, 13)==false){
                    ang_move[index] = malloc(5 * sizeof(char));
                    strcpy(ang_move[index++], tmp);
                }
            }
        }
        if (strcmp(pos, "e8")==0 && first_b_ang==0 && strcmp(board[pos[1]-'1'][pos[0]-'a'], PURPLE"\u265A "RESET)!=0){
            for (int i=8; i<=11; i++){
                tmp[0] = pos[0] + arr2[2*i];
                tmp[1] = pos[1] + arr2[2*i+1];
                tmp[2] = '\0';
                if (check_element(black, tmp, 13)==false){
                    ang_move[index] = malloc(5 * sizeof(char));
                    strcpy(ang_move[index++], tmp);
                }
            }
        }
    }
    ang_move[index] = malloc(5 * sizeof(char));
    strcpy(ang_move[index], "69");
    return ang_move;
}
Arr all_pseudo(int mode, char color){
    int i, j;
    Arr arr;
    char *pos;
    Arr all = malloc(92 * sizeof(char*));
    int index = 0;
    if (color=='w'){
        for (int i =0; i<=7; i++){
            for (int j =0; j<=7; j++){
                if (strcmp(board[i][j], "\u265F ")==0 || strcmp(board[i][j], YELLOW"\u265F "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    if (mode==0){
                        arr = returnMove(trey_pseudo(pos, color), pos);
                        for (k=0; strcmp(arr[k], "69")!=0; k++){
                            all[index] = malloc(5*sizeof(char));
                            strcpy(all[index++], arr[k]);
                            free(arr[k]);
                        }
                    } else if (mode==1){
                        arr = returnMove(trey_can_move(pos, color), pos);
                        for (k=0; strcmp(arr[k], "69")!=0; k++){
                            all[index] = malloc(5*sizeof(char));
                            strcpy(all[index++], arr[k]);
                            free(arr[k]);
                        }
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], "\u265B ")==0 || strcmp(board[i][j], YELLOW"\u265B "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(neang_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], "\u265E ")==0 || strcmp(board[i][j], YELLOW"\u265E "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(ses_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], "\u265D ")==0 || strcmp(board[i][j], YELLOW"\u265D "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(koul_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], "\u265C ")==0 || strcmp(board[i][j], YELLOW"\u265C "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(tuuk_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], "\u265A ")==0 
                        || strcmp(board[i][j], YELLOW"\u265A "RESET)==0
                        || strcmp(board[i][j], RED"\u265A "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(ang_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                }
            }
        }
    } else if (color=='b'){
        for (int i =0; i<=7; i++){
            for (int j =0; j<=7; j++){
                if (strcmp(board[i][j], GREEN"\u265F "RESET)==0 || strcmp(board[i][j], CYAN"\u265F "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    if (mode==0){
                        arr = returnMove(trey_pseudo(pos, color), pos);
                        for (k=0; strcmp(arr[k], "69")!=0; k++){
                            all[index] = malloc(5*sizeof(char));
                            strcpy(all[index++], arr[k]);
                            free(arr[k]);
                        }
                    } else if (mode==1){
                        arr = returnMove(trey_can_move(pos, color), pos);
                        for (k=0; strcmp(arr[k], "69")!=0; k++){
                            all[index] = malloc(5*sizeof(char));
                            strcpy(all[index++], arr[k]);
                            free(arr[k]);
                        }
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], GREEN"\u265B "RESET)==0 || strcmp(board[i][j], CYAN"\u265B "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(neang_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], GREEN"\u265E "RESET)==0 || strcmp(board[i][j], CYAN"\u265E "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(ses_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], GREEN"\u265D "RESET)==0 || strcmp(board[i][j], CYAN"\u265D "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(koul_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], GREEN"\u265C "RESET)==0 || strcmp(board[i][j], CYAN"\u265C "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(tuuk_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                } else if (strcmp(board[i][j], GREEN"\u265A "RESET)==0 
                        || strcmp(board[i][j], CYAN"\u265A "RESET)==0
                        || strcmp(board[i][j], PURPLE"\u265A "RESET)==0){
                    int k=0;
                    pos = returnNotation(i, j);
                    arr = returnMove(ang_can_move(pos, color), pos);
                    for (k=0; strcmp(arr[k], "69")!=0; k++){
                        all[index] = malloc(5*sizeof(char));
                        strcpy(all[index++], arr[k]);
                        free(arr[k]);
                    }
                    free(arr[k]);
                    free(arr);
                    free(pos);
                }
            }
        }
    }
    all[index] = malloc(5*sizeof(char));
    strcpy(all[index], "69");
    return all;
}

Arr new_arr(Arr arr1, Arr arr2){
    int i, j;
    int len1 = lens(arr1); 
    int len2 = lens(arr2); 
    int index = 0;
    Arr arr3 = malloc((len1+1) * sizeof(char*));
    for (int i=0; i<len1; i++){   
        for (j=0; j<len2; j++){
            if (strcmp(arr1[i], arr2[j])==0){
                break;
            }
            if (j == len2 - 1){
                arr3[index] = malloc(5 * sizeof(char));
                strcpy(arr3[index++], arr1[i]); 
            }
        }
    }
    arr3[index] = malloc(5 * sizeof(char));
    strcpy(arr3[index++], "69"); 
    return arr3;
}
Arr checkKing(char color){
    Arr thread = malloc(8 * sizeof(char*));
    Arr all_now;
    int index=0;
    if (color=='w'){
        all_now = all_legal('b');
        for (int i=0; strcmp(all_now[i], "69")!=0; i++){
            char x = all_now[i][2];
            char y = all_now[i][3];
            if (strcmp(board[y-'1'][x-'a'], "\u265A ")==0
                || strcmp(board[y-'1'][x-'a'], YELLOW"\u265A "RESET)==0
                || strcmp(board[y-'1'][x-'a'], RED"\u265A "RESET)==0){
                thread[index] = malloc(5 * sizeof(char));
                strcpy(thread[index++], all_now[i]);
            }
        }
    } else if (color=='b'){
        all_now = all_legal('w');
        for (int i=0; strcmp(all_now[i], "69")!=0; i++){
            char x = all_now[i][2];
            char y = all_now[i][3];
            if (strcmp(board[y-'1'][x-'a'], GREEN"\u265A "RESET)==0
                || strcmp(board[y-'1'][x-'a'], CYAN"\u265A "RESET)==0
                || strcmp(board[y-'1'][x-'a'], PURPLE"\u265A "RESET)==0){
                thread[index] = malloc(5 * sizeof(char));
                strcpy(thread[index++], all_now[i]);
            }
        }
    }
    Free(all_now);
    thread[index] = malloc(5 * sizeof(char));
    strcpy(thread[index++], "69");
    return thread;
}
bool checkK(char color){
    Arr all_now;
    if (color=='w'){
        all_now = all_pseudo(0, 'b');
        for (int i=0; strcmp(all_now[i], "69")!=0; i++){
            char x = all_now[i][2];
            char y = all_now[i][3];
            if (strcmp(board[y-'1'][x-'a'], "\u265A ")==0
                || strcmp(board[y-'1'][x-'a'], YELLOW"\u265A "RESET)==0
                || strcmp(board[y-'1'][x-'a'], RED"\u265A "RESET)==0){
                Free(all_now);
                return true;
            }
        }
    } else if (color=='b'){
        all_now = all_pseudo(0, 'w');
        for (int i=0; strcmp(all_now[i], "69")!=0; i++){
            char x = all_now[i][2];
            char y = all_now[i][3];
            if (strcmp(board[y-'1'][x-'a'], GREEN"\u265A "RESET)==0
            || strcmp(board[y-'1'][x-'a'], CYAN"\u265A "RESET)==0
            || strcmp(board[y-'1'][x-'a'], PURPLE"\u265A "RESET)==0){
                Free(all_now);
                return true;
            }
        }
    }
    Free(all_now);
    return false;
}
void check_piece_move(char move[], char piece[], char color){
    char pos1[3], temp1[24];
    char pos2[3], temp2[24];
    sscanf(move, "%2s%2s", pos1, pos2);
    strcpy(temp1, board[pos1[1]-'1'][pos1[0]-'a']);
    strcpy(temp2, board[pos2[1]-'1'][pos2[0]-'a']);
    strcpy(board[pos2[1]-'1'][pos2[0]-'a'], piece); //dumb variable
    strcpy(board[pos1[1]-'1'][pos1[0]-'a'], YELLOW"\u26CC "RESET); //dumb variable
    if (color=='w'){
        if (checkK('w')){
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
        }
    } else if (color=='b'){
        if (checkK('b')){
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
        }
    }
}
bool king_safe(char move[], char color){
    char pos1[3], pos2[3];
    char temp1[24], temp2[24];
    sscanf(move, "%2s%2s", pos1, pos2);
    strcpy(temp2, board[pos2[1]-'1'][pos2[0]-'a']);
    strcpy(temp1, board[pos1[1]-'1'][pos1[0]-'a']);
    if (color=='w'){
        if (strcmp(temp1, "\u265A ")==0
            || strcmp(temp1, YELLOW"\u265A "RESET)==0
            || strcmp(temp1, RED"\u265A "RESET)==0){
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], "\u265A ");
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], "\u26CC ");
            if (checkK('w')){
                strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
                strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
                return false;
            }
        } else {
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], "\u265F ");
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], "\u26CC ");
            if (checkK('w')){
                strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
                strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
                return false;
            }
        }
    } else if (color=='b'){
        if (strcmp(temp1, GREEN"\u265A "RESET)==0
            || strcmp(temp1, CYAN"\u265A "RESET)==0
            || strcmp(temp1, PURPLE"\u265A "RESET)==0){
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], GREEN"\u265A "RESET);
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], "\u26CC ");
            if (checkK('b')){
                strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
                strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
                return false;
            }
        } else {
            strcpy(board[pos2[1]-'1'][pos2[0]-'a'], GREEN"\u265F "RESET);
            strcpy(board[pos1[1]-'1'][pos1[0]-'a'], "\u26CC ");
            if (checkK('b')){
                strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
                strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
                return false;
            }
        }
    }
    strcpy(board[pos2[1]-'1'][pos2[0]-'a'], temp2);
    strcpy(board[pos1[1]-'1'][pos1[0]-'a'], temp1);
    return true;
}
Arr all_legal(char color){
    Arr arr;
    Arr all = malloc(92 * sizeof(char*));
    int index = 0;
    if (color=='w'){
        arr = all_pseudo(1, 'w');
        for (int i=0; strcmp(arr[i], "69")!=0; i++){
            if (king_safe(arr[i], 'w')){
                all[index] = malloc(5 * sizeof(char));
                strcpy(all[index++], arr[i]);
            }
        }
    } else if (color=='b'){
        arr = all_pseudo(1, 'b');
        for (int i=0; strcmp(arr[i], "69")!=0; i++){
            if (king_safe(arr[i], 'b')){
                all[index] = malloc(5 * sizeof(char));
                strcpy(all[index++], arr[i]);
            }
        }
    }
    all[index] = malloc(5*sizeof(char));
    strcpy(all[index], "69");
    Free(arr);
    return all;
}
Table findPiece(){
    Table pos = malloc(12 * sizeof(*pos));
    char *tmp;
    char arr[12][8][24];
    int index1=0,index2=0,index3=0,index4=0,index5=0,index6=0,index7=0,index8=0,index9=0,index10=0,index11=0,index12=0;
    for (int i=0; i<=7; i++){
        for (int j=0; j<=7; j++){
            if (strcmp(board[i][j], "\u265F ")==0 || strcmp(board[i][j], YELLOW"\u265F "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[0][index1++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], "\u265B ")==0 || strcmp(board[i][j], YELLOW"\u265B "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[1][index2++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], "\u265C ")==0 || strcmp(board[i][j], YELLOW"\u265C "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[2][index3++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], "\u265D ")==0 || strcmp(board[i][j], YELLOW"\u265D "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[3][index4++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], "\u265E ")==0 || strcmp(board[i][j], YELLOW"\u265E "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[4][index5++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j],"\u265A ")==0 
                    || strcmp(board[i][j], YELLOW"\u265A "RESET)==0
                    || strcmp(board[i][j], RED"\u265A "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[5][index6++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265F "RESET)==0 || strcmp(board[i][j], CYAN"\u265F "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[6][index7++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265B "RESET)==0 || strcmp(board[i][j], CYAN"\u265B "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[7][index8++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265C "RESET)==0 || strcmp(board[i][j], CYAN"\u265C "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[8][index9++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265D "RESET)==0 || strcmp(board[i][j], CYAN"\u265D "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[9][index10++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265E "RESET)==0 || strcmp(board[i][j], CYAN"\u265E "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[10][index11++], tmp);
                free(tmp);
            } else if (strcmp(board[i][j], GREEN"\u265A "RESET)==0
                    || strcmp(board[i][j], CYAN"\u265A "RESET)==0
                    || strcmp(board[i][j], PURPLE"\u265A "RESET)==0){
                tmp = returnNotation(i, j);
                strcpy(arr[11][index12++], tmp);
                free(tmp);
            }
        }
    }
    strcpy(arr[0][index1], "69");
    strcpy(arr[1][index2], "69");
    strcpy(arr[2][index3], "69");
    strcpy(arr[3][index4], "69");
    strcpy(arr[4][index5], "69");
    strcpy(arr[5][index6], "69");
    strcpy(arr[6][index7], "69");
    strcpy(arr[7][index8], "69");
    strcpy(arr[8][index9], "69");
    strcpy(arr[9][index10], "69");
    strcpy(arr[10][index11], "69");
    strcpy(arr[11][index12], "69");
    memcpy(pos, arr, sizeof(arr));
    return pos;
}

int checkmate(char color){
    Arr board_now;
    if (color=='w'){
        board_now = all_legal('w');
        if (strcmp(board_now[0], "69")==0){
            if (checkK('w')){
                return -1;
            }
            Free(board_now);
            return 0;
        }
    } else if (color=='b'){
        board_now = all_legal('b');
        if (strcmp(board_now[0], "69")==0){
            if (checkK('b')){
                return -1;
            }
            Free(board_now);
            return 0;
        }
    }
    Free(board_now);
    return 1;
}
Table copy_board(){
    Table new_board = malloc(12 * sizeof(*new_board)) ;
    memcpy(new_board, board, sizeof(board));
    return new_board;
}
int sum_over_board(char piece, Table arr, int type){
    int sum=0;
    if (type==1){
        if (piece=='t'){
            for (int i=0; strcmp(arr[0][i], "69")!=0; i++){
                sum += trey_w_Table[pos_from_str(arr[0][i])];
            }
            for (int i=0; strcmp(arr[6][i], "69")!=0; i++){
                sum -= trey_b_Table[pos_from_str(arr[6][i])];
            }
        } else if (piece=='n'){
            for (int i=0; strcmp(arr[1][i], "69")!=0; i++){
                sum += neang_w_Table[pos_from_str(arr[1][i])];
            }
            for (int i=0; strcmp(arr[7][i], "69")!=0; i++){
                sum -= neang_b_Table[pos_from_str(arr[7][i])];
            }
        } else if (piece=='r'){
            for (int i=0; strcmp(arr[2][i], "69")!=0; i++){
                sum += tuuk_w_Table[pos_from_str(arr[2][i])];
            }
            for (int i=0; strcmp(arr[8][i], "69")!=0; i++){
                sum -= tuuk_b_Table[pos_from_str(arr[8][i])];
            }
        } else if (piece=='k'){
            for (int i=0; strcmp(arr[3][i], "69")!=0; i++){
                sum += koul_w_Table[pos_from_str(arr[3][i])];
            }
            for (int i=0; strcmp(arr[9][i], "69")!=0; i++){
                sum -= koul_b_Table[pos_from_str(arr[9][i])];
            }
        } else if (piece=='s'){
            for (int i=0; strcmp(arr[4][i], "69")!=0; i++){
                sum += ses_w_Table[pos_from_str(arr[4][i])];
            }
            for (int i=0; strcmp(arr[10][i], "69")!=0; i++){
                sum -= ses_b_Table[pos_from_str(arr[10][i])];
            }
        } else if (piece=='a'){
            for (int i=0; strcmp(arr[5][i], "69")!=0; i++){
                sum += ang_w_Table[pos_from_str(arr[5][i])];
            }
            for (int i=0; strcmp(arr[11][i], "69")!=0; i++){
                sum -= ang_b_Table[pos_from_str(arr[11][i])];
            }
        }
    } else if (type==2){
        if (piece=='t'){
            for (int i=0; strcmp(arr[0][i], "69")!=0; i++){
                sum += trey_w_Table2[pos_from_str(arr[0][i])];
            }
            for (int i=0; strcmp(arr[6][i], "69")!=0; i++){
                sum -= trey_b_Table2[pos_from_str(arr[6][i])];
            }
        } else if (piece=='n'){
            for (int i=0; strcmp(arr[1][i], "69")!=0; i++){
                sum += neang_w_Table2[pos_from_str(arr[1][i])];
            }
            for (int i=0; strcmp(arr[7][i], "69")!=0; i++){
                sum -= neang_b_Table2[pos_from_str(arr[7][i])];
            }
        } else if (piece=='r'){
            for (int i=0; strcmp(arr[2][i], "69")!=0; i++){
                sum += tuuk_w_Table2[pos_from_str(arr[2][i])];
            }
            for (int i=0; strcmp(arr[8][i], "69")!=0; i++){
                sum -= tuuk_b_Table2[pos_from_str(arr[8][i])];
            }
        } else if (piece=='k'){
            for (int i=0; strcmp(arr[3][i], "69")!=0; i++){
                sum += koul_w_Table2[pos_from_str(arr[3][i])];
            }
            for (int i=0; strcmp(arr[9][i], "69")!=0; i++){
                sum -= koul_b_Table2[pos_from_str(arr[9][i])];
            }
        } else if (piece=='s'){
            for (int i=0; strcmp(arr[4][i], "69")!=0; i++){
                sum += ses_w_Table2[pos_from_str(arr[4][i])];
            }
            for (int i=0; strcmp(arr[10][i], "69")!=0; i++){
                sum -= ses_b_Table2[pos_from_str(arr[10][i])];
            }
        } else if (piece=='a'){
            for (int i=0; strcmp(arr[5][i], "69")!=0; i++){
                sum += ang_w_Table2[pos_from_str(arr[5][i])];
            }
            for (int i=0; strcmp(arr[11][i], "69")!=0; i++){
                sum -= ang_b_Table2[pos_from_str(arr[11][i])];
            }
        }
    }
    return sum;
}
int mvvLVA(char move[]){
    int fromPY = move[1] - '1';
    int fromPX = move[0] - 'a';
    int toPY = move[3] - '1';
    int toPX = move[2] - 'a';
    if (strcmp(board[toPY][toPX], "  ")==0){
        return 0;
    
    } else if (strcmp(board[fromPY][fromPX], "\u265A ")==0
            || strcmp(board[fromPY][fromPX], YELLOW"\u265A "RESET)==0
            || strcmp(board[fromPY][fromPX], RED"\u265A "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265A "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265A "RESET)==0
            || strcmp(board[fromPY][fromPX], PURPLE"\u265A "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][0];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][0];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][0];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][0];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][0];
        }
    } else if (strcmp(board[fromPY][fromPX], "\u265C ")==0
            || strcmp(board[fromPY][fromPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265C "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265C "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][1];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][1];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][1];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][1];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][1];
        }
    } else if (strcmp(board[fromPY][fromPX], "\u265E ")==0
            || strcmp(board[fromPY][fromPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265E "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265E "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][2];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][2];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][2];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][2];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][2];
        }
    } else if (strcmp(board[fromPY][fromPX], "\u265D ")==0
            || strcmp(board[fromPY][fromPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265D "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265D "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][3];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][3];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][3];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][3];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][3];
        }
    } else if (strcmp(board[fromPY][fromPX], "\u265B ")==0
            || strcmp(board[fromPY][fromPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265B "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265B "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][4];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][4];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][4];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][4];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][4];
        }
    } else if (strcmp(board[fromPY][fromPX], "\u265F ")==0
            || strcmp(board[fromPY][fromPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[fromPY][fromPX], GREEN"\u265F "RESET)==0
            || strcmp(board[fromPY][fromPX], CYAN"\u265F "RESET)==0){
        if (strcmp(board[toPY][toPX], "\u265C ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265C "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265C "RESET)==0){
            return mvv_lva[0][5];
        } else if (strcmp(board[toPY][toPX], "\u265E ")
            || strcmp(board[toPY][toPX],   YELLOW"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265E "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265E "RESET)==0){
            return mvv_lva[1][5];
        } else if (strcmp(board[toPY][toPX], "\u265D ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265D "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265D "RESET)==0){
            return mvv_lva[2][5];
        } else if (strcmp(board[toPY][toPX], "\u265B ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265B "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265B "RESET)==0){
            return mvv_lva[3][5];
        } else if (strcmp(board[toPY][toPX], "\u265F ")==0
            || strcmp(board[toPY][toPX],   YELLOW"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], GREEN"\u265F "RESET)==0
            || strcmp(board[toPY][toPX], CYAN"\u265F "RESET)==0){
            return mvv_lva[4][5];
        }
    }
}
int remaining_piece(){
    Table board_rn = findPiece();
    int wTrey = lenz(board_rn[0]);
    int wNeang = lenz(board_rn[1]);
    int wTuuk = lenz(board_rn[2]);
    int wKoul = lenz(board_rn[3]);
    int wSes = lenz(board_rn[4]);
    int bTrey = lenz(board_rn[6]);
    int bNeang = lenz(board_rn[7]);
    int bTuuk = lenz(board_rn[8]);
    int bKoul = lenz(board_rn[9]);
    int bSes = lenz(board_rn[10]);
    int sum = wTrey + wNeang + wTuuk + wKoul + wSes + bTrey + bNeang + bKoul + bSes + 2;
    free(board_rn);
    return sum;
}
int eval_board(int type){
    if (checkmate('w')==-1){
        return -696969;
    } else if (checkmate('b')==-1){
        return 696969;
    } else if (checkmate('w')==0 || checkmate('b')==0){
        return 0;
    }
    Table board_rn = findPiece();
    int wTrey = lenz(board_rn[0]);
    int wNeang = lenz(board_rn[1]);
    int wTuuk = lenz(board_rn[2]);
    int wKoul = lenz(board_rn[3]);
    int wSes = lenz(board_rn[4]);
    
    int bTrey = lenz(board_rn[6]);
    int bNeang = lenz(board_rn[7]);
    int bTuuk = lenz(board_rn[8]);
    int bKoul = lenz(board_rn[9]);
    int bSes = lenz(board_rn[10]);

    int material = 200*(wTrey-bTrey) + 300*(wNeang-bNeang) + 600*(wKoul-bKoul) + 800*(wSes-bSes) + 1300*(wTuuk-bTuuk); 
    int treySq = sum_over_board('t', board_rn, type);    
    int neangSq = sum_over_board('n', board_rn, type);
    int koulSq = sum_over_board('k', board_rn, type);
    int sesSq = sum_over_board('s', board_rn, type);
    int tuukSq = sum_over_board('r', board_rn, type);
    int angSq = sum_over_board('a', board_rn, type);
    int eval = material + treySq + neangSq + koulSq + sesSq + tuukSq + angSq;
    free(board_rn);
    return eval;
}
bool is_capture(char color, char move_piece[5]){
    char des[4];
    strncpy(des, &move_piece[2], 4);
    if (color=='w'){
        if (check_element(black, des, 13)){
            return true;
        }
    } else {
        if (check_element(white, des, 13)){
            return true;
        }
    }
    return false;
}
int quiesce(int alpha, int beta, char player, int type) {
    int stand_pat = eval_board(type);
    if (stand_pat>=beta){
        return beta;
    }
    if (alpha < stand_pat){
        alpha = stand_pat;
    }
    Arr all_current;
    if (player=='w'){
        all_current = all_legal('w');
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            if (is_capture('w', all_current[i])){
                char fromP[3], toP[3], tmp1[24], tmp2[24];
                int fromX, fromY, toX, toY;
                sscanf(all_current[i], "%2s%2s", fromP, toP);
                fromX = fromP[1]-'1';
                fromY = fromP[0]-'a';
                toX = toP[1]-'1';
                toY = toP[0]-'a';       
                strcpy(tmp1, board[fromX][fromY]);
                strcpy(tmp2, board[toX][toY]);
                strcpy(board[toX][toY], tmp1);   
                strcpy(board[fromX][fromY], "  ");

                int score = quiesce(alpha, beta, 'b', type);
                strcpy(board[toX][toY], tmp2);
                strcpy(board[fromX][fromY], tmp1);    
                
                if (score >= beta){
                    Free(all_current);
                    return beta;
                }
                if (score > alpha){
                    alpha = score;
                }
            }
        }
        Free(all_current);
        return alpha;
        
    } else if (player=='b'){   
        all_current = all_legal('b');
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            if (is_capture('b', all_current[i])){
                char fromP[3], toP[3], tmp1[24], tmp2[24];
                int fromX, fromY, toX, toY;
                sscanf(all_current[i], "%2s%2s", fromP, toP);
                fromX = fromP[1]-'1';
                fromY = fromP[0]-'a';
                toX = toP[1]-'1';
                toY = toP[0]-'a';
                strcpy(tmp1, board[fromX][fromY]);
                strcpy(tmp2, board[toX][toY]);
                strcpy(board[toX][toY], tmp1);
                strcpy(board[fromX][fromY], "  ");

                int score = quiesce(alpha, beta, 'b', type);
                strcpy(board[toX][toY], tmp2);
                strcpy(board[fromX][fromY], tmp1);
                
                if (score >= beta){
                    Free(all_current);
                    return beta;
                }
                if (score > alpha){
                    alpha = score;
                }
            }
        }
        Free(all_current);
        return alpha;
    }
}
int quiesce_Search(int depth, int alpha, int beta, char player, int type){
    int check = 0;
    if (player=='w'){
        if (checkmate('b')==-1 || checkmate('b')==0){
            return eval_board(type);
        }
        if (depth==0){
            return quiesce(alpha, beta, 'w', type);
        }
    } else if (player=='b'){
        if (checkmate('w')==-1 || checkmate('w')==0){
            return eval_board(type);
        }
        if (depth==0){
            return quiesce(alpha, beta, 'b', type);
        }
    }
    Arr all_current;
    if (player=='w'){
        all_current = all_legal('w');
        int maxEval = -6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int eval = minimax(depth-1, alpha, beta, 'b', type);
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            maxEval = MAX(maxEval, eval);
            alpha = MAX(alpha, eval);
            if (beta <= alpha){
                break;
            }
        }
        Free(all_current);
        return maxEval;
    } else if (player=='b'){
        all_current = all_legal('b');
        int minEval = 6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int eval = minimax(depth-1, alpha, beta, 'w', type);
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, minEval);
            if (beta <= alpha){
                break;
            }
        }
        Free(all_current);
        return minEval;
    }
}
int minimax(int depth, int alpha, int beta, char player, int type){
    if (player=='w'){
        if (depth==0 || checkmate('w')==-1){
            return eval_board(type);
        }
    } else if (player=='b'){
        if (depth==0 || checkmate('b')==-1){
            return eval_board(type);
        }
    }
    Arr all_current;
    if (player=='w'){
        all_current = all_legal('w');
        int maxEval = -6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int eval = minimax(depth-1, alpha, beta, 'b', type);
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            maxEval = MAX(maxEval, eval);
            alpha = MAX(alpha, eval);
            if (beta <= alpha){
                break;
            }
        }
        Free(all_current);
        return maxEval;
    } else if (player=='b'){
        all_current = all_legal('b');
        int minEval = 6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int eval = minimax(depth-1, alpha, beta, 'w', type);
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, minEval);
            if (beta <= alpha){
                break;
            }
        }
        Free(all_current);
        return minEval;
    }
}
/*int minimax2_test(int depth, char player){
    if (player=='w'){
        if (depth==0 || checkmate('w')==-1){
            //display();
            return eval_board();
        }
    } else if (player=='b'){
        if (depth==0 || checkmate('b')==-1){
            //display();
            return eval_board();
        }
    }
    Arr all_current;
    if (player=='w'){
        all_current = all_legal('w');
        int maxEval = -6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");
            printf("w.%s\n", all_current[i]);

            int eval = minimax_test(depth-1, 'b');
            printf("w.Eval: %d\n", eval);
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            maxEval = MAX(maxEval, eval);
            //printf("\nmax: %d", maxEval);
        }
        Free(all_current);
        return maxEval;
    } else if (player=='b'){
        all_current = all_legal('b');
        int minEval = 6969;
        for (int i=0; strcmp(all_current[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_current[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");
            printf("b.%s\n", all_current[i]);
            
            int eval = minimax_test(depth-1,'w');
            printf("b.Eval: %d\n", eval);
            
            strcpy(board[toX][toY], tmp2);
            strcpy(board[fromX][fromY], tmp1);
            minEval = MIN(minEval, eval);
            //printf("\nmin: %d", minEval);
        }
        Free(all_current);
        return minEval;
    }
}*/
char *selectMove(int depth, int alpha, int beta, char color, int type){
    char *bestMove = malloc(5 * sizeof(char));
    Arr all_moves;
    if (color=='b'){
        int bestValue = 9999;
        all_moves = all_legal('b');
        for (int i=0; strcmp(all_moves[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_moves[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int boardValue = minimax(depth-1, alpha, beta, 'w', type);
            if (boardValue < bestValue){
                bestValue = boardValue;
                strcpy(bestMove, all_moves[i]);
            }
            strcpy(board[toX][toY], tmp2); 
            strcpy(board[fromX][fromY], tmp1);
        }
    } else if (color=='w'){
        int bestValue = -9999;
        all_moves = all_legal('w');
        for (int i=0; strcmp(all_moves[i], "69")!=0; i++){
            char fromP[3], toP[3], tmp1[24], tmp2[24];
            int fromX, fromY, toX, toY;
            sscanf(all_moves[i], "%2s%2s", fromP, toP);
            fromX = fromP[1]-'1';
            fromY = fromP[0]-'a';
            toX = toP[1]-'1';
            toY = toP[0]-'a';
            strcpy(tmp1, board[fromX][fromY]);
            strcpy(tmp2, board[toX][toY]);
            strcpy(board[toX][toY], tmp1);
            strcpy(board[fromX][fromY], "  ");

            int boardValue = minimax(depth-1, alpha, beta, 'b', type);
            if (boardValue > bestValue){
                bestValue = boardValue;
                strcpy(bestMove, all_moves[i]);
            }
            strcpy(board[toX][toY], tmp2); 
            strcpy(board[fromX][fromY], tmp1);
        }
    }
    Free(all_moves);
    return bestMove;
}
Arr all_priority(Arr arr, char color){
    Arr test_arr = sub1(arr);
    int len = lens(arr);
    int index=0;
    Arr new_arr = malloc((len+1) * sizeof(char*));
    if (color=='w'){
        for (int i=0; strcmp(arr[i], "69")!=0; i++){
            if (check_element(white, test_arr[i], 13)){
                new_arr[index] = malloc(5 * sizeof(char));
                strcpy(new_arr[index++], arr[i]);
            }
        }
    } else if (color=='b'){
        for (int i=0; strcmp(arr[i], "69")!=0; i++){
            if (check_element(black, test_arr[i], 13)){
                new_arr[index] = malloc(5 * sizeof(char));
                strcpy(new_arr[index++], arr[i]);
            }
        }
    }
    new_arr[index] = malloc(5 * sizeof(char));
    strcpy(new_arr[index], "69");
    Free(test_arr);
    return new_arr;
}
char *select_Move(int depth, int alpha, int beta, int type){
    char *bestMove = malloc(5 * sizeof(char));
    int bestValue = -9999;
    Arr all_moves = all_legal('w');
    for (int i=0; strcmp(all_moves[i], "69")!=0; i++){
        char fromP[3], toP[3], tmp1[24], tmp2[24];
        int fromX, fromY, toX, toY;
        sscanf(all_moves[i], "%2s%2s", fromP, toP);
        fromX = fromP[1]-'1';
        fromY = fromP[0]-'a';
        toX = toP[1]-'1';
        toY = toP[0]-'a';
        strcpy(tmp1, board[fromX][fromY]);
        strcpy(tmp2, board[toX][toY]);
        strcpy(board[toX][toY], tmp1);
        strcpy(board[fromX][fromY], "  ");

        int boardValue = minimax(depth-1, alpha, beta, 'b', type);
        if (boardValue > bestValue){
            bestValue = boardValue;
            strcpy(bestMove, all_moves[i]);
        }
        strcpy(board[toX][toY], tmp2);    // put the piece into its place
        strcpy(board[fromX][fromY], tmp1);
    }
    Free(all_moves);
    return bestMove;
}
void display(){
    int i, j, k;
    int num[] = {0, 8, 7, 6, 5, 4, 3, 2, 1};
    clear();
    printf("\u2554"); //╔
    for (int i=1; i<=8; i++){
        if (i<=7){
            for (j=1; j<=8; j++){
                printf("\u2550"); // ═
            } printf("\u2566"); // ╦
        
        } else {
            for (j=1; j<=8; j++){
                printf("\u2550"); // ═
            } printf("\u2557\n"); // ╗
        }
    }
    for (k=1; k<=8; k++){  
        if (k % 2 == 1){  
            for (int i=1; i<=8; i++){
                printf("\u2551"); // ║
                if (i % 2 == 0){
                    printf(GREEN" \u250F"); // ┏
                    for (int i=1; i<=4; i++){
                        printf("\u2501"); // ━
                    } printf("\u2513 "RESET); // ┓
                } else {
                    printf(" \u250F"); // ┏
                    for (int i=1; i<=4; i++){
                        printf("\u2501"); // ━
                    } printf("\u2513 "); // ┓
                }
            } printf("\u2551\n"); // ║
            
            for (int i=1; i<=8; i++){
                printf("\u2551"); // ║
                if (i % 2 == 0){
                    printf(GREEN" \u2503 "RESET); // ┃
                    printf("%s ", board[8-k][i-1]); // chess piece
                    printf(GREEN"\u2503 "RESET); // ┃
                } else {
                    printf(" \u2503 %s \u2503 ", board[8-k][i-1]); // ┃ chess piece ┃
                }
            } printf("\u2551    %d\n", num[k]); // ║ then number

            for (int i=1; i<=8; i++){
                printf("\u2551 "); // ║
                if (i % 2 == 0){
                    printf(GREEN"\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B " RESET); // ┛
                } else{
                    printf("\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B "); // ┛
                }
            } printf("\u2551\n"); // ║

            printf("\u2560"); // ╠
            for (int i=1; i<=8; i++){
                if (i<=7){
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ═
                    } printf("\u256C"); // ╬
                } else {
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ═
                    } printf("\u2563"); // ╣
                }
            }printf("\n");
        } else if ((k % 2 == 0) && (k != 8)){
            for (int i=1; i<=8; i++){
                printf("\u2551"); // ║
                if (i % 2 == 1){
                    printf(GREEN" \u250F"); // ┏
                    for (int i=1; i<=4; i++){
                        printf("\u2501"); // ━
                    } printf("\u2513 "RESET); // ┓
                } else {
                    printf(" \u250F"); // ┏
                    for (int i=1; i<=4; i++){
                        printf("\u2501"); // ━
                    } printf("\u2513 "); // ┓
                }
                    
            } printf("\u2551\n"); // ║
            
            for (int i=1; i<=8; i++){
                printf("\u2551"); // ║
                if (i % 2 == 1){
                    printf(GREEN" \u2503 "RESET); // ┃
                    printf("%s ", board[8-k][i-1]); // chess piece
                    printf(GREEN"\u2503 "RESET); // ┃
                } else {
                    printf(" \u2503 %s \u2503 ", board[8-k][i-1]); // ┃ chess piece ┃
                }
            } printf("\u2551    %d\n", num[k]); // ║

            for (int i=1; i<=8; i++){
                printf("\u2551 "); // ║
                if (i % 2 == 1){
                    printf(GREEN"\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B " RESET); // ┛
                } else{
                    printf("\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B "); // ┛
                }
            } printf("\u2551\n"); // ║

            printf("\u2560"); // ╠
            for (int i=1; i<=8; i++){
                if (i<=7){
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ─
                    } printf("\u256C"); // ╬
                } else {
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ─
                    } printf("\u2563"); // ╬
                }
            }printf("\n");

        } else {
            for (int i=1; i<=8; i++){
            printf("\u2551"); // ║
            if (i % 2 == 1){
                printf(GREEN" \u250F"); // ┏
                for (int i=1; i<=4; i++){
                    printf("\u2501"); // ━
                } printf("\u2513 "RESET); // ┓
            } else {
                printf(" \u250F"); // ┏
                for (int i=1; i<=4; i++){
                    printf("\u2501"); // ━
                } printf("\u2513 "); // ┏
            }
                    
            } printf("\u2551\n"); // ║
            
            for (int i=1; i<=8; i++){
                printf("\u2551"); // ║
                if (i % 2 == 1){
                    printf(GREEN" \u2503 "RESET); // ┃
                    printf("%s ", board[8-k][i-1]);  // chess piece
                    printf(GREEN"\u2503 "RESET); // ┃
                } else {
                    printf(" \u2503 %s \u2503 ", board[8-k][i-1]); // ┃ chess piece ┃
                }
            } printf("\u2551    %d\n", num[k]); // ║

            for (int i=1; i<=8; i++){
                printf("\u2551 "); // ║
                if (i % 2 == 1){
                    printf(GREEN"\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B " RESET); // ┛
                } else{
                    printf("\u2517"); // ┗
                    for (j=1; j<=4; j++){
                        printf("\u2501"); // ━
                    } printf("\u251B "); // ┛
                }
            } printf("\u2551\n"); // ║

            printf("\u255A"); // ╚
            for (int i=1; i<=8; i++){
                if (i<=7){
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ═
                    } printf("\u2569"); // ╩
                } else {
                    for (int i=1; i<=8; i++){
                        printf("\u2550"); // ═
                    } printf("\u255D"); // ╝
                }
            }printf("\n");
        }
    } // this is full width unicode
    printf("    \uFF41       \uFF42       \uFF43       \uFF44       \uFF45       \uFF46       \uFF47       \uFF48\n"); // a b c d e f g h i j
}
void change_color(char color){
    int i, j;
    if (color=='w'){
        for (int i=0; i<=7; i++){
            for (j=0; j<=7; j++){
                if (strcmp(board[i][j], YELLOW"\u265F "RESET)==0){
                    strcpy(board[i][j], "\u265F ");
                } else if (strcmp(board[i][j], YELLOW"\u265A "RESET)==0){
                    strcpy(board[i][j], "\u265A ");
                } else if (strcmp(board[i][j], YELLOW"\u265B "RESET)==0){
                    strcpy(board[i][j], "\u265B ");
                } else if (strcmp(board[i][j], YELLOW"\u265C "RESET)==0){
                    strcpy(board[i][j], "\u265C ");
                } else if (strcmp(board[i][j], YELLOW"\u265D "RESET)==0){
                    strcpy(board[i][j], "\u265D ");
                } else if (strcmp(board[i][j], YELLOW"\u265E "RESET)==0){
                    strcpy(board[i][j], "\u265E ");
                } else if (strcmp(board[i][j], YELLOW"\u26CC "RESET)==0){
                    strcpy(board[i][j], "  ");
                } else if (strcmp(board[i][j], RED"\u265A "RESET)==0){
                    strcpy(board[i][j], "\u265A ");
                }
            } 
        }
    } else if (color=='b'){
        for (int i=0; i<=7; i++){
            for (j=0; j<=7; j++){
                if (strcmp(board[i][j], CYAN"\u26CC "RESET)==0){
                    strcpy(board[i][j], "  ");
                } else if (strcmp(board[i][j], CYAN"\u265A "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265A "RESET);
                } else if (strcmp(board[i][j], CYAN"\u265B "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265B "RESET);
                } else if (strcmp(board[i][j], CYAN"\u265C "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265C "RESET);
                } else if (strcmp(board[i][j], CYAN"\u265D "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265D "RESET);
                } else if (strcmp(board[i][j], CYAN"\u265E "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265E "RESET);
                } else if (strcmp(board[i][j], CYAN"\u265F "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265F "RESET);
                } else if (strcmp(board[i][j], CYAN"\u26CC "RESET)==0){
                    strcpy(board[i][j], "  ");
                } else if (strcmp(board[i][j], PURPLE"\u265A "RESET)==0){
                    strcpy(board[i][j], GREEN"\u265A "RESET);
                }
            }
        }
    }
}
int drawing_check(){

}
void twoplayers(FILE *file){
    char index = '0';
    char player1 = '0';
    char player2 = '1';
    char valid = '0';
    int count_color = 0;
    rule1.count = rule2.count = rule3.count = rule4.count = rule5.count = rule6.count = rule7.count = rule8.count = 0;
    rule1.moves=64, rule2.moves=8, rule3.moves=16, rule4.moves=22, rule5.moves=32, rule6.moves=44, rule7.moves=64, rule8.moves=64;
    int indeX = 1;
    int move_left = 1;
    int countdown = 0;
    display(board);
    do {
        Table count_p = findPiece();
        if (strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0 
            && strcmp(count_p[4][0], "69")==0 && strcmp(count_p[8][0], "69")==0
            && strcmp(count_p[9][0], "69")==0 && strcmp(count_p[10][0], "69")==0){
            if ((lenz(count_p[0])+lenz(count_p[1]))<3 || (lenz(count_p[6])+lenz(count_p[7]))<3){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
            && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
            && strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0
            && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0){
            if (lenz(count_p[4])==1 || lenz(count_p[10])==1){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (countdown==0){
            if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
                && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
                && strcmp(count_p[4][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 1;
            } else if (strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0 
                && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0
                && strcmp(count_p[10][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 2;
            }  
        } else if (countdown==2) {
            if (strcmp(count_p[0][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[2])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[2])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[3])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[4])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[3])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[4])==1){
                move_left = rule7.moves - remaining_piece();
                printf(RED"\nMove left: %d\n"RESET, move_left - rule7.count);
                rule7.count++;
            } else if (lenz(count_p[1])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        } else if (countdown==1){
            if (strcmp(count_p[6][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[8])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[8])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[9])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[10])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[9])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[10])==1){
                move_left = rule7.moves - remaining_piece() - rule7.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule7.count++;
            } else if (lenz(count_p[7])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material"RESET);
                exit(0);
            }
        }
        if (move_left<=0){
            printf(RED"\nDRAW by counting rule"RESET);
            exit(0);
        }
        free(count_p);
        if (index=='0'){
            index = player2;
            if (checkmate('w')==-1){
                printf(CYAN"\nCHECKMATE !!\nPlayer 2 WIN !!"RESET);
                exit(0);
            } else if (checkmate('w')==0){
                printf(CYAN"\nSTALEMATE !!"RESET);
                exit(0);
            }
            if (checkK('w')){
                printf(RED"\nYou are in CHECK !!\n"RESET);
            }
            if (valid=='0'){
                if (strcmp(move, "h")==0){
                    Arr all_move = all_legal('w');
                    printf("\nPossible Move: ");
                    for (int k=0; strcmp(all_move[k], "69")!=0; k++){
                        printf("%s ", all_move[k]);
                    }
                    Free(all_move);
                    printf("\nPlayer1's move: ");
                } else {
                    printf("\nPlayer1's move: ");
                }
            } else {
                printf("\nInvalid move!\nPlayer1's move: ");
            }
            scanf("%s", move);
            sscanf(move, "%2s%2s", from, to);
            if (check_move(move)){
                if (strcmp(move, "z")==0){
                    //memcpy(board, board_states[board_states_size], size_b);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265F ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265F "RESET)==0){ 
                    Arr trey = trey_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, trey) && valueIn(to, all_move)){
                        change_color('w');
                        if (to[1] == '6'){
                            strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        } else {
                            strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265F "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        }
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0'; 
                        valid = '1';
                    }
                    Free(trey);
                    Free(temp);
                    Free(all_move);
                    //memcpy(board_states[board_states_size++], board, size_bb);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265E ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265F "RESET)==0){
                    Arr ses_move = ses_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ses_move) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265E "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(ses_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265D ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265E "RESET)==0){
                    Arr koul_move = koul_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, koul_move) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265D "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x.%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(koul_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265B ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265B "RESET)==0){
                    Arr neang_move = neang_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, neang_move) && valueIn(to, all_move)){
                        first_w_neang = 1;
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ",move);
                        index = '0';
                        valid = '1';
                    }
                    Free(neang_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265C ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265C "RESET)==0){
                    Arr tuuk = tuuk_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, tuuk) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265C "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ",move);
                        index = '0';
                        valid = '1';
                    }
                    Free(tuuk);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265A ")==0
                        || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265A "RESET)==0
                        || strcmp(board[from[1]-'1'][from[0]-'a'], RED"\u265A "RESET)==0){
                    Arr ang_move = ang_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ang_move) && valueIn(to, all_move)){
                        first_w_ang = 1;
                        change_color('w');                        
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265A "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(ang_move);
                    Free(temp);
                    Free(all_move);
                } else {
                    index = '0';
                    valid = '1';
                }
                Arr check_king = checkKing('b');
                if (strcmp(check_king[0], "69")!=0){
                    char x = check_king[0][2];
                    char y = check_king[0][3];
                    strcpy(board[y-'1'][x-'a'], PURPLE"\u265A "RESET);
                }
                Free(check_king);
            } else if (strcmp(move, "h")==0){
                index = '0';
                valid = '0';
            } else {
                index = '0';
                valid = '1';
            }
        } else {
            index = player1;
            if (checkmate('b')==-1){
                printf(CYAN"\nCHECKMATE !!\nPlayer 1 WIN !!"RESET);
                exit(0);
            } else if (checkmate('b')==0){
                printf(CYAN"\nSTALEMATE !!"RESET);
                exit(0);
            }
            if (checkK('b')){
                printf(RED"\nYou are in CHECK !!\n"RESET);
            }
            if (valid=='0'){
                if (strcmp(move, "h")==0){
                    Arr all_move = all_legal('b');
                    printf("\nPossible Move: ");
                    for (int k=0; strcmp(all_move[k], "69")!=0; k++){
                        printf("%s ", all_move[k]);
                    }
                    Free(all_move);
                    printf("\nPlayer2's move: ");
                } else {
                    printf("\nPlayer2's move: ");
                }
            } else {
                printf("\nInvalid move!\nPlayer2's move: ");
                valid = '0';
            }
            scanf("%s", move);
            sscanf(move, "%2s%2s", from, to);
            if (check_move(move)){
                if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265F "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265F "RESET)==0){
                    Arr trey = trey_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, trey) && valueIn(to, all_move)){
                        change_color('b');            
                        if (to[1] == '3'){
                            strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        } else {
                            strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265F "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        }
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(trey);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265E "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265E "RESET)==0){
                    Arr ses_move = ses_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ses_move) && valueIn(to, all_move)){
                        change_color('b');                
                        strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265E "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(ses_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265D "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265D "RESET)==0){
                    Arr koul_move = koul_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, koul_move) && valueIn(to, all_move)){
                        change_color('b');                        
                        strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265D "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(koul_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265B "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265B "RESET)==0){
                    Arr neang_move = neang_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, neang_move) && valueIn(to, all_move)){
                        first_b_neang = 1;
                        change_color('b');                        
                        strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(neang_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265C "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265C "RESET)==0){
                    Arr tuuk = tuuk_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, tuuk) && valueIn(to, all_move)){
                        change_color('b');                    
                        strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265C "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(tuuk);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265A "RESET)==0
                        || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265A "RESET)==0
                        || strcmp(board[from[1]-'1'][from[0]-'a'], PURPLE"\u265A "RESET)==0){
                    Arr ang_move = ang_can_move(from, 'b');
                    Arr temp = all_legal('b');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ang_move) && valueIn(to, all_move)){
                        first_b_ang = 1;
                        change_color('b');                        
                        strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265A "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                        fprintf(file, "%d/%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%s ", move);
                        index = '1';
                        valid = '1';
                    }
                    Free(ang_move);
                    Free(temp);
                    Free(all_move);
                } else {
                    index = '1';
                    valid = '1';
                }
                Arr check_king = checkKing('w');
                if (strcmp(check_king[0], "69")!=0){
                    char x = check_king[0][2];
                    char y = check_king[0][3];
                    strcpy(board[y-'1'][x-'a'], RED"\u265A "RESET);
                }
                Free(check_king);
            } else if (strcmp(move, "h")==0){
                index = '1';
                valid = '0';
            } else {
                index = '1';
                valid = '1';
            }
        } 
        display(board);
    } while (strcmp(move, "x") != 0); 
}
void split_line (char line[100], char arr[15][5]) {
    char *x = strtok(line, " ");
    int i = 0;
    while (x){
        strcpy(arr[i++], x);
        x = strtok(0, " ");
    }
}
void vsBot(FILE *file){
    FILE *fp = fopen("opening/black.txt", "r+");
    char index = '0';
    char player1 = '0';
    char player2 = '1';
    char valid = '0';
    Arr all_move;
    Arr all_move1;
    Arr all_move2;
    int countdown = 0, bot_count=0;
    int move_left=1;
    int indeX = 1;
    rule1.count = rule2.count = rule3.count = rule4.count = rule5.count = rule6.count = rule7.count = rule8.count = 0;
    rule1.moves=64, rule2.moves=8, rule3.moves=16, rule4.moves=22, rule5.moves=32, rule6.moves=44, rule7.moves=64, rule8.moves=64;
    clock_t tic;
    clock_t toc;
    Table board_state = copy_board();
    Data store_data;
    int data_count = 0;
    char opening[15][5];
    char line_move[100];
    memcpy(store_data[0], board_state, sizeof(store_data[0]));
    free(board_state);
    display(board);
    if (fp != NULL){
        int count = 1;
        int line = rand() % 5;
        while(fgets(line_move, 100, fp)!=NULL){
            if (line==count){
                break;
            } else {
                count++;
            }
        }
        fclose(fp);
    }
    split_line(line_move, opening);
    do {
        Table count_p = findPiece();
        if (strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0 
            && strcmp(count_p[4][0], "69")==0 && strcmp(count_p[8][0], "69")==0
            && strcmp(count_p[9][0], "69")==0 && strcmp(count_p[10][0], "69")==0){
            if ((lenz(count_p[0])+lenz(count_p[1]))<3 || (lenz(count_p[6])+lenz(count_p[7]))<3){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
            && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
            && strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0
            && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0){
            if (lenz(count_p[4])==1 || lenz(count_p[10])==1){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (countdown==0){
            if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
                && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
                && strcmp(count_p[4][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 1;
            } else if (strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0 
                && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0
                && strcmp(count_p[10][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 2;
            }  
        } else if (countdown==2) {
            if (strcmp(count_p[0][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[2])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[2])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[3])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[4])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[3])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[4])==1){
                move_left = rule7.moves - remaining_piece();
                printf(RED"\nMove left: %d\n"RESET, move_left - rule7.count);
                rule7.count++;
            } else if (lenz(count_p[1])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        } else if (countdown==1){
            if (strcmp(count_p[6][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[8])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[8])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[9])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[10])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[9])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[10])==1){
                move_left = rule7.moves - remaining_piece() - rule7.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule7.count++;
            } else if (lenz(count_p[7])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material"RESET);
                exit(0);
            }
        }
        if (move_left<=0){
            printf(RED"\nDRAW by counting rule"RESET);
            exit(0);
        }
        free(count_p);
        if (index=='0'){
            index = player2;
            if (checkmate('w')==-1){
                printf(CYAN"\nCHECKMATE !!\nThe Bot WIN !!\n"RESET);
                exit(0);
            } else if (checkmate('w')==0){
                printf(CYAN"\nSTALEMATE !!\n"RESET);
                exit(0);
            }
            if (checkK('w')){
                printf(RED"\nYou are in CHECK !!\n"RESET);
            }
            
            if (valid=='0'){
                if (strcmp(move, "NULL")==0){
                    printf("\nYour move: ");
                } else if (strcmp(move, "h")==0){
                    Arr all_move = all_legal('w');
                    printf("\nPossible Move: ");
                    for (int k=0; strcmp(all_move[k], "69")!=0; k++){
                        printf("%s ", all_move[k]);
                    }
                    Free(all_move);
                    printf("\nYour move: ");
                } else {
                    printf("\nBot's move: %s", move);
                    printf("\nTime took: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
                    printf("\nYour move: ");
                }
            } else {
                printf("\nInvalid move!\nYour move: ");
            }
            scanf("%s", move);
            if (strcmp(move, "z")==0){
                int back = data_count - 1;
                printf("%d", back);
                if (back<=0){
                    memcpy(board, store_data[0], sizeof(board));
                    strcpy(move, "NULL");
                    index = '0';
                } else {
                    memcpy(board, store_data[back], sizeof(board));
                    strcpy(move, "NULL");
                    index = '0';
                }
                if (data_count>1){
                    data_count--;
                }
            } else if (check_move(move)){
                sscanf(move, "%2s%2s", from, to);
                if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265F ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265F "RESET)==0){
                    Arr trey = trey_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, trey) && valueIn(to, all_move)){
                        change_color('w');
                        if (to[1] == '6'){
                            strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        } else {
                            strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265F "RESET);
                            strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        }
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0'; 
                        valid = '1';
                    }
                    Free(trey);
                    Free(temp);
                    Free(all_move);
                    //memcpy(board_states[board_states_size++], board, size_bb);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265E ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265E "RESET)==0){
                    Arr ses_move = ses_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ses_move) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265E "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "%d.%s ", indeX, move);
                        index = '0';
                        valid = '1';
                    }
                    Free(ses_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265D ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265D "RESET)==0){
                    Arr koul_move = koul_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, koul_move) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265D "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(koul_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265B ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265B "RESET)==0){
                    Arr neang_move = neang_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, neang_move) && valueIn(to, all_move)){
                        first_w_neang = 1;
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(neang_move);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265C ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265C "RESET)==0){
                    Arr tuuk = tuuk_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, tuuk) && valueIn(to, all_move)){
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265C "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(tuuk);
                    Free(temp);
                    Free(all_move);
                } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265A ")==0 
                            || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265A "RESET)==0
                            || strcmp(board[from[1]-'1'][from[0]-'a'], RED"\u265A "RESET)==0){
                    Arr ang_move = ang_can_move(from, 'w');
                    Arr temp = all_legal('w');
                    Arr all_move = sub1(temp);
                    if (valueIn(to, ang_move) && valueIn(to, all_move)){
                        first_w_ang = 1;
                        change_color('w');
                        strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265A "RESET);
                        strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                        fprintf(file, "%d.%s ", indeX, move);
                        indeX++;
                        valid = '0';
                    } else {
                        fprintf(file, "x%s ", move);
                        index = '0';
                        valid = '1';
                    }
                    Free(ang_move);
                    Free(temp);
                    Free(all_move);

                } else {
                    index = '0';
                    valid = '1';
                }
                Arr check_king = checkKing('b');
                if (strcmp(check_king[0], "69")!=0){
                    char x = check_king[0][2];
                    char y = check_king[0][3];
                    strcpy(board[y-'1'][x-'a'], PURPLE"\u265A "RESET);
                }
                Free(check_king);
            } else if (strcmp(move, "h")==0){
                index = '0';
                valid = '0';
            } else {
                index = '0';
                valid = '1';
            }
        } else {
            index = player1;
            if (checkmate('b')==-1){
                printf(CYAN"\nCHECKMATE !!\nYou WIN !!"RESET);
                exit(0);
            } else if (checkmate('b')==0){
                printf(CYAN"\nSTALEMATE !!"RESET);
                exit(0);
            }
            Arr all = all_legal('b');
            if (valueIn(opening[bot_count], all)){
                strcpy(move, opening[bot_count++]);
                Free(all);
            } else {
                int score = eval_board(1);
                printf("\nYour move: %s", move);
                printf("\nBot's thinking...\n");
                Table count_p = findPiece();
                char *m = malloc(5 * sizeof(char));
                tic = clock();
                if (score <=-2000 && score >-2500){
                    m = selectMove(4, -6969, 6969, 'b', 1);
                } else if (score <=-2500){
                    m = selectMove(5, -6969, 6969, 'b', 1);
                } else {
                    m = selectMove(3, -6969, 6969, 'b', 1);
                }
                strcpy(move, m);
                free(m);
                free(count_p);
            }
            toc = clock();
            fprintf(file, "%d/%s ", indeX, move);
            indeX++;
            sscanf(move, "%2s%2s", from, to);
            if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265F "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265F "RESET)==0){
                change_color('b');
                if (to[1] == '3'){
                    strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                } else {
                    strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265F "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                }
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265E "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265E "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265E "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265D "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265D "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265D "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265B "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265B "RESET)==0){
                first_b_neang = 1;
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265C "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265C "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265C "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
    
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265A "RESET)==0 
                    || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265A "RESET)==0
                    || strcmp(board[from[1]-'1'][from[0]-'a'], PURPLE"\u265A "RESET)==0){
                first_b_ang = 1;
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265A "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
            }
            Arr check_king = checkKing('w');
            if (strcmp(check_king[0], "69")!=0){
                char x = check_king[0][2];
                char y = check_king[0][3];
                strcpy(board[y-'1'][x-'a'], RED"\u265A "RESET);
            }
            Free(check_king);
            board_state = copy_board();
            data_count++;
            memcpy(store_data[data_count], board_state, sizeof(store_data[0]));
            free(board_state);
        }
        display(board);
    } while (strcmp(move, "x") != 0); 
}
void botFun(FILE *file){
    FILE *f2 = fopen("opening/black.txt", "r+");
    FILE *f1 = fopen("opening/white.txt", "r+");
    char index = '0';
    char player1 = '0';
    char player2 = '1';
    char valid = '0';
    int indeX = 1;
    int move_left = 1;
    rule1.count = rule2.count = rule3.count = rule4.count = rule5.count = rule6.count = rule7.count = rule8.count = 0;
    rule1.moves=64, rule2.moves=8, rule3.moves=16, rule4.moves=22, rule5.moves=32, rule6.moves=44, rule7.moves=64, rule8.moves=64;
    int countdown=0, bot1_count=0, bot2_count=0;
    clock_t tic;
    clock_t toc;
    char opening1[15][5];
    char line_move1[100];
    char opening2[15][5];
    char line_move2[100];
    int op1=0, op2=0;
    if (f1 != NULL){
        int count = 1;
        int line = rand() % 5;
        while(fgets(line_move1, 100, f1)!=NULL){
            if (line==count){
                break;
            } else {
                count++;
            }
        }
        fclose(f1);
    }
    split_line(line_move1, opening1);
    if (f2 != NULL){
        int count = 1;
        int line = rand() % 5;
        while(fgets(line_move2, 100, f2)!=NULL){
            if (line==count){
                break;
            } else {
                count++;
            }
        }
        fclose(f2);
    }
    split_line(line_move2, opening2);
    display(board);
    do {
        Table count_p = findPiece();
        if (strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0 
            && strcmp(count_p[4][0], "69")==0 && strcmp(count_p[8][0], "69")==0
            && strcmp(count_p[9][0], "69")==0 && strcmp(count_p[10][0], "69")==0){
            if ((lenz(count_p[0])+lenz(count_p[1]))<3 || (lenz(count_p[6])+lenz(count_p[7]))<3){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
            && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
            && strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0
            && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0){
            if (lenz(count_p[4])==1 || lenz(count_p[10])==1){
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        }
        if (countdown==0){
            if (strcmp(count_p[0][0], "69")==0 && strcmp(count_p[1][0], "69")==0 
                && strcmp(count_p[2][0], "69")==0 && strcmp(count_p[3][0], "69")==0
                && strcmp(count_p[4][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 1;
            } else if (strcmp(count_p[6][0], "69")==0 && strcmp(count_p[7][0], "69")==0 
                && strcmp(count_p[8][0], "69")==0 && strcmp(count_p[9][0], "69")==0
                && strcmp(count_p[10][0], "69")==0){
                printf(RED"Counting rule started\n"RESET);
                countdown = 2;
            }  
        } else if (countdown==2) {
            if (strcmp(count_p[0][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[2])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[2])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[3])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[4])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[3])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[4])==1){
                move_left = rule7.moves - remaining_piece();
                printf(RED"\nMove left: %d\n"RESET, move_left - rule7.count);
                rule7.count++;
            } else if (lenz(count_p[1])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material\n"RESET);
                exit(0);
            }
        } else if (countdown==1){
            if (strcmp(count_p[6][0], "69")!=0){
                move_left = rule1.moves - remaining_piece() - rule1.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule1.count++;
            } else if (lenz(count_p[8])==2){
                move_left = rule2.moves - remaining_piece() - rule2.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule2.count++;
            } else if (lenz(count_p[8])==1){
                move_left = rule3.moves - remaining_piece() - rule3.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule3.count++;
            } else if (lenz(count_p[9])==2){
                move_left = rule4.moves - remaining_piece() - rule4.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule4.count++;
            } else if (lenz(count_p[10])==2){
                move_left = rule5.moves - remaining_piece() - rule5.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule5.count++;
            } else if (lenz(count_p[9])==1){
                move_left = rule6.moves - remaining_piece() - rule6.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule6.count++;
            } else if (lenz(count_p[10])==1){
                move_left = rule7.moves - remaining_piece() - rule7.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule7.count++;
            } else if (lenz(count_p[7])>=3){
                move_left = rule8.moves - remaining_piece() - rule8.count;
                printf(RED"\nMove left: %d\n"RESET, move_left);
                rule8.count++;
            } else {
                printf(CYAN"\nDRAW by insufficient material"RESET);
                exit(0);
            }
        }
        if (move_left<=0){
            printf(RED"\nDRAW by counting rule"RESET);
            exit(0);
        }
        free(count_p);
        if (index=='0'){
            index = player2;
            if (checkmate('w')==-1){
                printf(CYAN"\nCHECKMATE !!\nBot2 WIN !!"RESET);
                exit(0);
            } else if (checkmate('w')==0){
                printf(CYAN"\nSTALEMATE !!"RESET);
                exit(0);
            }
            if (strcmp(move, "NULL")==0){
                printf("\nBot1's thinking...\n");
            } else {
                printf("\nBot2's move: %s\n", move);
                printf("Time took: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
                printf("Bot1's thinking...\n");
            }
            tic = clock();
            Arr all = all_legal('w');
            if (valueIn(opening1[op1], all)){
                strcpy(move, opening1[op1++]);
                Free(all);
                #ifdef _WIN32 
                    Sleep(1);
                #else
                    sleep(1);
                #endif
            } else {
                char *mm = selectMove(3, -7000, 7000, 'w', 2);
                strcpy(move, mm);
                bot1_count++;
                if (bot1_count%2==1){
                    if (strcmp(rep_move1, move)!=0){
                        strcpy(rep_move1, move);
                    } else {
                        Arr allLegal = all_legal('w');
                        Arr priority = all_priority(allLegal, 'b');
                        int indexx = 0;
                        if (strcmp(priority[0], "69")!=0){
                            int max = mvvLVA(priority[0]);
                            for (int i=0; strcmp(priority[i], "69")!=0; i++){
                                max = MAX(max, mvvLVA(priority[i]));
                                if (max==mvvLVA(priority[i])){
                                    indexx = i;
                                }
                            }
                            strcpy(move, priority[indexx]);
                        } else {
                            int len = lens(allLegal);
                            int r = rand() % len;
                            strcpy(move, allLegal[r]);
                        }
                        Free(allLegal);
                        Free(priority);
                        free(mm);
                    }
            
                }
            }
            toc = clock();
            fprintf(file, "%d/%s ", indeX, move);
            indeX++;
            sscanf(move, "%2s%2s", from, to);
            if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265F ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265F "RESET)==0){
                change_color('w');
                if (to[1] == '6'){
                    strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                } else {
                    strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265F "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                }
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265E ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265E "RESET)==0){
                change_color('w');
                strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265E "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265D ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265D "RESET)==0){
                change_color('w');
                strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265D "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265B ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265B "RESET)==0){
                first_b_neang = 1;
                change_color('w');
                strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265B "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265C ")==0 || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265C "RESET)==0){
                change_color('w');
                strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265C "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
    
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], "\u265A ")==0 
                    || strcmp(board[from[1]-'1'][from[0]-'a'], YELLOW"\u265A "RESET)==0
                    || strcmp(board[from[1]-'1'][from[0]-'a'], RED"\u265A "RESET)==0){
                first_b_ang = 1;
                change_color('w');
                strcpy(board[to[1]-'1'][to[0]-'a'], YELLOW"\u265A "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], YELLOW"\u26CC "RESET);
            }
            Arr check_king = checkKing('b');
            if (strcmp(check_king[0], "69")!=0){
                char x = check_king[0][2];
                char y = check_king[0][3];
                strcpy(board[y-'1'][x-'a'], PURPLE"\u265A "RESET);
            }
            Free(check_king);
        } else {
            index = player1;
            if (checkmate('b')==-1){
                printf(CYAN"\nCHECKMATE !!\nBot1 WIN !!"RESET);
                exit(0);
            } else if (checkmate('b')==0){
                printf(CYAN"\nSTALEMATE !!"RESET);
                exit(0);
            }
            printf("\nBot1's move: %s\n", move);
            printf("Time took: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
            printf("Bot2's thinking...\n");
            tic = clock();
            Arr all = all_legal('b');
            if (valueIn(opening2[op2], all)){
                strcpy(move, opening2[op2++]);
                Free(all);
                #ifdef _WIN32 
                    Sleep(1);
                #else
                    sleep(1);
                #endif
            } else {
                char *m = selectMove(3, -7000, 7000, 'b', 1);
                strcpy(move, m);
                bot2_count++;
                if (bot2_count%2==1){
                    if (strcmp(rep_move2, move)!=0){
                        strcpy(rep_move2, move);
                    } else {
                        Arr allLegal = all_legal('w');
                        Arr priority = all_priority(allLegal, 'b');
                        int indexx = 0;
                        if (strcmp(priority[0], "69")!=0){
                            int max = mvvLVA(priority[0]);
                            for (int i=0; strcmp(priority[i], "69")!=0; i++){
                                max = MAX(max, mvvLVA(priority[i]));
                                if (max==mvvLVA(priority[i])){
                                    indexx = i;
                                }
                            }
                            strcpy(move, priority[indexx]);
                        } else {
                            int len = lens(allLegal);
                            int r = rand() % len;
                            strcpy(move, allLegal[r]);
                        }
                        Free(allLegal);
                        Free(priority);
                        free(m);
                    }
                }
            }
            toc = clock();
            fprintf(file, "%d/%s ", indeX, move);
            indeX++;
            sscanf(move, "%2s%2s", from, to);
            if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265F "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265F "RESET)==0){
                change_color('b');
                if (to[1] == '3'){
                    strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                } else {
                    strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265F "RESET);
                    strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                }
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265E "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265E "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265E "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265D "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265D "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265D "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265B "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265B "RESET)==0){
                first_b_neang = 1;
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265B "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
                
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265C "RESET)==0 || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265C "RESET)==0){
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265C "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
    
            } else if (strcmp(board[from[1]-'1'][from[0]-'a'], GREEN"\u265A "RESET)==0 
                    || strcmp(board[from[1]-'1'][from[0]-'a'], CYAN"\u265A "RESET)==0
                    || strcmp(board[from[1]-'1'][from[0]-'a'], PURPLE"\u265A "RESET)==0){
                first_b_ang = 1;
                change_color('b');
                strcpy(board[to[1]-'1'][to[0]-'a'], CYAN"\u265A "RESET);
                strcpy(board[from[1]-'1'][from[0]-'a'], CYAN"\u26CC "RESET);
            }
            Arr check_king = checkKing('w');
            if (strcmp(check_king[0], "69")!=0){
                char x = check_king[0][2];
                char y = check_king[0][3];
                strcpy(board[y-'1'][x-'a'], RED"\u265A "RESET);
            }
            Free(check_king);
        }
        display(board);
    } while (strcmp(move, "x") != 0); 
}
