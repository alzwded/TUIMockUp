TUIMockUp
=========

tool to create text-user-interface mockups which look nice with a monospaced font

Example usage
=============

You can run it, for example, with
```sh
tuimu 70 "My Window" <<END
Label 1
Tokens:

LN
ListBox 1
convert
f in.bmp
-scale
0.5x0.5
.
.

LN
TextBox 0.8
~/test out.bmp

Button
Add

Button
Browse

LN
TextBox 0.8
/tmp/do_stuff.csh

Combo 0.2
csh

Button
Save

LN
Label 0.15
CmdLine:

TextBox 0.85
convert "f in.bmp" -scale 0.5x0.5

Button
Copy



END
```
will output
```no-highlight
 _________________________________________________________________ 
|X| My Window                                                   |+|
|-----------------------------------------------------------------|
| Tokens:                                                         |
|  _____________________________________________________________  |
| | convert.................................................. |^| |
| | f in.bmp................................................. |_| |
| | -scale................................................... |_| |
| | 0.5x0.5.................................................. | | |
| | ......................................................... | | |
| |_._________________________________________________________|v| |
|  __________________________________________   _____   ________  |
| |_~/test out.bmp___________________________| | Add | | Browse | |
|  ________________________________________   _________   ______  |
| |_/tmp/do_stuff.csh______________________| | csh   |v| | Save | |
|           ___________________________________________   ______  |
| CmdLine: |_convert "f in.bmp" -scale 0.5x0.5_________| | Copy | |
|_________________________________________________________________|
```

TODO
====

* vertical group panel
* ~~stdin parser~~
* handle negative-length string exceptions && assuring a minimum window width in case there's not enough horizontal room for components
* table input control

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

Tokens
------

| token                 | output                               |
|-----------------------|--------------------------------------|
| Button                | a button                             |
| Label                 | a single or multilined label         |
| TextBox               | a single or multi-line text box      |
| ListBox               | a multi line list-box                |
| Combo                 | a combo dropdown menu thing          |
| ~~VGroup~~            | a vertical grouping of stuff         |
