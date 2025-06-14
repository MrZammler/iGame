## iGame VERSION_TAG - [RELEASE_DATE]
### Added
- By changing the Genre select box to a PopupString, it is now possible to edit the Genre title and create new ones, which will be added to the Genre lists
- Added Portuguese language

### Changed
- The genres in the Information window changed to a PopupString. Now, it takes less space to choose a genre, making it possible even on small resolutions.
- The genres lists have the titles sorted alphabetically
- Removed some not necessary fill ups of genre cycle boxes and loops. This should make iGame start a little bit faster
- Removed some extra change notifications in Genres lists, which should avoid not necessary refreshes of the games/demos lists
- Added a requester to show the errors occure when iGame is started and a library is missing
- Now the iGame icon is set back to start by Workbench
- Revamped the About window, adding a new logo and links
- Changed the main image in the repository

### Fixed
- The genres lists were not populated when the right sidebar was disabled (#260)

## iGame 2.5.2 - [2025-05-31]
### Fixed
- Fixed the execution of whdload that broke on v2.5.1 for those that use an old icon library

## iGame 2.5.1 - [2025-05-29]
### Added
- Added a file requester in the Properties window, which can be used to set a different WHDLoad slave file for an item. This is useful when a game/demo changed place on the hard disk. (#174)
- Based on the selected file by the new field, the tooltypes text is updated, enabled/disabled, based if the selected file is a WHDLoad slave one.

### Changed
- Moved the Properties window code to its own files
- Added the "Open game folder" menu in the MorphOS version, that was missing
- Disabled the gamepad usage on MorphOS because it was reported giving problems while playing a game

### Fixed
- Fixed starting WHDLoad games in MorphOS using WHDLoadopener (#253)

## iGame 2.5.0 - [2025-05-12]
### Added
- Added a new "Information" window that includes the "Released date", the "Released by", the "Chipset", the links to external websites and most of the fields from the properties window.
- Added the option to use repositories based on assigns (fixes #240)

### Changed
- The "Properties" window has only the tooltypes of the selected item available to change.

### Fixed
- Fixed a potential crash on exit, happening mostly on AmigaOS 3.1 systems (fixes #239)

## iGame 2.4.6 - [2024-02-18]
### Changed
- Some optimisation in list loading reducing the time needed more than 42%

## iGame 2.4.5 - [2023-11-03]
### Changed
- Speedup the slavesListAddTail(), almost 200% faster. This has an impact on the slaves list creation during the scan and the loading from the file.

### Fixed
- Fixed the opening of the properties window for some users, by reverting some changes from v2.4.1
- Fixed the item (demo/game) renaming from the properties window
- Fixed the addition of an item (demo/game) from the "Add game..." window. This was saved in a wrong way and was breaking the list
- Fixed the title change of the items in the list (#215)
- Code cleanup

## iGame 2.4.4 - [2023-09-11]
### Changed
- Now after the repository scans, any item that is not assigned to any genre gets the "Unknown" value by default

### Fixed
- Fixed the generated genre list titles after a rescan of the repository
- Fixed more memory leaks
- Fixed the load of the list of items when the side panel is hidden

## iGame 2.4.3 - [2023-09-01]
### Fixed
- Fixed memory leaks
- Fixed the lists getting wrong values when overflow

## iGame 2.4.2 - [2023-08-27]
### Changed
- Simplified the version string and added the release date in the screen title

### Fixed
- Fixed the menus in MorphOS that were broken for some time now

## iGame 2.4.1 - [2023-07-19]
### Added
- Now the Genre list is populated from the igame.data files and the genre file, if it exists, although it is not necessary. The genre filtering is working with these new values
- Added a new cycle box that lets the user filter the results based on the chipset. This requires in settings the "Prefer igame.data files" to be enabled

### Changed
- When real-time filtering is enabled at least 3 characters are required so as to be initiated. Less than 3 characters are ignored, unless the filtering by pressing the enter button is enabled.
- Removed the filtering options from the Genre list and moved them to their own select box above the entries list
- Now, even if the list entries list is populated a new repository scan will update the Chipset and Genre information based on the data found in igame.data files.
- If the "Prefer igame.data files" is not enabled those files are not used during the scan of repositories

### Fixed
- Fixed a hit when the entry properties are requested without having a selected entry

## iGame 2.4.0 - [2023-05-13]
### Added
- Added back the saving of the list after a scan of the repositories, which was removed in the previous releases
- Introduced the new igame.data files
- iGame uses NList now with extra columns that are sortable
- Added "Prefer igame.data" checkbox in preferences that let the user to set if the title is going to retrieved from the igame.data files or from the slave file or the folder name, like it used to work. This doesn't have any effect on non-WHDLoad items.

### Fixed
- Fixed the status messages which was stack with saving list message and didn't show the number of games again.

### Changed
- Now the MUI changes are loaded from envarc. With MUI v5 balance size and list columns are saved there and restored the next time iGame is started

## iGame 2.3.2 - [2023-04-20]
### Fixed
- Fixed a bug on repo removal, which remained in the repos list and was included in scans, unless the user restarted iGame

 ## iGame 2.3.2 - [2023-04-20]
### Fixed
- The custom screenshot sizes were wrongly saved. This is now fixed (#190)
- Fixed a duplication in slave names after the execution of a second scan in the same list, introduced in v2.3.0
- Fixed a crash on systems that use AutoUpdateWB patch. **HUGE THANKS to mfilos** for his testing, feedback and support up to late at nights
- Fixed an error on status text after launching a game that showed total of zero games, introduced in v2.3.0
- Refactored the Slave tooltype parsing on launching to fix a memory hit and potential crashes

### Changed
- Added some spaces left and right of the screenshot. This helps to resize the sidebar as needed keeping the screenshot centred
- Now, only the screenshot part of the GUI is updated on item selection and not the whole right side. This makes the GUI refresh faster on slow computers
- If the screenshot is toggled (show/hide) in the settings window the GUI is updated without the need of restarting iGame
- Now iGame doesn't fail to start even if any of the guigfx.library, render.library or Guigfx.mcc is missing. It falls back using the datatypes and the "No GuiGfx" checkbox is blocked in the settings
- With v2.3.0 WHDLoad games that have different names on .info file against the slave file was not starting. I changed the way the necessary info file is discovered to be closer to what v2.2.1 was doing and now it works fine

## iGame 2.3.1 - [2023-04-14]
### Fixed
- This is a hot fix release for the installer script that had a bug of not finding the icons folder

## iGame 2.3.0 - [2023-04-13]
### Added
- Some nonWHDLoad games require to be started from their own folder, otherwise they crash or fail to start. This is a problem mainly on systems that do not have WBRun or WBLoad (i.e. AmigaOS 3.1). Now, for all the added games/demos that are started on such systems a temporary script is created under T:
- Added German catalog and an alternative main image contributed by Martin Cornelius
- Added French catalog contributed by AmiGuy

### Changed
- Now we compile iGame with NDK 3.2R4
- Now the repositories path requester accepts only drawers
- Memory footprint is now reduced and optimised as much as possible
- iGame window appears first and then the list is loaded from the file. This makes the application appear faster, giving feedback to the user
- When the user selects a relative path (ie. //Games), that changes to an absolute path.
- Changed the way the same title slaves are counted. This will help for removing the multiple "Alt" words in the list
- Changed the repository scanning for files. The code is simplified and now uses more functions from the system API, making it more compatible. Also, all the "Data" folders are skipped from scanning, so slaves that exist in those folders are not used. A lot of code was removed and optimised, so the scan is now around 37% faster on the same machine, scanning a folder with 3562 entries.
- Changed the way the multiple instances with the same title are shown. Now no "Alt" labels will be added. Instead, we introduced a number in square brackets that represent the different instances, i.e. 3DPool [1], 4DSportsDriving [2]. Not every record has these values, so expect to see them only when there are duplicates.
- The gamelist.csv file changed by having double quotes around the string values, like the path and the title.
- A new column was added in the gamelist.csv file which holds the custom name of the item. The old column remains unchanged. This helps in situations like a repository rescan, where the given name by the user doesn't change. The old second column should remain unchanged.
- Now iGame requires icon.library v44+ for changing icon tooltypes. For systems that do include a new version, there is icon.library 46.4 on Aminet is free to download and use. Those that have older versions of the library will still be able to use iGame, but they won't be able to change the icon tooltypes. They will need to do it from Workbench
- Now the filter string field is not disabled in the hidden list, allowing the user to filter the results based on the title or part of it
- "Status" line, "Times played" and "Slave path" do not have a border anymore, since they are read-only fields
- "No smart spaces" checkbox has a proper border now
- "Slave path" in the item properties window doesn't stretch its width anymore. If MUI 5 is used the text will be shortened, showing three dots in the middle of it.
- If WBLoad exists in the C: folder, this is going to be used to start games/demos from WB, when they were added as extra items in the list
- The "Last played" list holds more than one record, so that you can easily find the last games you played
- Moved alt_icons under extras folder
- Updated installer script to give some info to the user at the installation folder selection step

### Fixed
- When a user changes the tooltypes of a slave, the NewIcon keeps working. The previous versions of iGame were deleting the image information. Now they are dropped from the icon tooltypes, but the image keeps working fine. As a result, the .info file size is reduced almost to half.
- Now it is possible to save the icon tooltypes even if it is used as an item screenshot. In the previous versions the icon file was blocked and no change was possible
- Fixed crashing on exit
- Fixed low memory computer freeze while scanning
- Fixed low memory computer freeze when loading big list of entries

## iGame 2.2.1 - [2023-01-30]
### Added
- Added Turkish catalog contributed by Serkan Dursun

### Fixed
- Now the 68K cpu specific versions are included in the archive. In v2.2.0 all versions where the same 68000 binary because of a missing flag in compilation
- Willem Drijver contributed with a fix on Execute tooltype, which was not working well.

### Changed
- Removed the tooltypes from the iGame icons. They are not used any more

## iGame 2.2.0 - [2022-11-06]
### Added
- Added automatic release to Aminet and OS4Depot through the CI/CD whenever a new release tag is created at the repo

### Changed
- Removed completely the Tooltypes. Now iGame defaults to the optimal settings and uses only the settings file to change its behaviour.
- Now if the file "envarc:igame.prefs" exists then this is used by iGame, instead of the one that exists at the PROGDIR:. This way multiple iGame instances will have common settings (#173)
- Introducing semantic versioning. No beta versions any more. Now the versions will have three digits based on MAJOR.MINOR.PATCH pattern. More info at https://semver.org/

## iGame 2.1 - [2022-06-04]
### Changed
- Did some fixes on libraries checks during application start based on the selected tooltypes
- Some minor fixes
- Update icons' positions to not overlap each other
- Added NOSIDEPANEL to iGame tooltypes that was missing

... plus all the changes that were released in previous beta versions (2.1b1 up to 2.1b3), which you can see below

## iGame 2.1b3 - [2021-12-04]
### Added
- Added a check if the screenshot image is supported by the installed datatypes. If not, it is skipped. This fixes situations where the Info datatype is not installed and no image is shown instead of the default.

### Fixed
- Fixed the menus on Aros 68k. Menus should work on ApolloOS now.

### Changed
- The games list is not multiselect now. This speeds up a little bit the selection of games on slow machines.
- Moved the strings methods to a separate file. Also merged the strcasestr.c and strdup.c files.
- Moved the filesystem methods to a separate file.
- Set local methods as static in the funcs.c and cleaned up the iGameExtern.h from the shared methods
- Now all the libraries open on application start and close on application exit. No OpenLibrary() calls in the middle of the application.
- Moved the joystick methods from iGameMain.c to funcs.c
- A lot of global methods and variables removed
- A lot of refactoring happened, so to make funcs.c file smaller. This makes code more clear and readable.
- All the necessary libraries and interfaces are set to be loaded in the code. No "-lauto" is necessary any more.
- Changed the localization system to support the new menus. Now the strings header file is created based on catcomp

## iGame 2.1b2 - [2021-03-15]
### Added
- Added iGame version at the screen title

### Updated
- Updated Italian catalog
- Updated Greek catalog
### Fixed
- Fixed starting whdload games that have tooltypes start with the characters »«.=#!
- Fixed starting whdload games/demos that the Slave path is missing. Now an error message is shown, and iGame doesn't crash hard, bringing down the whole system.


## iGame 2.1b1 - [2021-03-09]
### Added
- Three extra PNG icons contributed by Carlo Spadoni
- Added Greek translation

### Fixed
- Fixed install script on MorphOS
- Fixed usage of Catalog files (thanks coldacid)


## iGame 2.0 - 2020-10-16
This release includes all the fixes and changes of the 2.0 beta releases below.

## iGame 2.0b8 - 2020-10-08
### Added
- Added Italian catalog file in release package and it is now selectable in install script

### Fixed
- Fixed the copy of the OS4 native version on installer
- Fixed the running of games that include spaces in path or file names and added by "Add a game" menu item
- Fixed the zero records in gameslist file when the user changed game properties from the respective window

## iGame 2.0b7 - 2020-09-25
### Added
- Changelog file

### Changed
- Fully updated iGame.guide file
- Changed the menu item name "Add non-WHDLoad game..." to "Add game.." to be more precise, since from that you can add WHDLoad games as well

### Fixed
- Fixed a problem with saving of the CSV file when the old gameslist file had empty genres
- Fixed "Show Favorites" on startup which was not working under AmigaOS 3 with MUI 5
- Fixed the release version and the date on about window
- Fixed the installer that was complaining about missing locale/catalogs folder

## iGame 2.0b6 - 2020-09-04
### Added
- Now iGame checks for any missing library and informs the user if it is started from the shell
- New installation script
- Show "Unknown" genre in genres list for those games that are not categorized yet
- When iGame is iconified, now uses it's icon
- Open game directory from menu
- AmigaOS 4 native version added

### Changed
- Changed the gameslist file to CSV, which makes iGame to start faster and the file to be a lot smaller in size. If there is already a gameslist, iGame will read it and on next write it will convert it to csv. All genres and statistics will remain. The old plain list gamelist will not be used after this point, but it is not going to be deleted automatically
- Keep image aspect for screenshot/icon when GuiGfx is used

### Fixed
- Stability/Bug fixes, GUI fixes
- Fixed menu shortcuts that didn't work


## iGame 2.0b5 - 2019-05-03
### Added
- Option to start with the favorites list
- If C:WBRun exists, run non-whdload games with it

### Changed
- Some misc gui changes


## iGame 2.0b4 (iGame-2019-01-15.lha)
TBD


## iGame 2.0b3 (iGame-2018-11-01.lha)
### Added
- Added Makefile for Linux


## iGame 2.0b2 (iGame-2018-10-25.lha)
TBD


## iGame 2.0b1 (iGame-2018-10-11.lha)
### Added
- Added Makefile for Windows
- Added title scanning based on the parent folder
- Added Settings window. Now using a prefs file, instead of only Tooltypes. Falls back to tooltypes if no prefs file is found.
- Fully translatable using standard .catalog files (.cd file included for translators)

### Changed
- GUI rewritten from scratch keeping the same look and feel
- A lot of code cleanup
- Improved error handling in various places


## iGame (iGame-2016-11-30.lha)
TBD


## iGame (iGame-2015-11-07.lha)
### Fixed
- Fix a GR when selecting Game Properties without having selected a game. (Thanks @AllanU74)


## iGame (iGame-2015-02-13.lha)
### Changed
- Adjusts screenshot box according to workbench resolution.


## iGame (iGame-2014-08-13.lha)
### Added
- Builds for 030 and 060
- Added SAVESTATSONEXIT tooltype
- Under some circumstances, iGame now loads games icons

### Changed
- Now compiled with vbcc. Should produce a slightly faster build.
- Faster initial loading

### Fixed
- Fixed OS4 crashes (mainly during search).
- Some other small changes done during a few years of on/off coding.


## iGame 1.5 (12/08/08)
### Added
- Added "--Never Played--"
- Filter will now work together with genres
- Added FILTERUSEENTER
- Added indication when writing to disk

### Changed
- Genres are now in an external file
- Reworked "Show/Hide hidden slaves"


## iGame 1.4 (08/12/07)
### Added
- Editable tooltypes

### Changed
- Speed increase while listing games

### Fixed
- Misc bug fixes


## iGame 1.3 (28/04/07)
### Added
- Added the NOGUIGFX tooltype
- Added keyboard shortcut to Game Properties (Right A + P)
- Added option to hide a slave
- iGame will now launch a slave, even without an info file (with default tooltypes)
- Reads titles from slave files
- Added the update_titles app
- 'Alt' suffix is used for multiple slaves


## iGame 1.2 (06/04/2007)
### Changed
- Search .info files only when launching games

### Fixed
- Fixed screenshot size to be consistent when changing images
- Counters now work for genres filtering


## iGame 1.1 (27/03/2007)
### Added
- Added MUI Settings

### Fixed
- Fixed a nasty bug that caused iGame to sometimes list wrong titles (or no titles at all) after a scan


## iGame 1.0 (17/03/2007)
### Added
- First public release

### Changed
- Too many things changed to list here


## iGame 0.2 (30/07/2005)
### Added
- iGame finds the tooltypes from the game's icon and uses them


## iGame 0.1-23052005 (23/05/2005)
### Added
- Games that have their data stored in a sub-dir, now run


## iGame 0.1-05052005 (05/05/2005)
### Added
- First test version


The format of this changelog file is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
