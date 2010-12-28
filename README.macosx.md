Mac OS X
========
I've gone through a lot of trouble compiling the necessary stuff for a Mac OS X build of OpenSkyscraper.


Universal Binaries
------------------
Ideally you build the required stuff as static libraries universally, i.e. for i386, x86 and x86_64. Doing so can be easy or with some libraries rather annoying.

The easy way: `export CFLAGS="-arch i386 -arch x86_64 -arch ppc"` and use `--disable-dependency-tracking` upon configure.

The hard way:

### DevIL
Also do the export thingy but tell DevIL upon `configure` to `--enable-x86 --enable-x86_64 --enable-ppc`.


SDK Incompatibilities
---------------------
I just ran into a problem when trying to compile the OpenSkyscraper project against the 10.5 SDK, without properly building DevIL against it. The resulting error is:
`"_fopen$UNIX2003", referenced from: …`

According to http://cocoawithlove.com/2009/09/building-for-earlier-os-versions-in.html, this is caused by linking different object files that use the same symbol yet from two different SDK versions.

*See below for a fix!*


Building DevIL
--------------
Prepare the environment so that build occurs for the three architectures and against the 10.5 SDK.
`export CFLAGS="-arch i386 -arch x86_64 -arch ppc -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"`
`export CXXFLAGS="$CFLAGS"; export LDFLAGS="$CFLAGS"`

Configure the DevIL appropriately:
`./configure --disable-shared --enable-x86 --enable-x86_64 --enable-ppc --disable-asm --disable-dependency-tracking`
Note that you have to create a symlink MacOSX10.5.sdk/usr/local/include that points at /usr/local/include or the build will fail since png.h is required. You could also add another include path during configuration: `CFLAGS="$CFLAGS -I /usr/local/include"`.