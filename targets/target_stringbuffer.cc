// This file is used to mimic the StringBuffer bug in JDK1.4
// Author: Jie Yu (jieyu@umich.edu)

extern "C" {
#define restrict __restrict__
#include <pthread.h>
}

#define INTEGER_MAX_VALUE 0x7fffffff

class StringBuffer {
 public:
  StringBuffer();
  explicit StringBuffer(int length);
  explicit StringBuffer(char *str);
  ~StringBuffer();

  int length();
  void getChars(int srcBegin, int srcEnd, char *dst, int dstBegin);
  StringBuffer *append(StringBuffer *sb);
  StringBuffer *append(char *str);
  StringBuffer *erase(int start, int end);
  void print();

 private:
  char *value;
  int value_length;
  int count;
  pthread_mutex_t mutex_lock;

  static StringBuffer *null_buffer;

  void expandCapacity(int minimumCapacity);
};


#define assert(x) if(!(x)) { __builtin_trap(); }
#include <cstdio>
#include <cstring>
#include <pthread.h>

StringBuffer *StringBuffer::null_buffer = new StringBuffer("null");

StringBuffer::StringBuffer() {
  value = new char[16];
  value_length = 16;
  count = 0;
  pthread_mutex_init(&mutex_lock, NULL);
}

StringBuffer::StringBuffer(int length) {
  value = new char[length];
  value_length = length;
  count = 0;
  pthread_mutex_init(&mutex_lock, NULL);
}

StringBuffer::StringBuffer(char *str) {
  int length = strlen(str) + 16;
  value = new char[length];
  value_length = length;
  count = 0;
  pthread_mutex_init(&mutex_lock, NULL);
  append(str);
}

StringBuffer::~StringBuffer() {
  delete[] value;
}

int StringBuffer::length() {
  pthread_mutex_lock(&mutex_lock);
  int ret = count;
  pthread_mutex_unlock(&mutex_lock);
  return ret;
}

void StringBuffer::getChars(int srcBegin, int srcEnd,
                            char *dst, int dstBegin) {
  pthread_mutex_lock(&mutex_lock);
  if (srcBegin < 0) {
    assert(0);
  }
  if ((srcEnd < 0) || (srcEnd > count)) {
    assert(0);
  }
  if (srcBegin > srcEnd) {
    assert(0);
  }
  memcpy(dst + dstBegin, value + srcBegin, srcEnd - srcBegin);
  pthread_mutex_unlock(&mutex_lock);
}

StringBuffer *StringBuffer::append(StringBuffer *sb) {
  pthread_mutex_lock(&mutex_lock);
  if (sb == NULL) {
    sb = null_buffer;
  }

  int len = sb->length();
  int newcount = count + len;
  if (newcount > value_length)
    expandCapacity(newcount);
  sb->getChars(0, len, value, count);
  count = newcount;
  pthread_mutex_unlock(&mutex_lock);
  return this;
}

StringBuffer *StringBuffer::append(char *str) {
  pthread_mutex_lock(&mutex_lock);
  if (str == NULL) {
    str = "null";
  }

	int len = strlen(str);
	int newcount = count + len;
	if (newcount > value_length)
	    expandCapacity(newcount);
  memcpy(value + count, str, len);
	count = newcount;
	pthread_mutex_unlock(&mutex_lock);
	return this;
}

StringBuffer *StringBuffer::erase(int start, int end) {
  pthread_mutex_lock(&mutex_lock);
  if (start < 0)
    assert(0);
  if (end > count)
    end = count;
  if (start > end)
    assert(0);

  int len = end - start;
  if (len > 0) {
    memcpy(value + start, value + start + len, count - end);
    count -= len;
  }
  pthread_mutex_unlock(&mutex_lock);
  return this;
}

void StringBuffer::print() {
  for (int i = 0; i < count; i++) {
    printf("%c", *(value + i));
  }
  printf("\n");
}

void StringBuffer::expandCapacity(int minimumCapacity) {
  int newCapacity = (value_length + 1) * 2;
  if (newCapacity < 0) {
    newCapacity = INTEGER_MAX_VALUE;
  } else if (minimumCapacity > newCapacity) {
    newCapacity = minimumCapacity;
  }

  char *newValue = new char[newCapacity];
  memcpy(newValue, value, count);
  delete[] value;
  value = newValue;
  value_length = newCapacity;
}

/*
BEGIN MAIN
*/

StringBuffer *buffer;

extern "C" void *thread_main(void *args) {
  //while (1) {
    buffer->erase(0, 3);
    buffer->append("abc");
  //}
  return NULL;
}

extern "C" int fuzz_target(int argc, char *argv[]) {
  pthread_t thd;
  int rc;
  buffer = new StringBuffer("abc");

  rc = pthread_create(&thd, NULL, thread_main, NULL);

  //while (1) {
    StringBuffer *sb = new StringBuffer();
    sb->append(buffer);
  //}

  pthread_join(thd, NULL);

  delete buffer;
  
  return 0;
}
