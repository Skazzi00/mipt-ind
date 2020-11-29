#pragma once
#include <cstdio>
#include <cstring>
#include <cstdarg>

namespace mipt {

static constexpr size_t MAX_LINE_LENGTH = 512;

void vsayAndPrintf(const char *format, va_list argptr) {
  char buffer[MAX_LINE_LENGTH + 30];
  strcpy(buffer, "say ");
  vsnprintf(buffer + sizeof("say"), MAX_LINE_LENGTH, format, argptr);

  printf("%s", buffer + sizeof("say"));

  fflush(stdout);
  system(buffer);
}

void sayAndPrintf(const char *format, ...) {
  va_list argptr;
  va_start(argptr, format);
  vsayAndPrintf(format, argptr);
  va_end(argptr);
}

void input(char *buf, int size, const char *format, ...) {
  va_list argptr;
  va_start(argptr, format);
  vsayAndPrintf(format, argptr);
  scanf(" ");
  fgets(buf, size, stdin);
  buf[strlen(buf) - 1] = '\0';
  va_end(argptr);
}
}