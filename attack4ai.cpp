#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<list>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<string>
#include<utility>
#include<algorithm>
#include<functional>
#include<deque>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include<stdlib.h>
using namespace std;

enum f_c {Finish = 3, Continue = 4};
#define Mark_1 100
#define Mark_2 200

enum f_c player_turn_computer();
void expect();
void rule();
void set_v();
void rand_rand();
char disp_convert(int, int);
void disp_board();
int choose_number();
bool check_7_over(int);
bool check_4_line();
bool check_fulled();
enum f_c player_1();
enum f_c player_2();
enum f_c player_turn_human();
enum f_c after_choose_number(int);
int GetRandom(int, int);
void do_step(string);
bool check_put_able(int, int);
bool check_reach(int, int, int);
bool check_absolute_win(int);
void strategy();
void check_3_line_urge(int);
void check_3_line();


int v[9][9];  //0と8は周囲として用意しておく
int v_cp[9][9];
int ex[8] = {0};  //期待値(exp[0]は使わない)
int vd_1[9][9];
int vd_2[9][9];
int cnt_3_line_vd_1 = 0;  //リーチの数
int cnt_3_line_vd_2 = 0;
int player;
int number_com;
bool decide_com;
int stage = 1;


int main(){
  rule();
  set_v();
  rand_rand();
  enum f_c judge = Continue;
  while (judge == Continue){
    judge = player_1();
    if (judge == Continue){
      judge = player_2();
    }
  }
  return 0;
}


enum f_c player_turn_computer(){
  disp_board();
  printf(">>Player 2: Computer\n");
  printf("Please choose a number(1 ~ 7) : ");
  number_com = -1;
  decide_com = false;  //number_comが決まったらtrueに変える
  check_3_line();
  if (decide_com == false){
    check_3_line_urge(2);
  }
  if (decide_com == false){
    check_3_line_urge(1);
  }
  if (decide_com == false){
    /////strategy();
  }
  if (decide_com == false){
    expect();
  }
  while (decide_com == false){
    number_com = GetRandom(1, 7);
    if (check_7_over(number_com) == true) break;
  }
  return after_choose_number(number_com);
}


void expect(){
  stage = 1;
  queue<string> que;
  que.push("1");
  que.push("2");
  que.push("3");
  que.push("4");
  que.push("5");
  que.push("6");
  que.push("7");
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      v_cp[i][j] = v[i][j];
    }
  }
  while (decide_com == false && stage <= 4){
    for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
        v[i][j] = v_cp[i][j];
      }
    }
    do_step(que.front());
    if (v[1][1] == -1) break;  //7_overの場合
    if (check_absolute_win(1) == true) {
      string ss = que.front();
      printf("del %c\n", ss[0]);
      break;
    }
    if (check_absolute_win(2) == true){
      string s = que.front();
      number_com = s[0]-'0';
      decide_com = true;
      cout << "Absolute win" << endl;
      break;
    }


    que.pop();
    string str1 = que.front()+"1";
    que.push(str1);
    string str2 = que.front()+"2";
    que.push(str2);
    string str3 = que.front()+"3";
    que.push(str3);
    string str4 = que.front()+"4";
    que.push(str4);
    string str5 = que.front()+"5";
    que.push(str5);
    string str6 = que.front()+"6";
    que.push(str6);
    string str7 = que.front()+"7";
    que.push(str7);

    printf("stage %d: completed\n", stage++);
  }
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      v[i][j] = v_cp[i][j];
    }
  }
}


void rule(){
  printf("\nWelcome to Attack 4 !!\n\n");
  printf("~ Rule ~\n");
  printf("1. Two players(you & computer) take turns choosing a number(1 ~ 7).\n");
  printf("2. '#' is put in your selected line.   '+' is computer's.\n");
  printf("3. If you fill in 4 squares in a vertical, horizontal, or diagonal row, you are the winner!\n\n");
  printf("    #                                         #        #           \n");
  printf("    #                                      #              #        \n");
  printf("    #      or    #  #  #  #    or       #         or         #     \n");
  printf("    #                                #                          #  \n\n");
  printf("Then let's enjoy a game!!\n\n");
}


void set_v(){
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 7; j++){
      v[i][j] = 10 * i + j;
    }
  }
  for (int i = 0; i <= 8; i++){
    v[0][i] = -1;
    v[8][i] = -1;
    v[i][0] = -1;
    v[i][8] = -1;
  }
}


void rand_rand(){
  clock_t start = clock();
  printf("Please push Enter key...\n");
  cin.ignore();
  clock_t end = clock();
  for (int i = 0; i < (end-start)/10; i++){
    GetRandom(1, 7);
  }
}


char disp_convert(int i, int j){
  if (v[i][j] == Mark_1){
    return '#';
  } else if (v[i][j] == Mark_2){
    return '+';
  } else {
    return ' ';
  }
}


void disp_board(){
  puts("");
  printf("  ");
  for (int i = 1; i <= 7; i++){
    printf("%d   ", i);
  }
  puts("");
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 7; j++){
      printf("| %c ", disp_convert(i, j));
    }
    puts("|");
  }
  printf("\n\n");
}


int choose_number(){
  char char_choose[50];
  int choose;
  bool f = true;
  do {
    switch (player){
      case 1: printf(">>Player 1: You\n");  break;
      case 2: printf(">>Player 2: Computer\n");  break;
    }
    printf("Please choose a number(1 ~ 7) : ");
    cin.getline(char_choose, sizeof(char_choose));
    if (strlen(char_choose) != 1){
      printf("Error!\n");
      f = false;
    } else {
      choose = char_choose[0]-'0';
      if (!(char_choose[0] >= '1' && char_choose[0] <= '7') || check_7_over(choose) == false){
        printf("Error!\n");
        f = false;
      } else {
        f = true;
      }
    }
  } while (f == false);
  return choose;
}


//choose_numberで選択した列が既に7個埋まっているときにfalseを返す
bool check_7_over(int choose){
  if (v[1][choose] == Mark_1 || v[1][choose] == Mark_2) return false;
  else return true;
}


//4つ連続のラインが完成していればtrueを返す
bool check_4_line(){
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j] == v[i][j+1] && v[i][j+1] == v[i][j+2] && v[i][j+2] == v[i][j+3]){
        return true;
      } else if (v[j][i] == v[j+1][i] && v[j+1][i] == v[j+2][i] && v[j+2][i] == v[j+3][i]){
        return true;
      }
    }
  }
  for (int i = 1; i <= 4; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j] == v[i+1][j+1] && v[i+1][j+1] == v[i+2][j+2] && v[i+2][j+2] == v[i+3][j+3]){
        return true;
      } else if (v[i][j+3] == v[i+1][j+2] && v[i+1][j+2] == v[i+2][j+1] && v[i+2][j+1] == v[i+3][j]){
        return true;
      }
    }
  }
  return false;
}


//全てのマスが埋まっていればtrueを返す
bool check_fulled(){
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 7; j++){
      if (v[i][j] != Mark_1 && v[i][j] != Mark_2){
        return false;
      }
    }
  }
  return true;
}


enum f_c player_1(){
  player = 1;
  return player_turn_human();
}


enum f_c player_2(){
  player = 2;
  return player_turn_computer();
}


enum f_c player_turn_human(){
  disp_board();
  int number = choose_number();
  return after_choose_number(number);
}


enum f_c after_choose_number(int number){
  enum f_c judge = Continue;
  for (int i = 7; i >= 1; i--){
    if (v[i][number] != Mark_1 && v[i][number] != Mark_2){
      switch (player){
        case 1: v[i][number] = Mark_1;  break;
        case 2: v[i][number] = Mark_2;  break;
      }
      break;
    }
  }
  if (check_4_line() == true){
    disp_board();
    printf("4-line is achieved!!!\n");
    switch (player) {
      case 1: printf("Winner : You!!\n\n");  break;
      case 2: printf("Winner : Computer!!\n\n");  break;
    }
    judge = Finish;
  }
  if (check_fulled() == true){
    disp_board();
    printf("Finish...Draw...\n");
    printf("Please play this game once again!\n\n");
    judge = Finish;
  }
  return judge;
}


//GetRandom(a, b)で、a～bのランダム値を生成する
int GetRandom(int min, int max){
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}


void do_step(string str){
  int step;
  player = 2;
  for (int i = 0; i < (signed)str.length(); i++){
    step = str[i]-'0';
    if (check_7_over(step) == false){
      v[1][1] = -1;  //異常として知らせる
    }
    for (int j = 7; j >= 1; j--){
      if (v[j][step] != Mark_1 && v[j][step] != Mark_2){
        switch (player){
          case 1: v[j][step] = Mark_1;  player = 2;  break;
          case 2: v[j][step] = Mark_2;  player = 1;  break;
        }
        break;
      }
    }
  }
  player = 2;
}


//v[i][j]にput inできるならばtrueを返す
bool check_put_able(int i, int j){
  if (i >= 1 && i <= 6 && j >= 1 && j <= 7){
    if (v[i][j] != Mark_1 && v[i][j] != Mark_2 && (v[i+1][j] == Mark_1 || v[i+1][j] == Mark_2)){
      return true;
    }
  } else if (i == 7 && j >= 1 && j <= 7){
    return true;
  }
  return false;
}


//完全リーチであればtrueを返す
bool check_reach(int i, int j, int player){
  switch (player){
    case 1: if (vd_1[i][j] >= 1 && check_put_able(i, j) == true){
              return true;
            } else {
              return false;
            }
    case 2: if (vd_2[i][j] >= 1 && check_put_able(i, j) == true){
              return true;
            } else {
              return false;
            }
  }
  return false;
}


//必勝が確定すればtrueを返す
bool check_absolute_win(int player){
  bool f = false;
  int cnt = 0;
  check_3_line();
  switch (player){
    case 1: 
            if (cnt_3_line_vd_1 >= 1){
              for (int j = 7; j >= 1; j--){
                for (int i = 1; i <= 7; i++){
                  if (vd_1[i][j] >= 1 && check_put_able(i, j) == true){
                    cnt++;
                    if (vd_1[i][j-1] != 0){
                      f = true;  break;
                    }
                  }
                }
              }
            }
            if (cnt >= 2) f = true;
            for (int i = 1; i <= 7; i++){
              for (int j = 1; j <= 7; j++){
                if (check_reach(i, j, 1) == true){
                  f = true;  break;
                }
              }
            }
            return f;
    case 2: 
            if (cnt_3_line_vd_2 >= 1){
              for (int j = 7; j >= 1; j--){
                for (int i = 1; i <= 7; i++){
                  if (vd_2[i][j] >= 1 && check_put_able(i, j) == true){
                    cnt++;
                    if (vd_2[i][j-1] != 0){
                      f = true;  break;
                    }
                  }
                }
              }
            }
            if (cnt >= 2) f = true;
            for (int i = 1; i <= 7; i++){
              for (int j = 1; j <= 7; j++){
                if (check_reach(i, j, 2) == true){
                  f = true;  break;
                }
              }
            }
            return f;
  }
  return false;
}


void strategy(){
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 3; j++){
      if (v[i][j] != Mark_1 && v[i][j] != Mark_2 && v[i][j+1] == v[i][j+2] && v[i][j+3] != Mark_1 && v[i][j+3] != Mark_2
      && v[i][j+4] != Mark_1 && v[i][j+4] != Mark_2 && check_put_able(i, j) == true && check_put_able(i, j+3) == true && check_put_able(i, j+4) == true){
        number_com = j+3;
        decide_com = true;
      }
      if (v[i][j] != Mark_1 && v[i][j] != Mark_2 && v[i][j+1] != Mark_1 && v[i][j+1] != Mark_2 && v[i][j+2] == v[i][j+3]
      && v[i][j+4] != Mark_1 && v[i][j+4] != Mark_2 && check_put_able(i, j) == true && check_put_able(i, j+3) == true && check_put_able(i, j+4) == true){
        number_com = j+1;
        decide_com = true;
      }
      if (v[i][j] != Mark_1 && v[i][j] != Mark_2 && v[i][j+2] != Mark_1 && v[i][j+2] != Mark_2 && v[i][j+4] != Mark_1 && v[i][j+4] != Mark_2
      && v[i][j+1] == v[i][j+3] && check_put_able(i, j) == true && check_put_able(i, j+2) == true && check_put_able(i, j+4) == true){
        number_com = j+2;
        decide_com = true;
      }
    }
  }
}


void check_3_line_urge(int player){
  switch (player){
    case 1: if (cnt_3_line_vd_1 >= 1){
              for (int i = 1; i <= 7; i++){
                for (int j = 1; j <= 7; j++){
                  if (vd_1[i][j] != 0 && check_put_able(i, j) == true && check_7_over(j) == true){
                    number_com = j;
                    decide_com = true;
                    break;
                  }
                }
              }
            }
            break;
    case 2: if (cnt_3_line_vd_2 >= 1){
              for (int i = 1; i <= 7; i++){
                for (int j = 1; j <= 7; j++){
                  if (vd_2[i][j] != 0 && check_put_able(i, j) == true && check_7_over(j) == true){
                    number_com = j;
                    decide_com = true;
                    break;
                  }
                }
              }
            }
            break;
  }
}


void check_3_line(){
  cnt_3_line_vd_1 = 0;
  cnt_3_line_vd_2 = 0;
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 7; j++){
      vd_1[i][j] = 0;
      vd_2[i][j] = 0;
    }
  }
  for (int i = 0; i <= 8; i++){
    vd_1[0][i] = -1;  vd_1[i][0] = -1;  vd_1[8][i] = -1;  vd_1[i][8] = -1;
    vd_2[0][i] = -1;  vd_2[i][0] = -1;  vd_2[8][i] = -1;  vd_2[i][8] = -1;
  }
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j+1] == v[i][j+2] && v[i][j+2] == v[i][j+3] && v[i][j] != Mark_1 && v[i][j] != Mark_2){
        switch (v[i][j+1]){
          case Mark_1: vd_1[i][j]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j] == v[i][j+1] && v[i][j+1] == v[i][j+2] && v[i][j+3] != Mark_1 && v[i][j+3] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i][j+3]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j+3]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[j+1][i] == v[j+2][i] && v[j+2][i] == v[j+3][i] && v[j][i] != Mark_1 && v[j][i] != Mark_2){
        switch (v[j+1][i]){
          case Mark_1: vd_1[j][i]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[j][i]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[j][i] == v[j+1][i] && v[j+1][i] == v[j+2][i] && v[j+3][i] != Mark_1 && v[j+3][i] != Mark_2){
        switch (v[j][i]){
          case Mark_1: vd_1[j+3][i]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[j+3][i]++;  cnt_3_line_vd_2++;  break;
        }
      }
    }
  }
  for (int i = 1; i <= 4; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j] == v[i+1][j+1] && v[i+1][j+1] == v[i+2][j+2] && v[i+3][j+3] != Mark_1 && v[i+3][j+3] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i+3][j+3]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+3][j+3]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i+1][j+1] == v[i+2][j+2] && v[i+2][j+2] == v[i+3][j+3] && v[i][j] != Mark_1 && v[i][j] != Mark_2){
        switch (v[i+1][j+1]){
          case Mark_1: vd_1[i][j]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i+1][j+2] == v[i+2][j+1] && v[i+2][j+1] == v[i+3][j] && v[i][j+3] != Mark_1 && v[i][j+3] != Mark_2){
        switch (v[i+1][j+2]){
          case Mark_1: vd_1[i][j+3]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j+3]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j+3] == v[i+1][j+2] && v[i+1][j+2] == v[i+2][j+1] && v[i+3][j] != Mark_1 && v[i+3][j] != Mark_2){
        switch (v[i][j+3]){
          case Mark_1: vd_1[i+3][j]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+3][j]++;  cnt_3_line_vd_2++;  break;
        }
      }
    }
  }
  for (int i = 1; i <= 7; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j] == v[i][j+1] && v[i][j+1] == v[i][j+3] && v[i][j+2] != Mark_1 && v[i][j+2] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i][j+2]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j+2]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j] == v[i][j+2] && v[i][j+2] == v[i][j+3] && v[i][j+1] != Mark_1 && v[i][j+1] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i][j+1]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i][j+1]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[j][i] == v[j+1][i] && v[j+1][i] == v[j+3][i] && v[j+2][i] != Mark_1 && v[j+2][i] != Mark_2){
        switch (v[j][i]){
          case Mark_1: vd_1[j+2][i]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[j+2][i]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[j][i] == v[j+2][i] && v[j+2][i] == v[j+3][i] && v[j+1][i] != Mark_1 && v[j+1][i] != Mark_2){
        switch (v[j][i]){
          case Mark_1: vd_1[j+1][i]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[j+1][i]++;  cnt_3_line_vd_2++;  break;
        }
      }
    }
  }
  for (int i = 1; i <= 4; i++){
    for (int j = 1; j <= 4; j++){
      if (v[i][j] == v[i+1][j+1] && v[i+1][j+1] == v[i+3][j+3] && v[i+2][j+2] != Mark_1 && v[i+2][j+2] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i+2][j+2]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+2][j+2]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j] == v[i+2][j+2] && v[i+2][j+2] == v[i+3][j+3] && v[i+1][j+1] != Mark_1 && v[i+1][j+1] != Mark_2){
        switch (v[i][j]){
          case Mark_1: vd_1[i+1][j+1]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+1][j+1]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j+3] == v[i+1][j+2] && v[i+1][j+2] == v[i+3][j] && v[i+2][j+1] != Mark_1 && v[i+2][j+1] != Mark_2){
        switch (v[i][j+3]){
          case Mark_1: vd_1[i+2][j+1]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+2][j+1]++;  cnt_3_line_vd_2++;  break;
        }
      }
      if (v[i][j+3] == v[i+2][j+1] && v[i+2][j+1] == v[i+3][j] && v[i+1][j+2] != Mark_1 && v[i+1][j+2] != Mark_2){
        switch (v[i][j+3]){
          case Mark_1: vd_1[i+1][j+2]++;  cnt_3_line_vd_1++;  break;
          case Mark_2: vd_2[i+1][j+2]++;  cnt_3_line_vd_2++;  break;
        }
      }
    }
  }
}
