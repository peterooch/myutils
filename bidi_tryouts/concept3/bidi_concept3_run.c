#include "bidi_concept3.h"
#include <string.h>
#include <stdio.h>

int wmain()
{
  LPCWSTR teststring = L"Hello עולם!";
  LPWSTR result[12]; //result
  wprintf(L"Original String %s\n", teststring);

  if (!BiDi_ReOrder(teststring, 12, result))
  wprintf(L"Error occured");

  else
  wprintf(L"Result String %s", result); //hebrew chars should look reversed

  WCHAR dunno = getwchar(); //to pause the program
  if (dunno)
	  return 1;
}
