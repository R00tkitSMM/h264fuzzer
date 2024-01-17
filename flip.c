#include <IOKit/IOKitLib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <dlfcn.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>

void printStackTrace() {
  void *callstack[128];
  int frames = backtrace(callstack, sizeof(callstack) / sizeof(callstack[0]));

  char **symbols = backtrace_symbols(callstack, frames);
  if (symbols == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < frames; i++) {
    printf("%s\n", symbols[i]);
  }

  free(symbols);
}

char **arg;

int write_to_file(char *buffer, size_t size) {

  char str[11]; // Buffer to store the string representation

  // Convert the integer to a string
  snprintf(str, sizeof(str), "%d", (int)size);

  // Open a file in write mode
  FILE *file = fopen(str, "w");

  // Check if the file was opened successfully
  if (file == NULL) {
    perror("Error opening file");
    return 1; // Return an error code
  }

  // Write the buffer to the file
  size_t bytes_written = fwrite(buffer, 1, size, file);

  if (bytes_written != sizeof(buffer) - 1) {
    perror("Error writing to file");
    fclose(file);
    return 1; // Return an error code
  }

  // Flush the file stream to make sure the data is written to the file
  if (fflush(file) != 0) {
    perror("Error flushing file");
    fclose(file);
    return 1; // Return an error code
  }

  // Close the file
  fclose(file);

  return 0; // Return success
}

int maybe() {
  static int seeded = 0;
  if (!seeded) {
    srand(time(NULL));
    seeded = 1;
  }
  return !(rand() % 100);
}

void flip_bit(void *buf, size_t len) {
  if (!len)
    return;
  size_t offset = rand() % len;
  ((uint8_t *)buf)[offset] ^= (0x01 << (rand() % 8));
}

void get_arg() {
  void *handle = dlopen(NULL, RTLD_LAZY);
  arg = (char **)dlsym(handle, "arg1");
  printf("arg1 from the main arg: %s\n", *arg);
  dlclose(handle);
}

kern_return_t fake_IOConnectCallMethod(mach_port_t connection,
                                       uint32_t selector, uint64_t *input,
                                       uint32_t inputCnt, void *inputStruct,
                                       size_t inputStructCnt, uint64_t *output,
                                       uint32_t *outputCnt, void *outputStruct,
                                       size_t *outputStructCntP) {

  printf(
      "fake_IOConnectCallMethod selector:%u  inputCnt:%u inputStructCnt:%u\n",
      selector, inputCnt, inputStructCnt);
  printStackTrace();

  if (arg == NULL) {
    get_arg();
  }
  if (maybe()) {
    // write_to_file(input, sizeof(*input) * inputCnt);
    // flip_bit(input, sizeof(*input) * inputCnt);
  }

  if (maybe()) {
    // write_to_file(inputStruct, inputStructCnt);
    // flip_bit(inputStruct, inputStructCnt);
  }

  return IOConnectCallMethod(connection, selector, input, inputCnt, inputStruct,
                             inputStructCnt, output, outputCnt, outputStruct,
                             outputStructCntP);
}

typedef struct interposer {
  void *replacement;
  void *original;
} interpose_t;

__attribute__((used)) static const interpose_t interposers[]
    __attribute__((section("__DATA, __interpose"))) = {
        {.replacement = (void *)fake_IOConnectCallMethod,
         .original = (void *)IOConnectCallMethod}};

__attribute__((constructor)) static void ctor(void) {}
