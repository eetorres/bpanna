
#ifndef _WINDEF_H_
#define _WINDEF_H_

// Def WIN32 or Def WIN64
#if defined(WIN32) || defined(WIN64)
#define strcasecmp _stricmp
#define snprintf   _snprintf
#endif

#endif /* WINDEF_INCLUDED */

