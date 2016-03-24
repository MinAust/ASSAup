#include <io.h>

int fexist (char *fname)
{
  if (_access (fname, 0) != -1) /* is there a file? */
    return -1;                  /* file exists */
  else
    return 0;                   /* file doesn't exist */
}
