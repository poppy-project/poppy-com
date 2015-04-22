Poppy 2.0 communication lib                         {#mainpage}
===========================

The role of this code is to manage the Poppy 2.0 communication stack between all modules.

You can use it to develop you own Poppy 2.0 compatible module.

To understand how this protocole work, please read the [protocol definition](doc/protocol_definition.md).

How to start a new Poppy 2.0 module code project
------------------------------------------------

Lib installation
________________

Just do a fork and add a remote repository pointing on the poppy-com repository.
    TODO :
    http://git-scm.com/book/en/v2/Git-Basics-Working-with-Remotes

    ex :
    git remote add read-only alt-machine:/path/to/repo
    tu n'as plus qu'a faire un git pull read-only master pour merger
    read-only étant un "alias"

Lib modification
________________

To have a perfect integration and declaration of your module into a poppy network you have to:
 - specify your module_type_t
 - specify your target (Microcontroler)
 - specify your Mainclock frequency
 - specify your max sclfreq
 - create your specific hal folder if you need. To do that please read the [new hal creation documentation](doc/hal_creation.md)
