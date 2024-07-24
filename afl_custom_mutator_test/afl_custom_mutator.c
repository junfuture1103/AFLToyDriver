//
// This is an example on how to use afl_custom_send
// It writes each mutated data set to /tmp/foo
// You can modify this to send to IPC, shared memory, etc.
// in pwd PATH : /home/jun20/jun/etc/AFLToyDriver/afl_custom_mutator_test
//
// cc -O3 -shared -fPIC -o custom_send.so -I /AFLplusplus/include afl_custom_mutator.c
// cd ../..
// afl-gcc-fast -o afl_file_input afl_file_input.c
// AFL_CUSTOM_MUTATOR_LIBRARY=/home/jun20/jun/etc/AFLToyDriver/afl_custom_mutator_test/custom_send.so \
// afl-fuzz -i in -o out -- ./afl_file_input ./foo

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "afl-fuzz.h"

typedef struct my_mutator {

  afl_state_t *afl;

} my_mutator_t;

my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));
  if (!data) {

    perror("afl_custom_init alloc");
    return NULL;

  }

  data->afl = afl;

  return data;

}

void afl_custom_fuzz_send(my_mutator_t *data, uint8_t *buf, size_t buf_size) {

  // "FUZZ"의 길이는 4이므로, 새로운 버퍼의 크기를 계산
  size_t new_buf_size = buf_size + 4;

  // 새로운 버퍼를 동적으로 할당
  uint8_t *new_buf = (uint8_t *)malloc(new_buf_size);
  if (new_buf == NULL) {
    // 메모리 할당에 실패한 경우 오류 처리
    return;
  }

  // 새로운 버퍼의 앞부분을 "FUZZ"로 설정
  new_buf[0] = 'F';
  new_buf[1] = 'U';
  new_buf[2] = 'Z';
  new_buf[3] = 'Z';

  // 원래 버퍼의 내용을 새로운 버퍼에 복사
  memcpy(new_buf + 4, buf, buf_size);

  int fd = open("./foo", O_CREAT | O_NOFOLLOW | O_TRUNC | O_RDWR, 0644);

  if (fd >= 0) {
    (void)write(fd, new_buf, new_buf_size);
    close(fd);
  }

  // 동적으로 할당한 메모리를 해제
  free(new_buf);

  return;
}

void afl_custom_deinit(my_mutator_t *data) {

  free(data);

}
