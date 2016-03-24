
/**********************************************
 *                                            *
 * gotoxy works just like the old Borland C   *
 * gotoxy function. gotoxy ( x, y ). y is the *
 * line number starting at 0, and x is the    *
 * column number. For win32 only, of course.  *
 *                                            *
 * 06/04/2013 original by WH Minter           *
 **********************************************/

#include <windows.h>

COORD   coord = { 0, 0 };       /* sets coordinates to 0,0 */

void gotoxy (int x, int y)
{
  coord.X = x;
  coord.Y = y;                  /* X and Y coordinates */

  SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
}
