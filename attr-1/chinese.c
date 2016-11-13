#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <iconv.h>

#include "chinese.h"

void dumpbuf(const char* tip, char* p, size_t len)
{
    size_t i = 0;

    if (tip) printf("%s: ", tip);
    for(; i < len; i++) {
        printf("%02X ", (unsigned char)p[i]);
        if (!(i + 1) % 16) printf("\n");
    }
    printf ("\n");
}

int convert(char *src, char *des, int srclen, int deslen, const char *srctype,
            const char *destype) {
  iconv_t conv = 0;
  if (strcmp(srctype, destype) == 0) {
    memcpy(des, src, min(srclen, deslen));
    return 0;
  }
  conv = iconv_open(destype, srctype);
  if (conv == (iconv_t)-1) {
    printf("iconvopen err\n");
    return -1;
  }

  size_t avail = deslen;
  size_t insize = srclen;
  char *wrptr = des;
  char *inptr = src;
  int ret = 0;

  {
    size_t nconv;
    // printf("avail:%lu\n", avail);
    /* Do the conversion.  */
    nconv = iconv(conv, &inptr, &insize, &wrptr, &avail);
    if (nconv == (size_t)-1) {
      /* Not everything went right.  It might only be
         an unfinished byte sequence at the end of the
         buffer.  Or it is a real problem.  */
      if (errno == EINVAL) {
        /* This is harmless.  Simply move the unused
           bytes to the beginning of the buffer so that
           they can be used in the next round.  */
        // memmove (inbuf, inptr, insize);
        //printf("EINVAL\n");
      } else {
        /* It is a real problem.  Maybe we ran out of
           space in the output buffer or we have invalid
           input.  In any case back the file pointer to
           the position of the last processed byte.  */
        ret = errno;
        // printf("error %d, %s\n", errno, strerror(errno));

      }
    }
  }
  iconv_close(conv);
  return ret;
}
