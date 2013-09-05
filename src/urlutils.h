
#ifndef __URLUTILS_HEADER__
#define __URLUTILS_HEADER__

#include <stdlib.h>
#include <ctype.h>

// Adapted from http://stackoverflow.com/questions/2673207/c-c-url-decode-library
juce::String urldecode(const juce::String &str) {
  String out;
  out.preallocateBytes(str.length() + 1);

  const char *src = str.getCharPointer();

  char a, b;
  while (*src) {
    if ((*src == '%') &&
        ((a = src[1]) && (b = src[2])) &&
        (isxdigit(a) && isxdigit(b))) {
      if (a >= 'a')
        a -= 'A'-'a';
      if (a >= 'A')
        a -= ('A' - 10);
      else
        a -= '0';
      if (b >= 'a')
        b -= 'A'-'a';
      if (b >= 'A')
        b -= ('A' - 10);
      else
        b -= '0';

      char newChars[] = {static_cast<char>(16*a+b), '\0'};
      out += newChars;
      
      src+=3;
    } else {
      out += *src++;
    }
  }
  // out += '\0';

  return out;
}

#endif
