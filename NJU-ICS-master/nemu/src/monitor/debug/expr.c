#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include "monitor/expr.h"

enum {
  TK_NOTYPE = 256, 		//start from 256, and increase one by one
  TK_EQ,

  /* TODO: Add more token types */
  TK_PLUS,
  TK_MINUS,
  TK_MULTI,
  TK_DIVI,
  TK_LBRACKET,
  TK_RBRACKET,
  TK_NUM,
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level(优先级) of different rules.
   */

  {" +", TK_NOTYPE},    // spaces, + means match front substr once or more times
  {"\\+", TK_PLUS},         // plus
  {"\\-", TK_MINUS},// minus
  {"\\*", TK_MULTI},// multiply 
  {"\\/", TK_DIVI},// division
  {"\\(", TK_LBRACKET},// left bracket
  {"\\)", TK_RBRACKET},// right bracket
  {"^-?[1-9]\\d*$", TK_NUM},//integer
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
  	/* regcomp is used to compile RE,将指定的正则表达式rules[i].regex编译成一种特定的数据格式(regex_t) */
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
	  /* if compilation goes wrong, it means there are bugs in RE */
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

/* store tokens has been recognized in order */
Token tokens[32];
/* number of tokens has been recognized */
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
		/* regexec is used to match RE, and once only match 1 char */
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

		
		/* Notice:for dec number , I should also record its value which is in the substr */
		if (rules[i].token_type == TK_NUM)
		{
			 tokens[nr_token].type = rules[i].token_type;
			 char *temp = e+position;
			 strcat(tokens[nr_token].str, temp);
		}
        else
		{
			tokens[nr_token].type = rules[i].token_type;
        }

			
		
        switch (rules[i].token_type) {
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
