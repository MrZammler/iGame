## Description

iGame is a frontend to launching WHDLoad games for the Amiga. It is implemented as a MUI application.

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

Please check the [Wiki](https://github.com/MrZammler/iGame/wiki/Compiling-iGame)

## Contributing

Please do :-)

## Usage

Docs for the usage of iGame can be found inside the guide directory. The guide will be packed inside iGame releases and
can also be brought up by pressing the Help key when running iGame.
