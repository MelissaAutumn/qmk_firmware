# Mel's Fork

This is a fork of the QMK firmware that "fixes" the oled init issues I was experiencing, adds bongocat (like the stock firmware), and some other tweaks.

## Changes

* i2c timings have been updated to fast mode from the default timings. 
* `OLED_I2C_TIMEOUT` has been pushed up from the default `100` to `500` to work around oled_init fail issues.
* oled_on / oled_off time out repeatedly with a `OLED_I2C_TIMEOUT` greater than `100` so they've been replaced with clearing the screen.
* [Bongocat](https://github.com/pedker/OLED-BongoCat-Revision/) has been added in like the stock firmware.
* OLED refresh time has been upped to 30fps from 15fps. (I don't know how many refreshes a second this display can handle, but 30 seems to work.)
* Stock layout has been altered slightly (see [oopsappnaps/keymap.c](keyboards/cannonkeys/satisfaction75_hs/keymaps/oopsallnaps/keymap.c))
* OLED hid mode which updates the display with cpu, gpu, and ram from a python script running on your computer.
* Optionally with OLED hid mod enabled you can have the python script set the keyboard's date and time.

## Building

You can build this firmware for the Sat75x (not original Sat75 boards) with:

```
make cannonkeys/satisfaction75_hs:oopsallnaps
```

You can also build and then flash by running:

```
make cannonkeys/satisfaction75_hs:oopsallnaps:flash
```

# Quantum Mechanical Keyboard Firmware

[![Current Version](https://img.shields.io/github/tag/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/tags)
[![Discord](https://img.shields.io/discord/440868230475677696.svg)](https://discord.gg/qmk)
[![Docs Status](https://img.shields.io/badge/docs-ready-orange.svg)](https://docs.qmk.fm)
[![GitHub contributors](https://img.shields.io/github/contributors/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/pulse/monthly)
[![GitHub forks](https://img.shields.io/github/forks/qmk/qmk_firmware.svg?style=social&label=Fork)](https://github.com/qmk/qmk_firmware/)

This is a keyboard firmware based on the [tmk\_keyboard firmware](https://github.com/tmk/tmk_keyboard) with some useful features for Atmel AVR and ARM controllers, and more specifically, the [OLKB product line](https://olkb.com), the [ErgoDox EZ](https://ergodox-ez.com) keyboard, and the Clueboard product line.

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

The docs are powered by [VitePress](https://vitepress.dev/). They are also viewable offline; see [Previewing the Documentation](https://docs.qmk.fm/#/contributing?id=previewing-the-documentation) for more details.

You can request changes by making a fork and opening a [pull request](https://github.com/qmk/qmk_firmware/pulls).

## Supported Keyboards

* [Planck](/keyboards/planck/)
* [Preonic](/keyboards/preonic/)
* [ErgoDox EZ](/keyboards/ergodox_ez/)
* [Clueboard](/keyboards/clueboard/)
* [Cluepad](/keyboards/clueboard/17/)
* [Atreus](/keyboards/atreus/)

The project also includes community support for [lots of other keyboards](/keyboards/).

## Maintainers

QMK is developed and maintained by Jack Humbert of OLKB with contributions from the community, and of course, [Hasu](https://github.com/tmk). The OLKB product firmwares are maintained by [Jack Humbert](https://github.com/jackhumbert), the Ergodox EZ by [ZSA Technology Labs](https://github.com/zsa), the Clueboard by [Zach White](https://github.com/skullydazed), and the Atreus by [Phil Hagelberg](https://github.com/technomancy).

## Official Website

[qmk.fm](https://qmk.fm) is the official website of QMK, where you can find links to this page, the documentation, and the keyboards supported by QMK.
