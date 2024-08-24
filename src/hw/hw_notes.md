This is the hardware definition folder for supported platforms.
To make a new platform for BemaniUX to be compiled to, you will need to make the following libraries at a minimum within a folder for your arch:
* gpu.hpp
* gpufont.hpp
* windowproc.hpp (For PC OSes like Windows/Mac/Linux)
* and atleast one of the input libraries.

Then once you have made these libraries, add the arch to the various HW selection headers to allow the arch to be selected.

Once you have made these changes, the code should compile and display.