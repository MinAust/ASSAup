
/**********************************************************
 * ASSAup.c - Advanced SubStation Alpha subtitle time     *
 * fixer. Slows down PAL subtitles to NTSC times.         *
 *                                                        *
 * return code 0 means successful run                     *
 * return code 1 means invalid argument count             *
 * return code 2 means invalid file extension             *
 * return code 3 means error opening file                 *
 * return code 4 means file missing                       *
 *                                                        *
 * by W.H.Minter                                          *
 * 08/12/2013                                             *
 * 02/14/2014 - cosmetic fixes                            *
 * 01/23/2016 - backup file naming changed, command line  *
 *              seitches added                            *
 * 03/24/2016 - version maintainence improvement          *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "timefmt.h"
#include "conclr.h"
#include "sep.h"
#include "fexist.h"
#include "version.h"

#ifdef _MSC_VER
#ifdef _M_X64
#define ENVIRONMENT64
#define COPYRIGHT_NOTICE \
        "ASSAup v" ASSAUP_VERSION " (c) 2013, 2016 WH Minter.\nAll rights reserved.\nMSVC 64-bit build.\n"  \
        "Built on " __DATE__ " at " __TIME__ ".\n"

#else
#define ENVIRONMENT32
#define COPYRIGHT_NOTICE \
        "ASSAup v" ASSAUP_VERSION " (c) 2013, 2016 WH Minter.\nAll rights reserved.\nMSVC 32-bit build.\n"  \
        "Built on " __DATE__ " at " __TIME__ ".\n"
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#define COPYRIGHT_NOTICE \
        "ASSAup v" ASSAUP_VERSION " (c) 2013, 2016 WH Minter.\nAll rights reserved.\nGCC-MinGW v." __VERSION__ " 64-bit build.\n"  \
        "Built on " __DATE__ " at " __TIME__ ".\n"

#else
#define ENVIRONMENT32
#define COPYRIGHT_NOTICE \
        "ASSAup v" ASSAUP_VERSION " (c) 2013, 2016 WH Minter.\nAll rights reserved.\nGCC-MinGW v." __VERSION__ " 32-bit build.\n"  \
        "Built on " __DATE__ " at " __TIME__ ".\n"

#endif
#endif

void    usage (void);

int main (int argc, char *argv[])
{
  int     colr = 7;
  int     dialcnt = 0;
  int     linecnt = 0;
  int     p = 0;
  int     rc;
  int     x = 0;
  int     detailflag = 0;
  int     backflag = 0;
  int     direction = 0;

  double  ntime = 0;
  double  ptime = 0;
  double  mul = 25.0 / (24000.0 / 1001.0);

  char    data[1024];
  char    final[1024];
  char    infile[2048];
  char    outfile[2048];
  char    tempfile[2048];
  char    set[50][250];
  char    token[1024];
  char    remain[1024];

  char    drive[_MAX_DRIVE];
  char    dir[_MAX_DIR];
  char    fname[_MAX_FNAME];
  char    ext[_MAX_EXT];

  FILE   *in, *out;

  /*system ("cls");*/

  conclr (15);
  printf ("%s\nConvert .ass subtitles from PAL to NTSC timing.\n",
          COPYRIGHT_NOTICE);

  if (argc < 2)
    {
      conclr (12);
      printf ("Incorrect syntax.\n");
      usage ();
      return (1);
    }

  if (argc > 2)                 /* if argument count is more than two, figure 
                                   out which command line options were used */
    {
      for (x = 2; x < argc; x++)
        {
          if (stricmp (argv[x], "-nd") == 0)    // do not show details
            {
              detailflag = 1;
              conclr (15);
              printf ("-nd switch used - no detail print.\n");
            }
          else if (stricmp (argv[x], "-nb") == 0)   /* disable backups? Not a 
                                                       good idea, but hey,
                                                       you're the boss */
            {
              backflag = 1;
              conclr (15);
              printf ("-nb switch used - no backup file will be created.\n");
            }
          else if (stricmp (argv[x], "-np") == 0)   /* Do it backwards: NTSC
                                                       to PAL conversion */
            {
              direction = 1;
              conclr (15);
              mul = (24000.0 / 1001.0) / 25.0;
              printf
                ("-np switch used - NTSC to PAL conversion.\nMultiplier is %f.\n",
                 mul);
            }
          else                  /* default: we come here on a boo-boo. We
                                   already caught the valid switches. */
            {
              conclr (15);
              printf ("%s is an invalid command line switch.", argv[x]);
              usage ();
              conclr (7);
              return 4;
            }
        }
    }

  strcpy (infile, argv[1]);

  if (!fexist (infile))
    {
      printf ("%s - No such file.\n", infile);
      usage ();
      return 3;
    }

  _splitpath (infile, drive, dir, fname, ext);

  if (stricmp (ext, ".ass") != 0)
    {
      conclr (12);
      printf ("Invalid file type.\n");
      usage ();
      conclr (7);
      return (2);
    }

  _makepath (tempfile, drive, dir, fname, "$$$$$$$$");

  if (direction == 0)
    strcat (fname, ".PAL");
  else
    strcat (fname, ".NTSC");

  _makepath (outfile, drive, dir, fname, ext);

  in = fopen (infile, "r");     /* open 'in' for read */
  out = fopen (tempfile, "w");  /* open 'out' for write */

  if (in == NULL)
    {
      conclr (12);
      printf ("%s could not be opened.\n", infile);
      usage ();
      fclose (out);
      remove (tempfile);
      conclr (7);
      return (3);
    }

  while (fgets (data, sizeof (data), in) != NULL)
    {
      strcpy (final, data);
      linecnt++;

/*----------------------------------------*/
      if (strstr (data, "Dialogue:") != NULL)
        {
          x = 0;
          rc = 0;
          dialcnt++;

          while (rc == 0)
            {
              rc = sep (token, remain, data, ',');
              strcpy (set[x], token);
              x++;

              strcpy (data, remain);
            }

          ntime = timsec (set[1]);
          ptime = ntime * mul;
          sectim (set[1], ptime, 1, 2);

          ntime = timsec (set[2]);
          ptime = ntime * mul;
          sectim (set[2], ptime, 1, 2);

          strcpy (final, set[0]);

          for (p = 1; p < x; p++)
            {
              strcat (final, ",");
              strcat (final, set[p]);
            }
        }

/*----------------------------------------*/
      fputs (final, out);

      if (detailflag == 0)
        {
          conclr (colr);
          printf ("%s", final);

          switch (colr)
            {
            case 7:
              colr = 15;
              break;
            case 12:
              colr = 7;
              break;
            case 15:
              colr = 7;
              break;
            }
        }
    }

  fclose (in);
  fclose (out);

  if (backflag == 0)
    {
      if (fexist (outfile))
        remove (outfile);

      rename (infile, outfile);
      rename (tempfile, infile);
      conclr (11);
      printf
        ("\nProcessing completed. The time-adjusted file is stored as:\n%s.\n",
         infile);
      printf ("The original unchanged file is stored as:\n%s.\n", outfile);
      printf ("There were %i lines in the file, of which %i were dialog.\n\n",
              linecnt, dialcnt);
    }
  else
    {
      remove (infile);
      rename (tempfile, infile);
      conclr (11);
      printf
        ("\nProcessing completed. The time-adjusted file is stored as:\n%s.\n",
         infile);
      printf ("There is no backup file.\n");
      printf ("There were %i lines in the file, of which %i were dialog.\n\n",
              linecnt, dialcnt);
    }

  conclr (7);

  return (0);

  /* 
   * Miller Time! - end of program! 
   */
}

void usage (void)
{
  conclr (15);

  printf
    ("\nThe proper syntax is:\nASSAup infile.ass [switches: -nd -nb]\n"
     "the switches are:\n-nd - no detail print during processing\n"
     "-nb - no backup of the original file.\n"
     "-np - NTSC to PAL conversion. PAL to NTSC conversion is the default operation.\n\n");

  printf
    ("If the file name contains spaces, it is to be enclosed in double quotes,\n"
     "such as:\nASSAup \"My File.Ass\".\n\n");

  conclr (7);
}
