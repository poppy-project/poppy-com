Protocole definition
====================

The Poppy communication stack have two levels of messages.


The module level messages
-------------------------

You only have to deal with this kind of message in your module application.
These messages are dedicated to your module application, and you have to define it.

The single limitation is to match with the msg_t struct.

All messages send or received should have a register and a
size.
A typical message on the bus will be :
 - Slave addr
 - Register
 - Size
 - Data[0]
 - ...
 - Data[size - 1]

If you have a message with only a register needed you have to set size to 0.
The message transmitted will be :
- Slave addr
- Register
- 0

Your registers definition for your application should be an enum who start at "last_reg_val" value.

The poppy level messages
------------------------

This level is directly integrated into the library, he manage all messages needed to your module declaration and synchronization with others modules on a Poppy robot.
