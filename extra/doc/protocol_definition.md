Protocole definition
====================

Hardware and data flow
----------------------

Poppy communication protocol use 3 data wire :

 - SCL
 - SDA
 - PTP

Wire SCL and SDA are used to manage high-speed **I2C** (3.6Mhz). This bus is the **main communication way**. The **PTP** wire means "point to point", he manage hardware detection by passing tokens and allow to comminucate directly with the next or previous module with high speed serial communication.

All messages I2C or PTP have the same structure :

 - Address
 - msg_type
 - msg_size*
 - msg[0]*
 - ...*
 - msg[msg_size-1]*
 - CRC*

\* this part of the message can be missing on somes network level messages, but you will probably don't have to seal with it!

Protocol levels
---------------

The Poppy communication stack have different levels of messages :

 - **user messages** (accessible by the end user)
 - **module messages** (specific to a module type)
 - **network messages** (network management messages)

Each of these level have his hown **msg_type field**. **Network messages** are prioritary on **module messages** and **module messages** are prioritaty on **user messages**.

If you need to use some modules and write your hown code for your own robot you only have to deal with **user messages**.
If you are a module creator you will need to deal with module level messages.
You probably don't have to deal with **network messages**, but if you have any question please describe it in the [Poppy project forum](https://forum.poppy-project.org).

Messages definition
-------------------

If you write a code on a module you will need to create your own msg_type. There is the same methode to define it if you are at **user messages** level or **module messages**

You simply have to ceate your msg_type list

```c
/**
 * \enum msg_type_t
 * \brief Module type enumerator.
 *
 * This structure is used to list all your message type.
 */
typedef enum {
    REGISTER_1,
    REGISTER_2
}msg_type_t;
```
