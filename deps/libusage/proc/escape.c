/*
 * escape.c - printing handling
 * Copyright 1998-2002 by Albert Cahalan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "procps.h"
#include "escape.h"
#include "readproc.h"

#if (__GNU_LIBRARY__ >= 6) && (!defined(__UCLIBC__) || defined(__UCLIBC_HAS_WCHAR__))
# include <wchar.h>
# include <wctype.h>
# include <stdlib.h>  /* MB_CUR_MAX */
# include <ctype.h>
# include <langinfo.h>
#endif

#if (__GNU_LIBRARY__ >= 6) && (!defined(__UCLIBC__) || defined(__UCLIBC_HAS_WCHAR__))
static int escape_str_utf8(char *restrict dst, const char *restrict src, int bufsize, int *maxcells){
  int my_cells = 0;
  int my_bytes = 0;
  mbstate_t s;
  
  memset(&s, 0, sizeof (s));
  
  for(;;) {
    wchar_t wc;
    int len = 0;
	  
    if(my_cells >= *maxcells || my_bytes+1 >= bufsize) 
      break;
    
    if (!(len = mbrtowc (&wc, src, MB_CUR_MAX, &s)))
      /* 'str' contains \0 */
      break;
    
    if (len < 0) {
      /* invalid multibyte sequence -- zeroize state */
      memset (&s, 0, sizeof (s));
      *(dst++) = '?';
      src++;
      my_cells++; 
      my_bytes++;

    } else if (!iswprint(wc)) {
      /* multibyte - no printable */
      *(dst++) = '?';
      src+=len;
      my_cells++;
      my_bytes++; 
    
    } else {
      /* multibyte - printable */	
      int wlen = wcwidth(wc);

      if (wlen==0) {
	// invisible multibyte -- we don't ignore it, because some terminal 
	// interpret it wrong and more safe is replace it with '?'
	*(dst++) = '?';
	src+=len;
	my_cells++;
	my_bytes++;
      } else {
        // multibyte - printable
        // Got space?
        if (my_cells+wlen > *maxcells || my_bytes+1+len >= bufsize) break;
        // 0x9b is control byte for some terminals
        if (memchr(src, 0x9B, len)) {
	  // unsafe multibyte
	  *(dst++) = '?';
	  src+=len;
	  my_cells++;
	  my_bytes++;
        } else {
	  // safe multibyte
       	  memcpy(dst, src, len);
	  my_cells += wlen;
	  dst += len;
	  my_bytes += len;
          src += len;
        }
      }
    }
    //fprintf(stdout, "cells: %d\n", my_cells);
  }
  *dst = '\0';

  // fprintf(stderr, "maxcells: %d, my_cells; %d\n", *maxcells, my_cells);
  
  *maxcells -= my_cells;
  return my_bytes;        // bytes of text, excluding the NUL
}

#endif /* __GNU_LIBRARY__  */

/* sanitize a string via one-way mangle */
int escape_str(char *restrict dst, const char *restrict src, int bufsize, int *maxcells){
  unsigned char c;
  int my_cells = 0;
  int my_bytes = 0;
  const char codes[] =
  "Z..............................."
  "||||||||||||||||||||||||||||||||"
  "||||||||||||||||||||||||||||||||"
  "|||||||||||||||||||||||||||||||."
  "????????????????????????????????"
  "????????????????????????????????"
  "????????????????????????????????"
  "????????????????????????????????";
  
#if (__GNU_LIBRARY__ >= 6) && (!defined(__UCLIBC__) || defined(__UCLIBC_HAS_WCHAR__))
  static int utf_init=0;
  
  if(utf_init==0){
     /* first call -- check if UTF stuff is usable */
     char *enc = nl_langinfo(CODESET);
     utf_init = enc && strcasecmp(enc, "UTF-8")==0 ? 1 : -1;
  }
  if (utf_init==1 && MB_CUR_MAX>1) {
     /* UTF8 locales */
     return escape_str_utf8(dst, src, bufsize, maxcells);
  }
#endif
		  
  if(bufsize > *maxcells+1) bufsize=*maxcells+1; // FIXME: assumes 8-bit locale

  for(;;){
    if(my_cells >= *maxcells || my_bytes+1 >= bufsize) 
      break;
    c = (unsigned char) *(src++);
    if(!c) break;
    if(codes[c]!='|') c=codes[c];
    my_cells++;
    my_bytes++;
    *(dst++) = c;
  }
  *dst = '\0';
  
  *maxcells -= my_cells;
  return my_bytes;        // bytes of text, excluding the NUL
}

/////////////////////////////////////////////////

// escape an argv or environment string array
//
// bytes arg means sizeof(buf)
int escape_strlist(char *restrict dst, char *restrict const *restrict src, size_t bytes, int *cells){
  size_t i = 0;

  for(;;){
    i += escape_str(dst+i, *src, bytes-i, cells);
    if(bytes-i < 3) break;  // need room for space, a character, and the NUL
    src++;
    if(!*src) break;  // need something to print
    if (*cells<=1) break;  // need room for printed size of text
    dst[i++] = ' ';
    --*cells;
  }
  return i;    // bytes, excluding the NUL
}

///////////////////////////////////////////////////

int escape_command(char *restrict const outbuf, const proc_t *restrict const pp, int bytes, int *cells, unsigned flags){
  int overhead = 0;
  int end = 0;

  if(flags & ESC_ARGS){
    char **lc = (char**)pp->cmdline;
    if(lc && *lc) return escape_strlist(outbuf, lc, bytes, cells);
  }
  if(flags & ESC_BRACKETS){
    overhead += 2;
  }
  if(flags & ESC_DEFUNCT){
    if(pp->state=='Z') overhead += 10;    // chars in " <defunct>"
    else flags &= ~ESC_DEFUNCT;
  }
  if(overhead + 1 >= *cells){  // if no room for even one byte of the command name
    // you'd damn well better have _some_ space
//    outbuf[0] = '-';  // Oct23
    outbuf[1] = '\0';
    return 1;
  }
  if(flags & ESC_BRACKETS){
    outbuf[end++] = '[';
  }
  *cells -= overhead;
  end += escape_str(outbuf+end, pp->cmd, bytes-overhead, cells);

  // Hmmm, do we want "[foo] <defunct>" or "[foo <defunct>]"?
  if(flags & ESC_BRACKETS){
    outbuf[end++] = ']';
  }
  if(flags & ESC_DEFUNCT){
    memcpy(outbuf+end, " <defunct>", 10);
    end += 10;
  }
  outbuf[end] = '\0';
  return end;  // bytes, not including the NUL
}

/////////////////////////////////////////////////

// copy an already 'escaped' string,
// using the traditional escape.h calling conventions
int escaped_copy(char *restrict dst, const char *restrict src, int bufsize, int *maxroom){
  int n;
  if (bufsize > *maxroom+1) bufsize = *maxroom+1;
  n = snprintf(dst, bufsize, "%s", src);
  if (n >= bufsize) n = bufsize-1;
  *maxroom -= n;
  return n;
}
