#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void clear_input_buffer(void)
{
  int c;
  do
  {
    c = getchar();
  } while (c != '\n' && c != EOF);
  putchar('\n');
}

bool is_number(char *str)
{
  int len = strlen(str);
  int i;     // for index loop
  int N = 0; // start för loop

  if (len == 0)
  {
    return false;
  }
  if (str[0] == '-' && len == 1)
  {
    return false;
  }
  if (str[0] == '-')
  {
    N = 1;
  }

  for (i = N; i <= len - 1; i++)
  {
    if (!isdigit(str[i]))
    {
      return false;
    }
  }

  return true;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

int read_string(char *buf, int buf_siz)
{
  int n = 0;         // Räknare för buf
  int c = getchar(); // init c med första char i ström
  while (n < buf_siz - 1 && c != '\n' && c != EOF)
  {
    buf[n] = c;    //  spara nuvarande c i buf
    n++;           // inkrement
    c = getchar(); // init nästa c
  }
  buf[n] = '\0'; //  för att skapa en string av buf

  if (c != '\n' && c != EOF)
  {
    clear_input_buffer();
  }

  return n;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
  int buf_siz = 255;
  char buf[buf_siz];
  do
  {
    printf("%s\n", question);
    read_string(buf, buf_siz);
  } while (!check(buf)); //  loop om tom sträng

  return convert(buf);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *)atoi);
  return answer.int_value; // svaret som ett heltal
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *)strdup).string_value;
}

bool is_shelf(char *str)
{
  int len = strlen(str);
  int i;     // for index loop
  int N = 1; // start för loop

  if (len < 2)
  {
    return false;
  }
  if (!isupper(str[0]))
  {
    return false;
  }

  for (i = N; i < len; i++)
  {
    if (!isdigit(str[i]))
    {
      return false;
    }
  }

  return true;
}

char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func *)strdup).string_value;
}

/*
int main(void)
{
    int buf_siz = 255;
    char buf[buf_siz];

    ask_question_string("Din först string" ,buf, buf_siz);
    printf("Du skrev: %s\n", buf);

    ask_question_string("Din andra string:" ,buf, buf_siz);
    printf("Du skrev: %s\n", buf);

    return 0;
}
*/