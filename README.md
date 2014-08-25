Building a WinARM-using project on Linux.  Why?  Sometimes
life just works out that way.

Method:
 - Have gnu arm supported installed.
 - Use `Example-CMakeLists.txt` as a model.
 - Configure build using `cmake -DWinArm_DIR=$HERE -DSIMULATE=TRUE/FALSE ..`
