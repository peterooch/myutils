//should be placed in ReactOS gdi folder and be linked in ExtTextOutW function
#include "precomp.h"
#include "usp10.h"
/*in lieu of the wine function, this function for now receives the relevant string and does the magic on it(hopefully)
for now returns FALSE if no reordring was done or it failed(most likely) or TRUE if the function succeeds
lpString = the to be bidified string(or not)
uCount = count of the characters in the string.
*/
BOOL BiDi_ReOrder(LPWSTR lpString, UINT uCount)
{
LPWSTR workstr = lpSting; //the "frankenstring"
//check if the string needs treatment, if none required it can proceed to the next "thunk"
if (ScriptIsComplex(lpString, uCount, SIC_COMPLEX) == S_FALSE)
    return FALSE;
  //following msdn guidelines...
  const SCRIPT_PROPERTIES **ppScriptProperties;
  int iMaxScript;
  const int cMaxItems = 256;
  SCRIPT_ITEM si[cMaxItems + 1];
  SCRIPT_ITEM *pItems = si;
  int cItems;
  SCRIPT_CACHE cache = NULL;

  if(!ScriptGetProperties(&ppScriptProperties,&iMaxScript))
    return FALSE; //incase of disaster
  if(!ScriptItemize(workstr, uCount, cMaxItems, NULL, NULL, pItems, cItems))
    return FALSE; //same thing

    //the moment of truth
  if(!ScriptShape(NULL, cache, lpString, uCount, 256, pItems.a, 256, workstr, )) //function still incomplete...


}
/* or use ScriptTextOut(function code) in ExtTextOutW then call NtGdiExtTextOutW with the repurposed vars... eureka much */
