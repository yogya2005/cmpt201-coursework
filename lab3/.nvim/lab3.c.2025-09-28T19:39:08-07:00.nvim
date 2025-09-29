#define _POSIX_C_SOURCE 200809l
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *history[5] = {NULL};
  int count = 0;
  char *line = NULL;
  size_t len = 0;

  while (1) {
    printf("Enter input: ");
    getline(&line, &len, stdin);

    int pos = count % 5;

    if (history[pos] != NULL) {
      free(history[pos]);
    }

    history[pos] = strdup(line);
    count++;

    if (strncmp(line, "print", 5) == 0 &&
        (line[5] == '\n' || line[5] == '\0')) {
      int start = (count < 5) ? 0 : count % 5;
      int items = (count < 5) ? count : 5;

      for (int i = 0; i < items; i++) {
        printf("%s", history[(start + i) % 5]);
      }
    }
  }

  free(line);
  for (int i = 0; i < 5; i++) {
    if (history[i] != NULL) {
      free(history[i]);
    }
  }

  return 0;
}
