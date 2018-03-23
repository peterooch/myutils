#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "bidi_concept3.h"

int wmain()
{
  LPWSTR teststring = L"Hello עולם!";
  LPWSTR result[12]; //result
  wprintf(L"Original String %s", teststring);
  BiDi_ReOrder(teststring, 12, result);
  wprintf(L"Result String %s", result); //hebrew chars should look reversed
}
