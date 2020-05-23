/*
  iGameStrings_cat.c
  Translatable strings for iGame
  
  Copyright (c) 2018, Emmanuel Vasilakis
  
  This file is part of iGame.

  iGame is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  iGame is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with iGame. If not, see <http://www.gnu.org/licenses/>.
*/

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

#include <proto/locale.h>

#include <stdlib.h>
#include <string.h>

#include "iGameStrings_cat.h"


/*
    Variables
*/

struct FC_String iGameStrings_Strings[100] = {
    { (STRPTR) "A front-end to WHDLoad", 0 },
    { (STRPTR) "Emmanuel Vasilakis, Dimitris Panokostas", 1 },
    { (STRPTR) "iGame", 2 },
    { (STRPTR) "Actions", 3 },
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
    { (STRPTR) "Game", 18 },
    { (STRPTR) "Duplicate...", 19 },
    { (STRPTR) "Properties...", 20 },
    { (STRPTR) "P_", 21 },
    { (STRPTR) "Delete", 22 },
    { (STRPTR) "D_", 23 },
    { (STRPTR) "Settings", 24 },
    { (STRPTR) "Settings...", 25 },
    { (STRPTR) "Game Repositories...", 26 },
    { (STRPTR) "MUI Settings...", 27 },
    { (STRPTR) "Filter:", 28 },
    { (STRPTR) "Genres", 29 },
    { (STRPTR) "Game Properties", 30 },
    { (STRPTR) "Title", 31 },
    { (STRPTR) "Genre:", 32 },
    { (STRPTR) "Unknown", 33 },
    { (STRPTR) "Favorite:", 34 },
    { (STRPTR) "Hidden:", 35 },
    { (STRPTR) "Times Played:", 36 },
    { (STRPTR) "Slave Path:", 37 },
    { (STRPTR) "Tooltypes", 38 },
    { (STRPTR) "OK", 39 },
    { (STRPTR) "Cancel", 40 },
    { (STRPTR) "Game Repositories", 41 },
    { (STRPTR) "Add", 42 },
    { (STRPTR) "Remove", 43 },
    { (STRPTR) "Close", 44 },
    { (STRPTR) "Add a non-WHDLoad Game", 45 },
    { (STRPTR) "Title:", 46 },
    { (STRPTR) "Path:", 47 },
    { (STRPTR) "Genre:", 48 },
    { (STRPTR) "Unknown", 49 },
    { (STRPTR) "OK", 50 },
    { (STRPTR) "Cancel", 51 },
    { (STRPTR) "About iGame", 52 },
    { (STRPTR) "Emmanuel Vasilakis (mrzammler@mm.st)\n\nContributors:\nDimitris Panokostas (midwan@gmail.com)\nGeorge Sokianos (walkero@gmail.com)\nChris Charabaruk (chris+igame@charabaruk.com)\n\n", 53 },
    { (STRPTR) "OK", 54 },
    { (STRPTR) "iGame Settings", 55 },
    { (STRPTR) "Hide Screenshots", 56 },
    { (STRPTR) "Screenshots", 57 },
    { (STRPTR) "No GuiGfx", 58 },
    { (STRPTR) "Screenshot Size:", 59 },
    { (STRPTR) "160x128", 60 },
    { (STRPTR) "320x256", 61 },
    { (STRPTR) "Custom", 62 },
    { (STRPTR) "Width", 63 },
    { (STRPTR) "Height", 64 },
    { (STRPTR) "Titles", 65 },
    { (STRPTR) "Titles From:", 66 },
    { (STRPTR) "Slave Contents", 67 },
    { (STRPTR) "Directories", 68 },
    { (STRPTR) "No Smart Spaces", 69 },
    { (STRPTR) "Misc", 70 },
    { (STRPTR) "Save stats on exit", 71 },
    { (STRPTR) "Use enter to filter", 72 },
    { (STRPTR) "Hide side panel", 73 },
    { (STRPTR) "Save", 74 },
    { (STRPTR) "Use", 75 },
    { (STRPTR) "Cancel", 76 },
    { (STRPTR) "Select dir...", 77},
    { (STRPTR) "Select Game executable...", 78},
    { (STRPTR) "Total %d games.", 79},
    { (STRPTR) "Unknown", 80},
    { (STRPTR) "--Show All--", 81},
    { (STRPTR) "--Favorites--", 82},
    { (STRPTR) "--Last Played--", 83},
    { (STRPTR) "--Most Played--", 84},
    { (STRPTR) "--Never Played--", 85},
    { (STRPTR) "Running %s...", 86},
    { (STRPTR) "Error while executing WHDLoad or Game.\nPlease make sure WHDLoad is in your path.", 87},
    { (STRPTR) "Scanning [%s]. Please wait...", 88},
    { (STRPTR) "Could not create repos.prefs file!", 89},
    { (STRPTR) "Please select a Game from the List.", 90},
    { (STRPTR) "The Title you selected, already exists.", 91},
    { (STRPTR) "Please wait, saving gameslist...", 92},
    { (STRPTR) "Could not open gameslist file!", 93},
    { (STRPTR) "Bad tooltype!", 94},
    { (STRPTR) "Please let me know the name of the game...", 95},
    { (STRPTR) "Please pick the game executable...", 96},
    { (STRPTR) "Could not allocate memory! Aborting...", 97},
    { (STRPTR) "Game Directory not found!", 98},
    { (STRPTR) "Display favorites on start", 99}
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

	    for (i = 0, fc = iGameStrings_Strings;  i < 100;  i++, fc++) {
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

				    for (i = 0, fc = iGameStrings_Strings;  i < 82;  i++, fc++) {
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
