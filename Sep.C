#include <stdio.h>
#include <string.h>

int sep (char final1[], char final2[], char *buf, int match)
{
  char   *r;
  char   *s;

  s = strchr (buf, match);

  if (s != NULL)
    {
      r = buf;
      strcpy (final1, buf);
      final1[s - r] = '\0';
      strcpy (final2, s + 1);
      return (0);
    }
  else
    {
      strcpy (final1, buf);
      final2[0] = '\0';
      return (1);
    }

  return (2);
}
