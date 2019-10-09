/*
  iGameStrings_cat.h
  Header strings file for iGame
  
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

#ifndef iGameStrings_CAT_H
#define iGameStrings_CAT_H


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif


/*
**  Prototypes
*/
#if !defined(__GNUC__)  && !defined(__SASC)  &&  !defined(_DCC)
extern VOID OpeniGameStringsCatalog(VOID);
extern VOID CloseiGameStringsCatalog(VOID);
#endif
#ifdef LOCALIZE_V20
extern void InitiGameStringsCatalog(STRPTR);
#endif



struct FC_String {
    const UBYTE *msg;
    LONG id;
};

extern struct FC_String iGameStrings_Strings[102];

#define MSG_AppDescription (iGameStrings_Strings[0].msg)
#define _MSG_AppDescription (iGameStrings_Strings+0)
#define MSG_AppCopyright (iGameStrings_Strings[1].msg)
#define _MSG_AppCopyright (iGameStrings_Strings+1)
#define MSG_WI_MainWindow (iGameStrings_Strings[2].msg)
#define _MSG_WI_MainWindow (iGameStrings_Strings+2)
#define MSG_MNlabel2Actions (iGameStrings_Strings[3].msg)
#define _MSG_MNlabel2Actions (iGameStrings_Strings+3)
#define MSG_MNlabelScan (iGameStrings_Strings[4].msg)
#define _MSG_MNlabelScan (iGameStrings_Strings+4)
#define MSG_MNlabelScanChar (iGameStrings_Strings[5].msg)
#define _MSG_MNlabelScanChar (iGameStrings_Strings+5)
#define MSG_MNMainAddnonWHDLoadgame (iGameStrings_Strings[6].msg)
#define _MSG_MNMainAddnonWHDLoadgame (iGameStrings_Strings+6)
#define MSG_MNMainAddnonWHDLoadgameChar (iGameStrings_Strings[7].msg)
#define _MSG_MNMainAddnonWHDLoadgameChar (iGameStrings_Strings+7)
#define MSG_MNMainMenuShowHidehiddenentries (iGameStrings_Strings[8].msg)
#define _MSG_MNMainMenuShowHidehiddenentries (iGameStrings_Strings+8)
#define MSG_MNMainOpenList (iGameStrings_Strings[9].msg)
#define _MSG_MNMainOpenList (iGameStrings_Strings+9)
#define MSG_MNMainOpenListChar (iGameStrings_Strings[10].msg)
#define _MSG_MNMainOpenListChar (iGameStrings_Strings+10)
#define MSG_MNMainSaveList (iGameStrings_Strings[11].msg)
#define _MSG_MNMainSaveList (iGameStrings_Strings+11)
#define MSG_MNMainSaveListChar (iGameStrings_Strings[12].msg)
#define _MSG_MNMainSaveListChar (iGameStrings_Strings+12)
#define MSG_MNMainSaveListAs (iGameStrings_Strings[13].msg)
#define _MSG_MNMainSaveListAs (iGameStrings_Strings+13)
#define MSG_MNMainExportListtoTextfile (iGameStrings_Strings[14].msg)
#define _MSG_MNMainExportListtoTextfile (iGameStrings_Strings+14)
#define MSG_MNMainAbout (iGameStrings_Strings[15].msg)
#define _MSG_MNMainAbout (iGameStrings_Strings+15)
#define MSG_MNMainQuit (iGameStrings_Strings[16].msg)
#define _MSG_MNMainQuit (iGameStrings_Strings+16)
#define MSG_MNMainQuitChar (iGameStrings_Strings[17].msg)
#define _MSG_MNMainQuitChar (iGameStrings_Strings+17)
#define MSG_MNlabel2Game (iGameStrings_Strings[18].msg)
#define _MSG_MNlabel2Game (iGameStrings_Strings+18)
#define MSG_MNMainMenuDuplicate (iGameStrings_Strings[19].msg)
#define _MSG_MNMainMenuDuplicate (iGameStrings_Strings+19)
#define MSG_MNMainProperties (iGameStrings_Strings[20].msg)
#define _MSG_MNMainProperties (iGameStrings_Strings+20)
#define MSG_MNMainPropertiesChar (iGameStrings_Strings[21].msg)
#define _MSG_MNMainPropertiesChar (iGameStrings_Strings+21)
#define MSG_MNMainDelete (iGameStrings_Strings[22].msg)
#define _MSG_MNMainDelete (iGameStrings_Strings+22)
#define MSG_MNMainDeleteChar (iGameStrings_Strings[23].msg)
#define _MSG_MNMainDeleteChar (iGameStrings_Strings+23)
#define MSG_MNlabel2Tools (iGameStrings_Strings[24].msg)
#define _MSG_MNlabel2Tools (iGameStrings_Strings+24)
#define MSG_MNMainiGameSettings (iGameStrings_Strings[25].msg)
#define _MSG_MNMainiGameSettings (iGameStrings_Strings+25)
#define MSG_MNlabel2GameRepositories (iGameStrings_Strings[26].msg)
#define _MSG_MNlabel2GameRepositories (iGameStrings_Strings+26)
#define MSG_MNMainMUISettings (iGameStrings_Strings[27].msg)
#define _MSG_MNMainMUISettings (iGameStrings_Strings+27)
#define MSG_LA_Filter (iGameStrings_Strings[28].msg)
#define _MSG_LA_Filter (iGameStrings_Strings+28)
#define MSG_LV_GenresListTitle (iGameStrings_Strings[29].msg)
#define _MSG_LV_GenresListTitle (iGameStrings_Strings+29)
#define MSG_WI_Properties (iGameStrings_Strings[30].msg)
#define _MSG_WI_Properties (iGameStrings_Strings+30)
#define MSG_STR_PropertiesGameTitleTitle (iGameStrings_Strings[31].msg)
#define _MSG_STR_PropertiesGameTitleTitle (iGameStrings_Strings+31)
#define MSG_LA_PropertiesGenre (iGameStrings_Strings[32].msg)
#define _MSG_LA_PropertiesGenre (iGameStrings_Strings+32)
#define MSG_CY_PropertiesGenre0 (iGameStrings_Strings[33].msg)
#define _MSG_CY_PropertiesGenre0 (iGameStrings_Strings+33)
#define MSG_CH_PropertiesFavorite (iGameStrings_Strings[34].msg)
#define _MSG_CH_PropertiesFavorite (iGameStrings_Strings+34)
#define MSG_CH_PropertiesHidden (iGameStrings_Strings[35].msg)
#define _MSG_CH_PropertiesHidden (iGameStrings_Strings+35)
#define MSG_LA_PropertiesTimesPlayed (iGameStrings_Strings[36].msg)
#define _MSG_LA_PropertiesTimesPlayed (iGameStrings_Strings+36)
#define MSG_LA_PropertiesSlavePath (iGameStrings_Strings[37].msg)
#define _MSG_LA_PropertiesSlavePath (iGameStrings_Strings+37)
#define MSG_TX_PropertiesTooltypesTitle (iGameStrings_Strings[38].msg)
#define _MSG_TX_PropertiesTooltypesTitle (iGameStrings_Strings+38)
#define MSG_BT_PropertiesOK (iGameStrings_Strings[39].msg)
#define _MSG_BT_PropertiesOK (iGameStrings_Strings+39)
#define MSG_BT_PropertiesCancel (iGameStrings_Strings[40].msg)
#define _MSG_BT_PropertiesCancel (iGameStrings_Strings+40)
#define MSG_WI_GameRepositories (iGameStrings_Strings[41].msg)
#define _MSG_WI_GameRepositories (iGameStrings_Strings+41)
#define MSG_BT_AddRepo (iGameStrings_Strings[42].msg)
#define _MSG_BT_AddRepo (iGameStrings_Strings+42)
#define MSG_BT_RemoveRepo (iGameStrings_Strings[43].msg)
#define _MSG_BT_RemoveRepo (iGameStrings_Strings+43)
#define MSG_BT_CloseRepoWindow (iGameStrings_Strings[44].msg)
#define _MSG_BT_CloseRepoWindow (iGameStrings_Strings+44)
#define MSG_WI_AddNonWHDLoad (iGameStrings_Strings[45].msg)
#define _MSG_WI_AddNonWHDLoad (iGameStrings_Strings+45)
#define MSG_LA_AddGameTitle (iGameStrings_Strings[46].msg)
#define _MSG_LA_AddGameTitle (iGameStrings_Strings+46)
#define MSG_LA_AddGamePath (iGameStrings_Strings[47].msg)
#define _MSG_LA_AddGamePath (iGameStrings_Strings+47)
#define MSG_LA_AddGameGenre (iGameStrings_Strings[48].msg)
#define _MSG_LA_AddGameGenre (iGameStrings_Strings+48)
#define MSG_CY_AddGameGenre0 (iGameStrings_Strings[49].msg)
#define _MSG_CY_AddGameGenre0 (iGameStrings_Strings+49)
#define MSG_BT_AddGameOK (iGameStrings_Strings[50].msg)
#define _MSG_BT_AddGameOK (iGameStrings_Strings+50)
#define MSG_BT_AddGameCancel (iGameStrings_Strings[51].msg)
#define _MSG_BT_AddGameCancel (iGameStrings_Strings+51)
#define MSG_WI_About (iGameStrings_Strings[52].msg)
#define _MSG_WI_About (iGameStrings_Strings+52)
#define MSG_TX_About (iGameStrings_Strings[53].msg)
#define _MSG_TX_About (iGameStrings_Strings+53)
#define MSG_BT_AboutOK (iGameStrings_Strings[54].msg)
#define _MSG_BT_AboutOK (iGameStrings_Strings+54)
#define MSG_WI_Settings (iGameStrings_Strings[55].msg)
#define _MSG_WI_Settings (iGameStrings_Strings+55)
#define MSG_LA_HideScreenshots (iGameStrings_Strings[56].msg)
#define _MSG_LA_HideScreenshots (iGameStrings_Strings+56)
#define MSG_GR_ScreenshotsTitle (iGameStrings_Strings[57].msg)
#define _MSG_GR_ScreenshotsTitle (iGameStrings_Strings+57)
#define MSG_LA_NoGuiGfx (iGameStrings_Strings[58].msg)
#define _MSG_LA_NoGuiGfx (iGameStrings_Strings+58)
#define MSG_LA_ScreenshotSize (iGameStrings_Strings[59].msg)
#define _MSG_LA_ScreenshotSize (iGameStrings_Strings+59)
#define MSG_CY_ScreenshotSize0 (iGameStrings_Strings[60].msg)
#define _MSG_CY_ScreenshotSize0 (iGameStrings_Strings+60)
#define MSG_CY_ScreenshotSize1 (iGameStrings_Strings[61].msg)
#define _MSG_CY_ScreenshotSize1 (iGameStrings_Strings+61)
#define MSG_CY_ScreenshotSize2 (iGameStrings_Strings[62].msg)
#define _MSG_CY_ScreenshotSize2 (iGameStrings_Strings+62)
#define MSG_LA_Width (iGameStrings_Strings[63].msg)
#define _MSG_LA_Width (iGameStrings_Strings+63)
#define MSG_LA_Height (iGameStrings_Strings[64].msg)
#define _MSG_LA_Height (iGameStrings_Strings+64)
#define MSG_GR_TitlesTitle (iGameStrings_Strings[65].msg)
#define _MSG_GR_TitlesTitle (iGameStrings_Strings+65)
#define MSG_RA_TitlesFromTitle (iGameStrings_Strings[66].msg)
#define _MSG_RA_TitlesFromTitle (iGameStrings_Strings+66)
#define MSG_RA_TitlesFrom0 (iGameStrings_Strings[67].msg)
#define _MSG_RA_TitlesFrom0 (iGameStrings_Strings+67)
#define MSG_RA_TitlesFrom1 (iGameStrings_Strings[68].msg)
#define _MSG_RA_TitlesFrom1 (iGameStrings_Strings+68)
#define MSG_LA_SmartSpaces (iGameStrings_Strings[69].msg)
#define _MSG_LA_SmartSpaces (iGameStrings_Strings+69)
#define MSG_GR_MiscTitle (iGameStrings_Strings[70].msg)
#define _MSG_GR_MiscTitle (iGameStrings_Strings+70)
#define MSG_LA_SaveStatsOnExit (iGameStrings_Strings[71].msg)
#define _MSG_LA_SaveStatsOnExit (iGameStrings_Strings+71)
#define MSG_LA_FilterUseEnter (iGameStrings_Strings[72].msg)
#define _MSG_LA_FilterUseEnter (iGameStrings_Strings+72)
#define MSG_LA_HideSidepanel (iGameStrings_Strings[73].msg)
#define _MSG_LA_HideSidepanel (iGameStrings_Strings+73)
#define MSG_BT_SettingsSave (iGameStrings_Strings[74].msg)
#define _MSG_BT_SettingsSave (iGameStrings_Strings+74)
#define MSG_BT_SettingsUse (iGameStrings_Strings[75].msg)
#define _MSG_BT_SettingsUse (iGameStrings_Strings+75)
#define MSG_BT_SettingsCancel (iGameStrings_Strings[76].msg)
#define _MSG_BT_SettingsCancel (iGameStrings_Strings+76)
#define MSG_SelectDir (iGameStrings_Strings[77].msg)
#define _MSG_SelectDir (iGameStrings_Strings+77)
#define MSG_GameExecutable (iGameStrings_Strings[78].msg)
#define _MSG_GameExecutable (iGameStrings_Strings+78)
#define MSG_TotalNumberOfGames (iGameStrings_Strings[79].msg)
#define _MSG_TotalNumberOfGames (iGameStrings_Strings+79)
#define MSG_UnknownGenre (iGameStrings_Strings[80].msg)
#define _MSG_UnknownGenre (iGameStrings_Strings+80)
#define MSG_FilterShowAll (iGameStrings_Strings[81].msg)
#define _MSG_FilterShowAll (iGameStrings_Strings+81)
#define MSG_FilterFavorites (iGameStrings_Strings[82].msg)
#define _MSG_FilterFavorites (iGameStrings_Strings+82)
#define MSG_FilterLastPlayed (iGameStrings_Strings[83].msg)
#define _MSG_FilterLastPlayed (iGameStrings_Strings+83)
#define MSG_FilterMostPlayed (iGameStrings_Strings[84].msg)
#define _MSG_FilterMostPlayed (iGameStrings_Strings+84)
#define MSG_FilterNeverPlayed (iGameStrings_Strings[85].msg)
#define _MSG_FilterNeverPlayed (iGameStrings_Strings+85)
#define MSG_RunningGameTitle (iGameStrings_Strings[86].msg)
#define _MSG_RunningGameTitle (iGameStrings_Strings+86)
#define MSG_ErrorExecutingWhdload (iGameStrings_Strings[87].msg)
#define _MSG_ErrorExecutingWhdload (iGameStrings_Strings+87)
#define MSG_ScanningPleaseWait (iGameStrings_Strings[88].msg)
#define _MSG_ScanningPleaseWait (iGameStrings_Strings+88)
#define MSG_CouldNotCreateReposFile (iGameStrings_Strings[89].msg)
#define _MSG_CouldNotCreateReposFile (iGameStrings_Strings+89)
#define MSG_SelectGameFromList (iGameStrings_Strings[90].msg)
#define _MSG_SelectGameFromList (iGameStrings_Strings+90)
#define MSG_TitleAlreadyExists (iGameStrings_Strings[91].msg)
#define _MSG_TitleAlreadyExists (iGameStrings_Strings+91)
#define MSG_SavingGamelist (iGameStrings_Strings[92].msg)
#define _MSG_SavingGamelist (iGameStrings_Strings+92)
#define MSG_FailedOpeningGameslist (iGameStrings_Strings[93].msg)
#define _MSG_FailedOpeningGameslist (iGameStrings_Strings+93)
#define MSG_BadTooltype (iGameStrings_Strings[94].msg)
#define _MSG_BadTooltype (iGameStrings_Strings+94)
#define MSG_NoTitleSpecified (iGameStrings_Strings[95].msg)
#define _MSG_NoTitleSpecified (iGameStrings_Strings+95)
#define MSG_NoExecutableSpecified (iGameStrings_Strings[96].msg)
#define _MSG_NoExecutableSpecified (iGameStrings_Strings+96)
#define MSG_NotEnoughMemory (iGameStrings_Strings[97].msg)
#define _MSG_NotEnoughMemory (iGameStrings_Strings+97)
#define MSG_DirectoryNotFound (iGameStrings_Strings[98].msg)
#define _MSG_DirectoryNotFound (iGameStrings_Strings+98)
#define MSG_LA_StartWithFavorites (iGameStrings_Strings[99].msg)
#define _MSG_LA_StartWithFavorites (iGameStrings_Strings+99)
#define MSG_MNMainOpenCurrentDir (iGameStrings_Strings[100].msg)
#define _MSG_MNMainOpenCurrentDir (iGameStrings_Strings+100)
#define MSG_MNMainOpenCurrentDirChar (iGameStrings_Strings[101].msg)
#define _MSG_MNMainOpenCurrentDirChar (iGameStrings_Strings+101)

#endif
