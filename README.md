## Description

iGame is a frontend to launching whdload games for the Amiga. It is implemented as a MUI application.

http://winterland.no-ip.org/igame/

![Alt text](/igame_screen.png?raw=true "iGame screenshot")

## Builds and releases

The iGame_rel folder should contain builds that are done at some point in time, following any major code changes.

## Installing

Just place the iGame folder anywhere you want on your drive. iGame also requires the following libraries to work:

* guigfx.mcc
* TextEditor.mcc
* render.library
* guigfx.library

Make sure your installation contains MUI and the above libraries. You can find the latest versions in Aminet.

If you're having problems with iGame starting (guru 8000000B) try these versions of render and guigfx instead (Thanks to Jools 'buzz' Smyth for providing them): [guigfx_render_nofpu.lha](guigfx_render_nofpu.lha)

If you still get gurus, try disabling the Screenshot pane with adding NOSCREENSHOT in iGames info.

## Compiling

The early versions of iGame were developed and compiled on a real Amiga using gcc at first and then vbcc.
While you can still compile it on an Amiga, it is much faster to do so on a PC. (Side note: I don't really recall the time it took
to compile on a 060, but it must have been close to 1 or 2 minutes. After a while I tried on an 030, and it took nearly 10!). Of course
on a modern PC it takes less than a few seconds...)

These instruction assume the latter approach.

There is a pre-build vbcc enviroment that runs on x64 linux here: http://winterland.no-ip.org/2014/07/12/vbcc-cross-compiler/
For Windows users, there is an installer that includes the latest version of VBCC and the NDK3.9, and takes care of the environment variables for you: http://eab.abime.net/showthread.php?t=83113&highlight=VBCC

iGame requires the include files of the libraries used to be present when compiling it. The Linux Makefile has these hardcoded paths in
/opt/vbcc/includes_and_libs/ directories. The Windows Makefile assumes a VBCC installation on "D:\vbcc" with the includes under that directory (e.g. "D:\vbcc\MUI")

You should provide the following includes for building:

* MUI includes
* MCC_Guigfx includes
* MCC_TextEditor includes
* NDK 3.9 includes

The pre-build vbcc enviroment for Linux already contains them. The VBCC installer for Windows contains the NDK3.9, but the rest need to be copied in D:\VBCC (or whichever installation path you chose).

After you have installed the above, it's then a matter of running *make* or *make iGame* on the iGame folder.
For Windows users, you should run *make -f Makefile.Windows.mak* instead.

You could also build a processor specific verion by running *make iGame.0X0* or *make -f Makefile.Windows.mak iGame.0X0* for Windows.

*make clean* (or or *make -f Makefile.Windows.mak clean* for Windows) will remove any object and binaries from the directories.

*make release* (or or *make -f Makefile.Windows.mak clean* for Windows) will build all the cpu specific binaries, and pack a nice lha archive inside the iGame_rel directory.

## Contributing

Please do :-)

## Usage

Docs for the usage of iGame can be found inside the guide directory. The guide will be packed inside iGame releases and
can also be brought up by pressing the Help key when running iGame.