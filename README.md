Description
===========
Your Mouse is broken?

Click fires sometimes as Doubleclick?

No Problem, use this to prevent unusual Dobuleclicks.


How to use?
-----------
Simply start

`NoDobuleClick.exe`
It will watch left and right clicks with an timeout of 0.1ms.

You can optionally set your own timeout:

`NoDobuleClick.exe -l 0.05`

will only hook left click with timeout of 0.05 ms.

`NoDobuleClick.exe -r 0.05`

will only hook right click with timeout of 0.05 ms.

`NoDobuleClick.exe -l 0.05 -r 0.07`

will hook left and right click with timeout of 0.05 and 0.07 ms.

Default timeout is 0.1 for both.


How it works
------------
It hooks the mouse clicks and detects to fast clicks in the specifed interval