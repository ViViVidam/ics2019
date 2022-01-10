#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
  char *arg = strtok(NULL, " ");
  if(arg==NULL){
    exec_once();
    return 0;
  }
  else{
    int count = atoi(arg);
    if(count==0){
      printf("must be an integer greater than zero\n");
      return 0;
    }
    else{
    //printf("%s %d",arg,count);
      cpu_exec(count);
    }
  }
};

static int cmd_info(char *args){
  char *arg=strtok(NULL," ");
  if(arg==NULL){
    printf("expect subparam r or w \n");
  }
  else if(strcmp(arg,"r")==0){
    isa_reg_display();
  }
  else if(strcmp(arg,"w")==0){

  }
  else{
    printf("invalid subparam %s",arg);
  }

}

static int cmd_p(char* args){
  char *arg = strtok(NULL,"");
  bool success=false;
  int result = 0;
  if(arg==NULL){
    printf("subparams are expected\n");
  }
  printf("%s",arg);
  result = expr(arg,&success);
  if(success){
    printf("expression answer: %d\n",result);
  }
  else{
    printf("invalid expression\n");
  }
}

static int cmd_x(char* args){
  char *arg = strtok(NULL," ");
  bool success = false;
  int size = atoi(arg);
  arg = strtok(NULL,"");
  int addr = expr(arg,&success);
  if(success){
    for(int i=0;i<size;i++){
      vaddr_read(addr,4);
      addr+=1;
    }
  }
  else
    printf("invalid addr expression\n");
}

static int cmd_d(char* args){
  char *arg = strtok(NULL,"");
  int num = atoi(arg);
  if(num==0 && arg[0]!='0' && strlen(arg)!=1){
    printf("invalid number %s\n",arg);
  }
  else{
    switch(free_wp(num)){
      case 1:
        printf("watchpoint freed %d\n",num);
        break;
      case -1:
        printf("watchpoint %d not found\n",num);
        break;
      case 0:
        printf("no watchpoint is set\n");
        break;
      default:
        ;
    }
    
  }
}

static int cmd_w(char* args){
  char *arg = strtok(NULL,"");
  printf("arg: %s\n",arg);
  WP* wp =  new_wp();
  if(wp){
  strcpy(wp->expr,arg);
  printf("watchpoint created at %d\nexpression: %s\n",wp->NO,wp->expr);
  }
  else{
    printf("please free some watchpoint\n");
  }
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "execute next N", cmd_si},
  { "info", "", cmd_info},
  { "p", "get expression val", cmd_p},
  { "x", "scan memory size with N", cmd_x},
  { "w", "set watchpoint N", cmd_w},
  { "d", "delete watchpoint N", cmd_d},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
