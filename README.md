TUIMockUp
=========

tool to create text-user-interface mockups which look nice with a monospaced font

TODO
====

* vertical group panel
* stdin parser

STDIN Parser
============

You will be able to write-up the mock-up in plain text on tuimu's stdin.

It will be something like this:
```
Label 0.8                           # Type weight
hello!                              # text lines, finished with an empty line

Button
Okay

LN
Label 0.6

Button
B1

Button
B2

LN
Combo 0.5
Type

ListBox 0.5
Item1
Item2
Item3

```
