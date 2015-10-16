#Environment setup

# Introduction #

This wiki describes how to set up the environment.


# Details #

When developing on a windows system:
- Download mingw from: http://www.mingw.org/category/wiki/download
- Set the path variable in you system variabled to the following folder: C:\MinGW\msys\1.0\bin\
- download and install the arm toolchain on: https://launchpad.net/gcc-arm-embedded/+download


- Open eclipse and select the eclipse workspace which can be found in svn.
- Download the gnu c/c++ support plugin for eclipse on: http://sourceforge.net/projects/gnuarmeclipse/
- Install this plugin in eclipse

After installing this plugin and a reboot of eclipse:

press file->Import
select "Exisiting project into workspace"
and clock next.
Select the root directory of the project in svn. and click finish

Now you should be able to build the source.

# Known Issues #

It can be that you have a compiler error saying that it can't find the lpc17xxadc.h file. If this is the case, right click on your project and select properties.
select c/c++general->Paths and symbols. Select the Includes tab en then select the GNU C language. Now click add->workspace. Select quadcopterlpc17xx\_lib->include and click ok twice.
Now you should be able to build the project.