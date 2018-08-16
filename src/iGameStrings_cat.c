
/****************************************************************

   This file was created automatically by `FlexCat 2.18'
   from "iGameStrings.cd".

   Do NOT edit by hand!

****************************************************************/

/****************************************************************
    This file uses the auto initialization features of
    Dice, gcc and SAS/C, respectively.

    Dice does this by using the __autoinit and __autoexit
    keywords, whereas SAS/C uses names beginning with _STI
    or _STD, respectively. gcc uses the asm() instruction
    to emulate C++ constructors and destructors.

    Using this file you don't have *all* the benefits of
    locale.library (no Locale or Language arguments are
    supported when opening the catalog). However, these are
    *very* rarely used, so this should be sufficient for most
    applications.
****************************************************************/

/*
    Include files and compiler specific stuff
*/

#include <exec/memory.h>
#include <libraries/locale.h>
#include <libraries/iffparse.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/locale.h>
#include <proto/utility.h>
#include <proto/iffparse.h>

#include <stdlib.h>
#include <string.h>



#include "iGameStrings_cat.h"


/*
    Variables
*/

struct FC_String iGameStrings_Strings[60] = {
    { (STRPTR) "A front-end to WHDLoad", 0 },
    { (STRPTR) "Dimitris Panokostas 2018", 1 },
    { (STRPTR) "iGame", 2 },
    { (STRPTR) "File", 3 },
    { (STRPTR) "Scan Repositories", 4 },
    { (STRPTR) "R_", 5 },
    { (STRPTR) "Add non-WHDLoad game...", 6 },
    { (STRPTR) "A_", 7 },
    { (STRPTR) "Show/Hide hidden entries", 8 },
    { (STRPTR) "Open List...", 9 },
    { (STRPTR) "O_", 10 },
    { (STRPTR) "Save List", 11 },
    { (STRPTR) "S_", 12 },
    { (STRPTR) "Save List As...", 13 },
    { (STRPTR) "Export List to Text file...", 14 },
    { (STRPTR) "About...", 15 },
    { (STRPTR) "Quit", 16 },
    { (STRPTR) "Q_", 17 },
    { (STRPTR) "Edit", 18 },
    { (STRPTR) "Copy", 19 },
    { (STRPTR) "C_", 20 },
    { (STRPTR) "Paste as New...", 21 },
    { (STRPTR) "V_", 22 },
    { (STRPTR) "Delete", 23 },
    { (STRPTR) "D_", 24 },
    { (STRPTR) "Properties...", 25 },
    { (STRPTR) "P_", 26 },
    { (STRPTR) "Settings", 27 },
    { (STRPTR) "Settings...", 28 },
    { (STRPTR) "Game Repositories...", 29 },
    { (STRPTR) "MUI Settings...", 30 },
    { (STRPTR) "Filter:", 31 },
    { (STRPTR) "Genres", 32 },
    { (STRPTR) "Game Properties", 33 },
    { (STRPTR) "Title", 34 },
    { (STRPTR) "Genre:", 35 },
    { (STRPTR) "Unknown", 36 },
    { (STRPTR) "Favorite:", 37 },
    { (STRPTR) "Hidden:", 38 },
    { (STRPTR) "Times Played:", 39 },
    { (STRPTR) "Slave Path:", 40 },
    { (STRPTR) "Tooltypes", 41 },
    { (STRPTR) "OK", 42 },
    { (STRPTR) "Cancel", 43 },
    { (STRPTR) "Game Repositories", 44 },
    { (STRPTR) "Add", 45 },
    { (STRPTR) "Remove", 46 },
    { (STRPTR) "Close", 47 },
    { (STRPTR) "Add a non-WHDLoad Game", 48 },
    { (STRPTR) "Title:", 49 },
    { (STRPTR) "Path:", 50 },
    { (STRPTR) "Genre:", 51 },
    { (STRPTR) "Unknown", 52 },
    { (STRPTR) "OK", 53 },
    { (STRPTR) "Cancel", 54 },
    { (STRPTR) "About iGame", 55 },
    { (STRPTR) "iGame version 1.7 beta", 56 },
    { (STRPTR) "OK", 57 },
    { (STRPTR) "iGame Settings", 58 },
    { (STRPTR) "Nothing here yet!", 59 }
};

STATIC struct Catalog *iGameStringsCatalog = NULL;
#ifdef LOCALIZE_V20
STATIC STRPTR iGameStringsStrings = NULL;
STATIC ULONG iGameStringsStringsSize;
#endif


#if defined(_DCC)
STATIC __autoexit VOID _STDCloseiGameStringsCatalog(VOID)
#elif defined(__SASC)
VOID _STDCloseiGameStringsCatalog(VOID)
#elif defined(__GNUC__)
STATIC VOID __attribute__ ((destructor)) _STDCloseiGameStringsCatalog(VOID)
#else
VOID CloseiGameStringsCatalog(VOID)
#endif

{
    if (iGameStringsCatalog) {
	CloseCatalog(iGameStringsCatalog);
    }
#ifdef LOCALIZE_V20
    if (iGameStringsStrings) {
	FreeMem(iGameStringsStrings, iGameStringsStringsSize);
    }
#endif
}


#if defined(_DCC)
STATIC __autoinit VOID _STIOpeniGameStringsCatalog(VOID)
#elif defined(__SASC)
VOID _STIOpeniGameStringsCatalog(VOID)
#elif defined(__GNUC__)
VOID __attribute__ ((constructor)) _STIOpeniGameStringsCatalog(VOID)
#else
VOID OpeniGameStringsCatalog(VOID)
#endif

{
    if (LocaleBase) {
	if ((iGameStringsCatalog = OpenCatalog(NULL, (STRPTR) "iGameStrings.catalog",
				     OC_BuiltInLanguage, "english",
				     OC_Version, -1,
				     TAG_DONE))) {
	    struct FC_String *fc;
	    int i;

	    for (i = 0, fc = iGameStrings_Strings;  i < 60;  i++, fc++) {
		 fc->msg = GetCatalogStr(iGameStringsCatalog, fc->id, (STRPTR) fc->msg);
	    }
	}
    }
}




#ifdef LOCALIZE_V20
VOID InitiGameStringsCatalog(STRPTR language)

{
    struct IFFHandle *iffHandle;

    /*
    **  Use iffparse.library only, if we need to.
    */
    if (LocaleBase  ||  !IFFParseBase  ||  !language  ||
	Stricmp(language, "english") == 0) {
	return;
    }

    if ((iffHandle = AllocIFF())) {
	char path[128];  /* Enough to hold 4 path items (dos.library V40) */
	strcpy(path, "PROGDIR:Catalogs");
	AddPart((STRPTR) path, language, sizeof(path));
	AddPart((STRPTR) path, "iGameStrings.catalog", sizeof(path));
	if (!(iffHandle->iff_Stream = Open((STRPTR) path, MODE_OLDFILE))) {
	    strcpy(path, "LOCALE:Catalogs");
	    AddPart((STRPTR) path, language, sizeof(path));
	    AddPart((STRPTR) path, language, sizeof(path));
	    iffHandle->iff_Stream = Open((STRPTR) path, MODE_OLDFILE);
	}

	if (iffHandle->iff_Stream) {
	    InitIFFasDOS(iffHandle);
	    if (!OpenIFF(iffHandle, IFFF_READ)) {
		if (!PropChunk(iffHandle, MAKE_ID('C','T','L','G'),
			       MAKE_ID('S','T','R','S'))) {
		    struct StoredProperty *sp;
		    int error;

		    for (;;) {
			if ((error = ParseIFF(iffHandle, IFFPARSE_STEP))
				   ==  IFFERR_EOC) {
			    continue;
			}
			if (error) {
			    break;
			}

			if ((sp = FindProp(iffHandle, MAKE_ID('C','T','L','G'),
					   MAKE_ID('S','T','R','S')))) {
			    /*
			    **  Check catalog and calculate the needed
			    **  number of bytes.
			    **  A catalog string consists of
			    **      ID (LONG)
			    **      Size (LONG)
			    **      Bytes (long word padded)
			    */
			    LONG bytesRemaining;
			    LONG *ptr;

			    iGameStringsStringsSize = 0;
			    bytesRemaining = sp->sp_Size;
			    ptr = (LONG *) sp->sp_Data;

			    while (bytesRemaining > 0) {
				LONG skipSize, stringSize;

				ptr++;                  /* Skip ID */
				stringSize = *ptr++;
				skipSize = ((stringSize+3) >> 2);

				iGameStringsStringsSize += stringSize+1;  /* NUL */
				bytesRemaining -= 8 + (skipSize << 2);
				ptr += skipSize;
			    }

			    if (!bytesRemaining  &&
				(iGameStringsStrings = AllocMem(iGameStringsStringsSize, MEMF_ANY))) {
				STRPTR sptr;

				bytesRemaining = sp->sp_Size;
				ptr = (LONG *) sp->sp_Data;
				sptr = iGameStringsStrings;

				while (bytesRemaining) {
				    LONG skipSize, stringSize, id;
				    struct FC_String *fc;
				    int i;

				    id = *ptr++;
				    stringSize = *ptr++;
				    skipSize = ((stringSize+3) >> 2);

				    CopyMem(ptr, sptr, stringSize);
				    bytesRemaining -= 8 + (skipSize << 2);
				    ptr += skipSize;

				    for (i = 0, fc = iGameStrings_Strings;  i < 60;  i++, fc++) {
					if (fc->id == id) {
					    fc->msg = sptr;
					}
				    }

				    sptr += stringSize;
				    *sptr++ = '\0';
				}
			    }
			    break;
			}
		    }
		}
		CloseIFF(iffHandle);
	    }
	    Close(iffHandle->iff_Stream);
	}
	FreeIFF(iffHandle);
    }
}
#endif
