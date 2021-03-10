## iGame 2.1b1 - [future]
### Added
- Three extra PNG icons contributed by Carlo Spadoni
- Added Greek translation

### Fixed
- Fixed install script on MorphOS
- Fixed usage of Catalog files (thanks coldacid)
- Fixed starting whdload games that have tooltypes start with the characters »«.=#!

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
