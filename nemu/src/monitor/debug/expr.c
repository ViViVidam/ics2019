#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  RE_GREAT = 300,
  RE_EQU = 299,
  RE_SMALL = 298,
  RE_NOTYPE = 297,
  REDUCED = 296,
  TK_NOTYPE = 256,
  TK_EQ = 255,
  TK_NUM = 127,


  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus,在字符串层面先把/给转义了，然后/会把+转义
  {"==", TK_EQ},         // equal
  {"\\-",'-'},
  {"\\/",'/'},
  {"\\*",'*'},
  {"[0-9]+",TK_NUM},
  {"\\-[0-9]+",TK_NUM},
  {"\\(",'('},
  {"\\)",')'}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    //regcomp内置函数，编译错了返回非0
    //compiled result will be put into re 数组
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
  int val;
} Token;
/* + - * / ( ) i # */
static int expr_priority[8][8] = {
  {RE_GREAT,RE_GREAT,RE_SMALL,RE_SMALL,RE_SMALL,RE_GREAT,RE_SMALL,RE_GREAT},
  {RE_GREAT,RE_GREAT,RE_SMALL,RE_SMALL,RE_SMALL,RE_GREAT,RE_SMALL,RE_GREAT},
  {RE_GREAT,RE_GREAT,RE_GREAT,RE_GREAT,RE_SMALL,RE_GREAT,RE_SMALL,RE_GREAT},
  {RE_GREAT,RE_GREAT,RE_GREAT,RE_GREAT,RE_SMALL,RE_GREAT,RE_SMALL,RE_GREAT},
  {RE_SMALL,RE_SMALL,RE_SMALL,RE_SMALL,RE_SMALL,RE_EQU,RE_SMALL,RE_NOTYPE},
  {RE_GREAT,RE_GREAT,RE_GREAT,RE_GREAT,RE_NOTYPE,RE_GREAT,RE_NOTYPE,RE_GREAT},
  {RE_GREAT,RE_GREAT,RE_GREAT,RE_GREAT,RE_NOTYPE,RE_GREAT,RE_NOTYPE,RE_GREAT},
  {RE_SMALL,RE_SMALL,RE_SMALL,RE_SMALL,RE_SMALL,RE_NOTYPE,RE_SMALL,RE_EQU}
};
static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
static bool make_token(char *e,int* token_length) {
  int position = 0;
  int i;
  int usedtoken = 0;
  regmatch_t pmatch; //匹配的开始点与结束点

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NUM:
            strncpy(tokens[usedtoken].str, substr_start,substr_len);
            printf("%s\n",tokens[usedtoken].str);
            tokens[usedtoken++].type = TK_NUM;
            break;
          case '+':
            tokens[usedtoken].type = '+';
            tokens[usedtoken++].str[0] = '\0';
            break;
          case '-':
            tokens[usedtoken].str[0] = '\0';
            tokens[usedtoken++].type = '-';
            break;
          case '*':
            tokens[usedtoken].str[0] = '\0';
            tokens[usedtoken++].type = '*';
            break;
          case '/':
            tokens[usedtoken].str[0] = '\0';
            tokens[usedtoken++].type = '/';
            break;
          case '(':
            tokens[usedtoken].str[0] = '\0';
            tokens[usedtoken++].type = '(';
            break;
          case ')':
            tokens[usedtoken].str[0] = '\0';
            tokens[usedtoken++].type = ')';
            break;
          default:;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  *token_length = usedtoken;
  return true;
}

int getindex(char a){
  switch(a){
      case '#':
        return 7;
        break;
      case TK_NUM:
        return 6;
      case ')':
        return 5;
      case '(':
        return 4;
      case '/':
        return 3;
      case '*':
        return 2;
      case '-':
        return 1;
      case '+':
        return 0;
  }
}

uint32_t expr(char *e, bool *success) {
  int i = 0;
  Token tokenstack[128];
  int token_top = 0;
  int priority_stack[128];//the location where <
  int priority_top = 0;
  int token_length = 0;

  if (!make_token(e,&token_length)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  /* total token count only has 32 length at maximum*/
  priority_stack[priority_top++] = 0;
  tokens[token_length].type = '#';
  tokens[token_length++].str[0] = "\0";
  tokenstack[token_top++].type = '#';
  for(i = 0;i < token_length; i++){
    int present = tokenstack[priority_stack[priority_top-1]].type;
    int next = tokens[i].type;
    char* str = tokens[i].str;
    int relation = expr_priority[getindex(present)][getindex(next)];
    printf("%d %d\n",present,next);
    if(relation==RE_SMALL){
      strcpy(tokenstack[token_top].str,str);
      tokenstack[token_top++].type = next;
      priority_stack[priority_top++]=token_top-1;
      printf("%s move in RE_SMALL\n",tokenstack[token_top-1].str);
    }
    else if(relation==RE_GREAT){
      int begin = priority_stack[--priority_top];
      /* i => real number*/
      printf("%d\n",begin);
      if( (token_top - begin) == 1 && tokenstack[begin].type == TK_NUM)
      {
        printf("number reduced:%s\n",tokenstack[begin].str);
        tokenstack[begin].val = atoi(tokenstack[begin].str);
        tokenstack[begin].type = REDUCED; 
      }
      /* (E) or E op E */
      else{
        printf("expression reduced\n");
        if(tokenstack[begin].type==')'){
          token_top = begin-1;//push out three element and in one element
          tokenstack[begin-2].type = REDUCED;
          tokenstack[begin-2].val = tokenstack[begin-1].val;
          priority_top--;
          printf("() detected, res:%d\n",tokenstack[begin-2].val);
        }
        else if(tokenstack[begin-1].type==REDUCED){
          token_top = begin;
          switch(tokenstack[begin].type){
            case '+':
              tokenstack[begin-1].val = tokenstack[begin-1].val + tokenstack[begin+1].val;
              printf("%d\n",tokenstack[begin].val);
              break;
            case '-':
              tokenstack[begin-1].val = tokenstack[begin-1].val - tokenstack[begin+1].val;
              break;
            case '*':
              tokenstack[begin-1].val = tokenstack[begin-1].val * tokenstack[begin+1].val;
              break;
            case '/':
              tokenstack[begin-1].val = tokenstack[begin-1].val / tokenstack[begin+1].val;
              break; 
            default:
              printf("unrecognized operation %d\n",tokenstack[begin].type);
              return -1;
          }
        }
      }
      i = i - 1;
    }
    else if(relation==RE_EQU){
      printf("RE_EQU\n");
      strcpy(tokenstack[token_top].str,str);
      tokenstack[token_top++].type = next;
      priority_stack[priority_top++] = token_top - 1;
    }
    else{
      *success = false;
      return 0;
    }
  }
  if(token_top!=3){
    printf("reduction failed, wrong expression\n");
    *success = false;
    return 0;
  }
  else{
    printf("answer: %d\n",tokenstack[1].val);
    *success = true;
    return tokenstack[1].val;
  }
}
