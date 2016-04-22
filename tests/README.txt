To build all test executables please use makefile which is suitable for your's OS.

Right now only Mint 17 'Qiana', Debian 8.4 'Jessie' are fully supported.
Windows 8 has problem with test.cpp compilation, so on this system please to
change name of file test.cpp to test.cpp_OLD.

To use specific makefile write in terminal "make -f makefile_name", i.e.
to build on Mint 17 'Qiana' write "make -f Makefile_mint".

Required Mint's (or Debians') packages:
g++
qt5-default
libboost-all-dev

Required Windows 8 programs:
Qt 5.6.0 with MinGW toolchain.

If any error occurs please contact with:
Filip Wrobel - f.wrobel@stud.elka.pw.edu.pl
or
Michal Andruszkiewicz - m.andruszkiewicz@stud.elka.pw.edu.pl
or
Konrad Gotfryd - k.gotfryd@stud.elka.pw.edu.pl
