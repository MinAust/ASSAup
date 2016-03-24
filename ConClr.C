#include <windows.h>

int conclr (int icolor)
{
  HANDLE  hConsole;

  hConsole = GetStdHandle (STD_OUTPUT_HANDLE);

  SetConsoleTextAttribute (hConsole, icolor);
  return 0;
}
