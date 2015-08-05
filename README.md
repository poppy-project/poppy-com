Poppy 2.0 communication lib  {#mainpage}
===========================

[![Build Status](https://travis-ci.org/poppy-project/poppy-com.svg?branch=master)](https://travis-ci.org/poppy-project/poppy-com)[![Coverage Status](https://coveralls.io/repos/poppy-project/poppy-com/badge.svg?branch=master&service=github)](https://coveralls.io/github/poppy-project/poppy-com?branch=master)
Please read [the doc](http://poppy-project.github.io/poppy-com/)

The role of this code is to manage the Poppy 2.0 communication stack between all modules.

You can use it to develop you own Poppy 2.0 compatible module.

To understand how this protocole work, please read the [protocol definition](doc/protocol_definition.md).

How to start a new Poppy 2.0 module code project
------------------------------------------------

Lib use
_______

Just do a fork of this repository to create your hown module. To avoid any trouble during future updates, take care to not modifying any files in the poppy-pross folder or create a specific branch for all your developpements.

In this repo travis is used to create documentation, update readme, and pass some tests.
To acivate all this things you have to follow some steps :
 - enable your repo on [travis](https://travis-ci.org/) and on [coveralls](https://coveralls.io).
 - [Create an encrypted token for travis.](doc/travis_encrypt.md)
 - replace your encrypted token to the .travis.yml

Lib modification
________________

To have a perfect integration and declaration of your module into a poppy network you have to:
 - specify your module_type_t
 - specify your target (Microcontroler)
 - specify your Mainclock frequency
 - specify your max sclfreq
 - create your specific hal folder if you need. To do that please read the [new hal creation documentation](doc/hal_creation.md)
