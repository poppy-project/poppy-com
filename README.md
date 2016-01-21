[![Build Status](https://travis-ci.org/poppy-project/poppy_com.svg?branch=bootloader)](https://travis-ci.org/poppy-project/poppy_com)[![Coverage Status](https://coveralls.io/repos/poppy-project/poppy_com/badge.svg?branch=bootloader)](https://coveralls.io/github/poppy-project/poppy_com?branch=bootloader)
Please read [the code documentation](http://poppy-project.github.io/poppy_com/)

<span \mainpage<span></span>

Poppy 2.0 communication lib
===========================

The role of this code is to manage the Poppy 2.0 communication stack between all modules.

You can use it to develop you own Poppy 2.0 module code.

To understand how this protocole work, please read the [protocol definition](doc/protocol_definition.md).

How to start a new Poppy 2.0 module code project
------------------------------------------------

First of all you have to fork this repository to create your hown module on your github account.
To avoid any trouble during future updates of the poppy-pross lib, take care to not modifying any files in the poppy-com folder or create a specific branch for all your developpements.
__________________________________________________________________________

In this repo travis is used to create documentation, update readme, and pass some tests.
To activate all this things you have to follow some steps :
 - enable your repo on [travis](https://travis-ci.org/) and on [coveralls](https://coveralls.io).
 - [Create an encrypted token for travis.](doc/travis_encrypt.md)
 - replace your encrypted token to the .travis.yml file of this repo.

__________________________________________________________________________

To have a perfect integration and declaration of your module into a poppy 2.0 network you have to:
 - specify your module in the mod_list.h list.
 - specify your MCU on the Makefile.
 - specify your MAINCLOCK frequency on the Makefile.
 - specify your SCLFREQ to specify the I2C max speed on the Makefile.
 - create your specific hal folder if you need. To do that please read the [new hal creation documentation](doc/hal_creation.md).

You can use the [template.c](template.c) file to format your main file and use it as base.

__________________________________________________________________________

Please feel free to use and contribute to this project.
-------------------------------------------------------

If you have any question please describe it in the [Poppy project forum](https://forum.poppy-project.org).
