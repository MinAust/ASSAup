ASSAup is a Windows program that converts .ass (Advanced SubStation Alpha)
subtitles files between PAL and NTSC rates. This is a command line progran,
so if you are "command line impaired", don't bother with this one. it is
meant to be run from batch files for mass conversions. It can be compiled
with Visual Studio 2015 or MinGW.

At the command prompt, just enter 'assaup Movie.ass [options]' (assuming
Movie.ass is a PAL subtitle), and you'll have two files: Movie.PAL.ass and
Movie.ass. Movie.PAL.ass is your original PAL subtitle, unaltered, just
renamed. Movie.ass is the new file, timing changed to the NTSC rate.

OPTIONS:
-nd  no detail print
-nb  no backup file
-np  NTSC to PAL conversion - PAL to NTSC is the default. If used, the
     backup file will be named Movie.NTSC.Ass.

Multiple options must be seperated by a space.
