/* 
 Total.c

 CS 223b, hw1  
 See http://zoo.cs.yale.edu/classes/cs223/current/s17h1.v0.html

 Name: <Conor Brennan>
 netid: <cb687>

 start with Total.c: simple C program: counts characters from input
 
 We have included some useful libraries.

 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define ungetchar(c)  ungetc(c, stdin)    // unread char read from stdin


// here is where you can gobble up line splices
int mygetchar() {
  int ch = getchar();
  // logic to ignore backslashes.  If backslash encountered, skip the following character
  if (ch == '\\'){
    while (ch == '\\'){
      ch = getchar();
      ch = getchar();
    }
  }
  // logic for dealing with quotations.  if a quotation is encountered, the function enters a loop
  // until it reaches another quotation - unless this quotation is preceded by a backslash in which case
  // it is ignored
  if(ch == '"'){
    ch = getchar();
    while(ch != '"'){
      if (ch == EOF){
        exit(0);
      }
      if(ch == '\\'){
        ch = getchar();
        if(ch == '"'){
          ch = getchar();
        }
      }
    ch = getchar();
    }
  }
  return ch;
}

int main(int argc, char **argv) {
  // variables to store total, current character, the char array, and two conter variables to keep track of
  // the position in the array and whether or not a negative number is encountered
  long total = 0;
  int c;
  char s[30];
  int j = 0;
  int negative = 0;
  while ((c = mygetchar()) != EOF){
    for (int i = 0; i < 30; i++){
      s[i] = ' ';
    }
    j = 0;
    // if a 0 is encountered, the program then checks the next character to determine if the number is 
    // hexadecimal, octal, or binary.  If it is hexadecimal, for example, the number is read into a char
    // array, and the total is calculated.  The same process is done for binary and octal 
    if (c == '0'){
      c = mygetchar();
      if (tolower(c) == 'x'){
        c = mygetchar();
        while(isxdigit(c) != 0){
          s[j] = tolower(c);
          j++;
          c = mygetchar();
        }
        long n = 0;
        for (int i = 0; (s[i] >= '0' && s[i] <= '9') || (tolower(s[i]) >= 'a' && tolower(s[i]) <= 'f'); i++){
          if (isdigit(s[i]) != 0){
            n = 16 * n + (s[i] - '0');
          }
          else{
            n = 16 * n + (s[i] - 87);
          }
        }
        // if negative == 1, that means a negative number was read, so it should be subtracted from total
        if (negative == 1){
          total = total - n;
        }
        else{
          total = total + n;
        }
        negative = 0;
      }
      else if (tolower(c) == 'b'){
        c = mygetchar();
        while (c == '0' || c == '1'){
          s[j] = c;
          j++;
          c = mygetchar();
        }
        ungetchar(c);
        long n = 0;
        for (int i = 0; s[i] == '0' || s[i] == '1'; i++){
          n = 2 * n + (s[i] - '0');
        }
        if (negative == 1){
          total = total - n;
        }
        else{
          total = total + n;
        }
        negative = 0;
      }
      else if (isdigit(c) != 0){
        while (isdigit(c) != 0 && c >= '0' && c < '8'){
          s[j] = c;
          j++;
          c = mygetchar();
        }
        ungetchar(c);
        long n = 0;
        for (int i = 0; s[i] >= '0' && s[i] < '8'; i++){
          n = 8 * n + (s[i] - '0');
        }
        if (negative == 1){
          total = total - n;
        }
        else{
          total = total + n;
        }
        negative = 0;
      }
      else{
        ungetchar(c);
      }
    }
    // if the number is decimal, then it will enter this logic and undergo the same process as with the
    // other number types
    else if (isdigit(c) != 0){
      while (isdigit(c) != 0){
        s[j] = c;
        j++;
        c = mygetchar();
      }
      if (negative == 1){
        total = total - atol(s);
      }
      else{
        total = total + atol(s);
      }
      negative = 0;
    }
    // logic to deal with negative numbers.  Sets negative = 1 if a number is preceded by '-'
    else if (c == '-'){
      c = mygetchar();
      if (isdigit(c) == 0){
        negative = 0;
      }
      else{
        negative = 1;
      }
      ungetchar(c);
    }
  }
  printf("Total: %ld\n", total);
}
