#include<iostream>
#include<string.h>
using namespace std;

char c[85];

bool search_th(){
  bool f = false;
  for (int i = 0; i < strlen(c); i++){
    if ((c[i] == 't' && c[i+1] == 'h' && c[i+2] == 'e') ||
        (c[i] == 't' && c[i+1] == 'h' && c[i+2] == 'i' && c[i+3] == 's') ||
        (c[i] == 't' && c[i+1] == 'h' && c[i+2] == 'a' && c[i+3] == 't')){
          f = true;  break;
    }
  }
  return f;
}

int main(){
  while (cin.getline(c, sizeof(c))){
    while (1){
      if (search_th() == true){
      　cout << c << endl;
      　for (int i = 0; i < 85; i++){
      　　c[i] = '\0';
    　　}
     　 break;
      }
      for (int i = 0; i < strlen(c); i++){
        if (c[i] >= 'a' && c[i] <= 'y'){
          c[i]++;
        } else if (c[i] == 'z'){
          c[i] = 'a';
        }
      }
    }
  }
  return 0;
}
