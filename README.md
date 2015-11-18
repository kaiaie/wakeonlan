# wakeonlan- Wake-on-LAN utility

A small Win32/ WinSock program that sends a Wake-on-LAN "[magic packet](https://en.wikipedia.org/wiki/Wake-on-LAN)" 
to the specified MAC address. There are many such utilities; this one is mine. Written to wake up a machine 
so I could VNC into it.

## Usage

> `wakeonlan MAC-address`

where `MAC address` is a of the form:

> xx:xx:xx:xx:xx:xx
	
or

> xx-xx-xx-xx-xx-xx
	
and `x` is a hexadecimal digit.

## Requirements

Built with MingW32. Compiled binaries may be downloaded at [my website](http://www.kaia.ie)
