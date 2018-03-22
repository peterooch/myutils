/*win32ss/gdi/gdi32/objects/text.c */
/*dll/win32/usp10/usp10.c */
/* reconfigured ScriptTextOut to ExtTextOutW... who would belive that */
#include <windows.h>
#include <usp10.h>
BOOL
WINAPI
ExtTextOutW(
    _In_ HDC hdc,
    _In_ INT x,
    _In_ INT y,
    _In_ UINT fuOptions,
    _In_opt_ const RECT *lprc,
    _In_reads_opt_(cwc) LPCWSTR lpString,
    _In_ UINT cwc,
    _In_reads_opt_(cwc) const INT *lpDx)
{
    //what this means?
    HANDLE_METADC(BOOL,
                  ExtTextOut,
                  FALSE,
                  hdc,
                  x,
                  y,
                  fuOptions,
                  lprc,
                  lpString,
                  cwc,
                  lpDx);
//Add here preceding usp10 functions before manipulation
if (ScriptIsComplex(lpString, uCount, SIC_COMPLEX) == S_FALSE) //bypass completely if not needed
      return NtGdiExtTextOutW(hdc, x,  y,  fuOptions,  (LPRECT)lprc,  (LPWSTR)lpString,  cwc,  (LPINT)lpDx,  0);

SCRIPT_CONTROL sControl;
SCRIPT_ANALYSIS *psa;
GOFFSET pGoffset[cwc];
SCRIPT_CACHE *psc = NULL;
SCRIPT_STRING_ANALYSIS *pssa = NULL;

if(ScriptPlace(hdc, psc, ) != S_OK) //need to find Goffsets
      return NtGdiExtTextOutW(hdc, x,  y,  fuOptions,  (LPRECT)lprc,  (LPWSTR)lpString,  cwc,  (LPINT)lpDx,  0);
if(ScriptStringAnalyse(hdc, lpString, cwc, (1.5 * sizeof(lpString) + 16), -1, SSA_RTL, lprc.right - lprc.left, NULL, NULL, NULL, NULL, ) != S_OK) //function needs to be fully arged
      return NtGdiExtTextOutW(hdc, x,  y,  fuOptions,  (LPRECT)lprc,  (LPWSTR)lpString,  cwc,  (LPINT)lpDx,  0); //admitting defeat
                  //now manipulate this so at the end we can show legit bidi text
                  //commented out args mean they are not used in the function
                  HRESULT WINAPI ScriptTextOut(/*const HDC hdc, SCRIPT_CACHE *psc, int x, int y, UINT fuOptions,*/
                                               /*const RECT *lprc*/, const SCRIPT_ANALYSIS *psa, /*const WCHAR *pwcReserved,
                                               int iReserved*/, /* const WORD *lpString, int cwc */, /*const int *piAdvance,*/
                                               /*const int *piJustify*/, const GOFFSET *pGoffset) //so need to configure/find psc,psa,piAdvance, pGoffset

                      HRESULT hr = S_OK;
                      INT i, dir = 1;
                      //INT *lpDx;
                      WORD *reordered_glyphs = (WORD *)lpString;

                      //if (!hdc || !psc) return E_INVALIDARG;
                      //if (!piAdvance || !psa || !lpString) return E_INVALIDARG;

                      fuOptions &= ETO_CLIPPED + ETO_OPAQUE;
                      fuOptions |= ETO_IGNORELANGUAGE;
                      if  (!psa->fNoGlyphIndex)                                     /* Have Glyphs?                      */
                          fuOptions |= ETO_GLYPH_INDEX;                             /* Say don't do translation to glyph */

                      //lpDx = heap_alloc(cwc * sizeof(INT) * 2);
                      if (!lpDx) return E_OUTOFMEMORY;
                      fuOptions |= ETO_PDY;

                      if (psa->fRTL && psa->fLogicalOrder)
                      {
                          reordered_glyphs = heap_alloc( cwc * sizeof(WORD) );
                          if (!reordered_glyphs)
                          {
                              //heap_free( lpDx );
                              return E_OUTOFMEMORY;
                          }

                          for (i = 0; i < cwc; i++)
                              reordered_glyphs[i] = lpString[cwc - 1 - i];
                          dir = -1;
                      }

                      for (i = 0; i < cwc; i++)
                      {
                          int orig_index = (dir > 0) ? i : cwc - 1 - i;
                          lpDx[i * 2] = 8;
                          lpDx[i * 2 + 1] = 0;

                          if (pGoffset)
                          {
                              if (i == 0)
                              {
                                  x += pGoffset[orig_index].du * dir;
                                  y += pGoffset[orig_index].dv;
                              }
                              else
                              {
                                  lpDx[(i - 1) * 2]     += pGoffset[orig_index].du * dir;
                                  lpDx[(i - 1) * 2 + 1] += pGoffset[orig_index].dv;
                              }
                              lpDx[i * 2]     -= pGoffset[orig_index].du * dir;
                              lpDx[i * 2 + 1] -= pGoffset[orig_index].dv;
                          }
                      }

                      if (!NtGdiExtTextOutW(hdc, x, y, fuOptions, (LPRECT)lprc, (LPWSTR)reordered_glyphs, cwc, (LPINT)lpDx, 0))
                        hr = S_FALSE;

                      if (reordered_glyphs != lpString) heap_free( reordered_glyphs );
                      //heap_free(lpDx);

                      return hr;

}
