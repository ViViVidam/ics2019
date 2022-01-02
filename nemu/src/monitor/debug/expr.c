#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  RULE = 200,
  RE_NOTYPE = 297,
  REDUCED = 296,
  TK_NOTYPE = 256,
  TK_EQ = 255,
  TK_NUM = 127,
  TK_HEX=126,
  TK_REG=125


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
  {"0x[0-9]+",TK_HEX},
  {"[0-9]+",TK_NUM},
  {"\\(",'('},
  {"\\)",')'},
  {"\\$[a-z|0-9]+",TK_REG}
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
static int reg_expr[26][13] = {
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,2},
  {3,8,11,12,15,17,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,25,RULE+1,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,4},
  {RULE+2,RULE+2,11,12,RULE+2,RULE+2,RE_NOTYPE,RULE+2,RULE+2,RULE+2,RULE+2,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,9},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,13},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,10},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,14},
  {RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RULE+6,RE_NOTYPE,RE_NOTYPE},
  {3,8,11,12,15,17,RE_NOTYPE,19,RE_NOTYPE,25,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,21},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,22},
  {RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RULE+7,RE_NOTYPE,RE_NOTYPE},
  {RULE+3,RULE+3,11,12,RULE+3,RULE+3,RE_NOTYPE,RULE+3,RULE+3,RULE+3,RULE+3,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,16,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,23},
  {RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,18,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,24},
  {RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RULE+10,RE_NOTYPE,RE_NOTYPE},
  {RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RULE+11,RE_NOTYPE,RE_NOTYPE},
  {RULE+4,RULE+4,RULE+4,RULE+4,RULE+4,RULE+4,RE_NOTYPE,RULE+4,RULE+4,RULE+4,RULE+4,RE_NOTYPE,RE_NOTYPE},
  {RULE+5,RULE+5,RULE+5,RULE+5,RULE+5,RULE+5,RE_NOTYPE,RULE+5,RULE+5,RULE+5,RULE+5,RE_NOTYPE,RE_NOTYPE},
  {3,8,11,12,RULE+8,RULE+8,RE_NOTYPE,RULE+8,RULE+8,RULE+8,RULE+8,RE_NOTYPE,RE_NOTYPE},
  {3,8,11,12,RULE+9,RULE+9,RE_NOTYPE,RULE+9,RULE+9,RULE+9,RULE+9,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,26,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE},
  {RE_NOTYPE,6,5,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,7,RE_NOTYPE,20,RE_NOTYPE,RE_NOTYPE,RE_NOTYPE,27},
  {3,8,11,12,15,17,RE_NOTYPE,RULE+12,RULE+12,RULE+12,RULE+12,RE_NOTYPE,RE_NOTYPE}
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
          case TK_HEX:
            strncpy(tokens[usedtoken].str, substr_start,substr_len);
            tokens[usedtoken++].type = TK_HEX;
          case TK_REG:
            strncpy(tokens[usedtoken].str, substr_start,substr_len);
            tokens[usedtoken++].type = TK_REG;
          case TK_NUM:
            strncpy(tokens[usedtoken].str, substr_start,substr_len);
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

int getindex(uint32_t a){
  switch(a){
      case 'E':
        return 12;
      case 'A':
       return 11;
      case '#':
        return 10;
      case '&':
        return 9;
      case TK_REG:
        return 8;
      case TK_HEX:
        return 8;
      case TK_NUM:
        return 8;
      case ')':
        return 7;
      case '(':
        return 6;
      case '!':
        return 5;
      case '=':
        return 4;
      case '/':
        return 3;
      case '*':
        return 2;
      case '-':
        return 1;
      case '+':
        return 0;
      default:
        return -1;
  }
}

uint32_t expr(char *e, bool *success) {
  int i = 0;
  Token tokenstack[128];
  int tokenpointer = 0;
  int statestack[128];//the location where <
  int statepointer = 0;
  int token_length = 0;
  bool success2 = false;
  int type = 0;
  if (!make_token(e,&token_length)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  /* total token count only has 32 length at maximum*/
  tokens[token_length++].type = '#';
  tokenstack[tokenpointer++].type = '#';
  statestack[statepointer++] = 1;
  for(i=0;i<token_length;i++){
    int x = statestack[statepointer-1]-1;
    int y = getindex(tokens[i].type);
    int action = reg_expr[x][y];
    printf("(%d %d)action: %d\n",x,y,action);
    if(action>RULE){
      action = action - RULE;
      switch(action){
        case 1:
          *success = true;
          printf("finish\n");
          return tokenstack[tokenpointer-1].val;
          break;
        case 2:
          tokenstack[tokenpointer-3].val = tokenstack[tokenpointer-1].val + tokenstack[tokenpointer-3].val;
          tokenpointer -= 2;
          statepointer -= 2;
          break;
        case 3:
          tokenstack[tokenpointer-3].val = tokenstack[tokenpointer-3].val - tokenstack[tokenpointer-1].val;
          tokenpointer -= 2;
          statepointer -= 2;
          break;
        case 4:
          tokenstack[tokenpointer-3].val = tokenstack[tokenpointer-3].val * tokenstack[tokenpointer-1].val;
          tokenpointer -= 2;
          statepointer -= 2;
          break;
        case 5:
          tokenstack[tokenpointer-3].val = tokenstack[tokenpointer-3].val / tokenstack[tokenpointer-1].val;
          tokenpointer -= 2;
          statepointer -= 2;
          break;
        case 6:
          tokenstack[tokenpointer-2].val = vaddr_read(tokenstack[tokenpointer-1].val,4);
          tokenpointer -= 1;
          statepointer -= 1;
          break;
        case 7:
          printf("val %d %d\n",tokenstack[tokenpointer-2].val,tokenstack[tokenpointer-1].val);
          tokenstack[tokenpointer-2].val = -tokenstack[tokenpointer-1].val;
          tokenpointer -= 1;
          statepointer -= 1;
          break;
        case 8:
          tokenstack[tokenpointer-4].val = (tokenstack[tokenpointer-1].val == tokenstack[tokenpointer-4].val);
          tokenpointer -= 3;
          statepointer -= 3;
          break;
        case 9:
          tokenstack[tokenpointer-4].val = (tokenstack[tokenpointer-1].val != tokenstack[tokenpointer-4].val);
          tokenpointer -= 3;
          statepointer -= 3;
          break;
        case 10:
          tokenstack[tokenpointer-3].val = tokenstack[tokenpointer-2].val;
          tokenpointer -= 2;
          statepointer -= 2;
          break;
        case 11:
          type = tokenstack[tokenpointer-1].type;
          printf("token type %d\n",type);
          if(type == TK_HEX){
            tokenstack[tokenpointer-1].val = (int)strtol(tokenstack[tokenpointer], NULL, 16);
            printf("hex %d\n",tokenstack[tokenpointer-1].val);
          }
          else if(type == TK_NUM)
            tokenstack[tokenpointer-1].val = atoi(tokenstack[tokenpointer-1].str);
          else{
            int tmp = isa_reg_str2val(tokenstack[tokenpointer-1].str,&success2);
            if(success2){
              tokenstack[tokenpointer-1].val = tmp;
            }
            else{
              printf("invalid register name %s\n",tokenstack[tokenpointer-1].str);
              success = false;
              return 0;
            }
          }
          break;
        case 12:
          tokenstack[tokenpointer-4].val = (tokenstack[tokenpointer-1].val && tokenstack[tokenpointer-4].val);
          tokenpointer -= 3;
          statepointer -= 3;
          break;
        default:
          printf("unrecognized action\n");
          i = 999;
      }
      tokenstack[tokenpointer-1].type = 'E';
      y = getindex('E');
      int state_pre = statestack[statepointer-2]-1;
      statestack[statepointer-1] = reg_expr[state_pre][y];
      for(int j=0;j<3;j++)
        printf("%d ",tokenstack[j].val);
      printf("\n");
      i--;
    }
    else if(action==RE_NOTYPE){
      *success = false;
      printf("Illegal expression\n");
      return 0;
    }
    else{
        statestack[statepointer++] = action;
        strcpy(tokenstack[tokenpointer].str,tokens[i].str);
        tokenstack[tokenpointer++].type = tokens[i].type;
    }
  }
}
