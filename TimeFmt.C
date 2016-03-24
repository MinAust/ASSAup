#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sep.h"

/**********************************************
 * Timsec converts formatted time string i.e  *
 * hh:mm:ss.uuu and converts it into seconds. *
 * seconds may be floating point up to three  *
 * digits.                                    *
 *                                            *
 * Timein is the formatted input time string. *
 * defaults to 3.                             *
 *                                            *
 * The converted seconds are returned as the  *
 * type of "double".                          *
 *                                            *
 * by W.H.Minter                              *
 * 08/12/2013                                 *
 **********************************************/

double timsec (char *timein)
{
  char    intime[80];
  char    intime2[80];
  char    sh[80];
  char    sm[80];
  char    ss[80];

  double  dh;
  double  dm;
  double  ds;
  double  ts;

  strcpy (intime, timein);

  sep (sh, intime2, intime, ':');
  sep (sm, ss, intime2, ':');

  dh = atof (sh);
  dm = atof (sm);
  ds = atof (ss);

  ts = (dh * 3600) + (dm * 60) + ds;

  return (ts);
}

/**********************************************
 * SecTim converts seconds (and microseconds) *
 * into formatted time string i.e             *
 * hh:mm:ss.uuu.                              *
 * Timeout is the formatted string.           *
 * hd is how many digits in the hour. 1 or 2  *
 * are valid, invalid value defaults to 1.    *
 * dp is how manu decimal places for the      *
 * seconds 0-4 are valid, invalid value       *
 * defaults to 3.                             *
 *                                            *
 * by W.H.Minter                              *
 * 08/12/2013                                 *
 **********************************************/

int sectim (char timeout[], double insec, int hd, int dp)
{
  char    timetmp[80];
  char    strhour[80];

  double  inseconds;

  int     ih = 0;
  int     im = 0;

  inseconds = insec;

  if (inseconds >= 3600)
    {
      ih = inseconds / 3600;
      inseconds = inseconds - (ih * 3600);
    }

  if (inseconds >= 60)
    {
      im = inseconds / 60;
      inseconds = inseconds - (im * 60);
    }

  switch (hd)
    {
    case 1:
      sprintf (strhour, "%1i", ih);
      break;
    case 2:
      sprintf (strhour, "%02i", ih);
      break;
    default:
      sprintf (strhour, "%1i", ih);
      break;
    }

  switch (dp)
    {
    case 0:
      sprintf (timetmp, ":%02i:%02.0f", im, inseconds);
      break;
    case 1:
      sprintf (timetmp, ":%02i:%04.1f", im, inseconds);
      break;
    case 2:
      sprintf (timetmp, ":%02i:%05.2f", im, inseconds);
      break;
    case 3:
      sprintf (timetmp, ":%02i:%06.3f", im, inseconds);
      break;
    case 4:
      sprintf (timetmp, ":%02i:%07.4f", im, inseconds);
      break;
    default:
      sprintf (timetmp, ":%02i:%06.3f", im, inseconds);
      break;
    }

  strcat (strhour, timetmp);
  strcpy (timeout, strhour);

  return 0;
}
