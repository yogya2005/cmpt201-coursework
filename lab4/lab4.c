#define _DEFAULT_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *memory = sbrk(256);
  if (memory == (void *)-1) {
    handle_error("sbrk failed");
  }

  struct header *first_block = (struct header *)memory;

  struct header *second_block = (struct header *)((char *)memory + 128);

  first_block->size = 128;
  first_block->next = NULL;

  second_block->size = 128;
  second_block->next = first_block;

  print_out("first block:         %p\n", &first_block, sizeof(void *));
  print_out("second block:        %p\n", &second_block, sizeof(void *));
  print_out("first block size:    %p\n", &first_block->size, sizeof(uint64_t));
  print_out("first block next:    %p\n", &first_block->next, sizeof(void *));
  print_out("second block size:   %p\n", &second_block->size, sizeof(uint64_t));
  print_out("second block next:   %p\n", &second_block->next, sizeof(void *));

  char *first_data = (char *)first_block + sizeof(struct header);
  char *second_data = (char *)second_block + sizeof(struct header);

  size_t data_size = 128 - sizeof(struct header);

  memset(first_data, 0, data_size);
  memset(second_data, 1, data_size);

  for (size_t i = 0; i < data_size; i++) {
    char byte_buf[16];
    ssize_t len = snprintf(byte_buf, 16, "%d\n", first_data[i]);
    write(STDOUT_FILENO, byte_buf, len);
  }
  for (size_t i = 0; i < data_size; i++) {
    char byte_buf[16];
    ssize_t len = snprintf(byte_buf, 16, "%d\n", second_data[i]);
    write(STDOUT_FILENO, byte_buf, len);
  }
  return 0;
}
