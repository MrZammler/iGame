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

There is a pre-build vbcc enviroment that runs on x64 linux here: http://winterland.no-ip.org/2014/07/vbcc-cross-compiler/

iGame requires the include files of the libraries used to be present when compiling it. The Makefile has these hardcoded paths in
/opt/vbcc/includes_and_libs/ directories. You should provide the following includes for building:

* MUI includes
* MCC_Guigfx includes
* MCC_TextEditor includes
* NDK 3.9 includes

The pre-build vbcc enviroment already contains them.

After you have installed the above, it's then a matter of running *make* or *make iGame* on the iGame folder.

You could also build a processor specific verion by running *make iGame.0X0*

*make clean* will remove any object and binaries from the directories.

*make release* will build all the cpu specific binaries, and pack a nice lha archive inside the iGame_rel directory.

## Contributing

Please do :-)

## Usage

Docs for the usage of iGame can be found inside the guide directory. The guide will be packed inside iGame releases and
can also be brought up by pressing the Help key when running iGame.