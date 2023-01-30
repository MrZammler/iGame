## iGame VERSION_TAG - [RELEASE_DATE]
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
