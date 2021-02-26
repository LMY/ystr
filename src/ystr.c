#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>

#define StringOrEmpty(x)    ((x)?(x):"")

typedef struct
{
  char input[1024];
  char value1[1024];
  char value2[1024];

  int index;

} ystr_func_arg;

void ystr_func_arg_init(ystr_func_arg* arg)
{
  memset(arg, '\0', sizeof(ystr_func_arg));
  arg->index = 1;
}
typedef void (*ystr_func)(ystr_func_arg*);


void ystr_rep(ystr_func_arg* arg)
{
  for (int i=0; i<arg->index; i++)
    g_print("%s", arg->input);
  g_print("\n");
}

void ystr_set(ystr_func_arg* arg)
{
  const int value_len = strlen(arg->value1);

  char* ret = g_malloc0(MAX(strlen(arg->input), arg->index + value_len) + 1);
  strcpy(ret, arg->input);
  strncpy(&ret[arg->index], arg->value1, value_len);
  g_print("%s\n", ret);
  g_free(ret);
}

void ystr_inc(ystr_func_arg* arg)
{
  arg->input[arg->index] += atoi(arg->value1);
  g_print("%s\n", arg->input);
}

void ystr_get(ystr_func_arg* arg)
{
  const int len = (arg->value1 && arg->value1[0] ? atoi(arg->value1) : 1);

  g_print("%s\n", len >= 0 ? g_strndup(&arg->input[arg->index], len)
                           : g_strndup(&arg->input[arg->index + len], -len));
}

void ystr_reverse(ystr_func_arg* arg)
{
  const int len = strlen(arg->input);

  for (int i=0; i<len/2; i++)
  {
    char temp = arg->input[i];
    arg->input[i] = arg->input[len-1-i];
    arg->input[len-1-i] = temp;
  }

  g_print("%s\n", arg->input);
}

void ystr_equal(ystr_func_arg* arg)
{
  return g_print("%d\n", strcmp(arg->input, arg->value1));
}

void ystr_append(ystr_func_arg* arg)
{
  g_print("%s%s\n", StringOrEmpty(arg->input), StringOrEmpty(arg->value1));
}

void ystr_replace(ystr_func_arg* arg)
{
  char* ret = g_strdup(arg->input);
  int times = arg->index;

  for (int i=0; ret[i]; i++)
    if (ret[i] == arg->value1[0])
      if (--times <= 0)
        ret[i] = arg->value2[0];

  g_print("%s\n", ret);
  g_free(ret);
}

void ystr_comb_replace(ystr_func_arg* arg)
{
  int times = arg->index;

  int idxs[1024];
  int idxs_len = 0;

  for (int i=0; arg->input[i]; i++)
    if (arg->input[i] == arg->value1[0])
      if (--times <= 0)
        idxs[idxs_len++] = i;

  int poss = pow(2, idxs_len);

  for (int i=0; i<poss; i++)
    {
      char* newstring = g_strdup(arg->input);

      for (int k=0, kmask=i; k<idxs_len; k++, kmask /=2)
        if (kmask & 1)
          newstring[idxs[idxs_len - 1 - k]] = arg->value2[0];

      g_print("%s\n", newstring);
      g_free(newstring);
    }
}




int main(int argc, char* argv[])
{
  ystr_func func = NULL;
  ystr_func_arg arg;
  ystr_func_arg_init(&arg);

  for (int i=1; i<argc; i++)
    if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0)
      strcpy(arg.input, argv[++i]);
    else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--times") == 0 || strcmp(argv[i], "--index") == 0)
      arg.index = atoi(argv[++i]);
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--value") == 0 || strcmp(argv[i], "--value1") == 0)
      strcpy(arg.value1, argv[++i]);
    else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--value2") == 0)
      strcpy(arg.value2, argv[++i]);

    else if (strcmp(argv[i], "fill") == 0 || strcmp(argv[i], "background") == 0)
      func = ystr_rep;
    else if (strcmp(argv[i], "set") == 0)
      func = ystr_set;
    else if (strcmp(argv[i], "get") == 0)
      func = ystr_get;
    else if (strcmp(argv[i], "inc") == 0)
      func = ystr_inc;
    else if (strcmp(argv[i], "reverse") == 0)
      func = ystr_reverse;
    else if (strcmp(argv[i], "equal") == 0)
      func = ystr_equal;
    else if (strcmp(argv[i], "append") == 0)
      func = ystr_append;
    else if (strcmp(argv[i], "replace") == 0)
      func = ystr_replace;

    else if (strcmp(argv[i], "comb-replace") == 0)
      func = ystr_comb_replace;

  // if func exist, execute
  if (func)
    func(&arg);

  return 0;
}
