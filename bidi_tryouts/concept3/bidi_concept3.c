#include <windows.h>
#include "bidi_concept3.h"
#define LC 0 //Left aligned character
#define RC 1 //Right aligned character
#define NC 2 //Neutral character

/* This function receives a pointer to a string to be reordered, its length
and a pointer to a empty string that should receive the reordered string */
BOOL BiDi_ReOrder(LPWSTR lpString, UINT uCount, LPWSTR lpnewString)
{
  LPWSTR workstr = NULL;
  LPWSTR line = NULL;
  UINT len = 0;
  for(UINT i = 0; i <= uCount ; i++ , len++)
  {
    wcscat(line, lpString[i]);
    if(lpString[i] == L"\n" || lpString[i] == L"\0")
    {
      UINT *dirs = NULL;
      analyseLine(line, dirs, len);
      if(!reorderLine(line, dirs, len))
        return FALSE;
      wcscat(workstr, line);
      line = NULL;
      len = 0;
    }
  }
  wcscpy(lpnewString, workstr);
  return TRUE;
}
//checks each character and puts its type in its index number in dirs
UINT analyseLine(LPWSTR line, UINT *dirs, UINT len)
{
  for (UINT i = 0; i <= len; i++)
  {
    dirs[i] = checkChar(line[i]);
  }
  return TRUE;//Visual studio nag
}
//Returns LL, RL, NL for checked character
UINT checkChar(WCHAR wch)
{
  //for now only checks for hebrew characters
	if (wch < 0x05F4 && wch > 0x0591)
	{
		return RC;
	}
	else
	{
		return LC;
	}
  //return NL;
}
//Magic reordering should happen here
BOOL reorderLine(LPWSTR line, UINT *dirs, UINT len)
{
  LPWSTR workline = NULL;
  /*UINT charCount = 0;
  UINT lineDone = 0; 
  uneeded for some reason ^ */
  for(UINT i = 0; i <= len;)
  {
	  if (dirs[i] == LC)
	  {
		  workline[i] = line[i];
		  i++;
	  }
    else if (dirs[i] == RC)
    {
      UINT startOff = i;
      UINT lastRC = calcRCs(dirs, startOff);
      //reverse copy
      for(UINT startRC = startOff + lastRC; startOff == lastRC; startRC--)
      {
        workline[i] = line[startRC];
        i++;
      }
    }
    else
    {
      //if something terrifying happens
      return FALSE;
    }
  }
  wcscpy(line, workline);
  return TRUE;
}
//Calculate the position of the last RC in the given sequence
UINT calcRCs(UINT *dirs, UINT startLook)
{
    UINT res = startLook;
    while(dirs[res] == RC)
    {
      res++;
    }
    res--;
    return res;
}
