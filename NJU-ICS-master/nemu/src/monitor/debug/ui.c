#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
  	/* After using the allocated variable, you must free it */ 
    free(line_read);
    line_read = NULL;
  }

  /* (nemu) is a prompt */
  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

/* continue to run the program which had been stopped before */
static int cmd_c(char *args) {
 /* when the arg of cpu_exec is -1, it means cpu runs without stop */
 /* because the argument's tyep of cpu_exec is uint64_t, so -1 will
   * be turn to a very very large number: 1111……1111(total 64)
 */
  cpu_exec(-1);
  return 0;
}

/* exit the nemu */
static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

/* help info that cmd_help function will print */
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","Let program execute(single step) N instructions and pause; when N is not given, the default is 1",cmd_si},
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

/* single step execution */
static int cmd_si(char *args)
{
	/* if args is not given, then just execute 1 instruction */
	if (args == NULL)
	{
		cpu_exec(1);
	}
	else
	{
		/* Trans char to int */
		int N = (int)(*args);
		cpu_exec(N);
	}
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    /* then print all the help info */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
		/* if user entered a command, then find which comman user entered */
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
	/* If there is no matched command, print texts showed below */
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  /* batch_mode:批处理模式 */
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     * 将剩余的字符串当做参数，将来可能会被解析
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
	  	/* 调用cmd_table[i]对应的函数，并传入参数args */
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
