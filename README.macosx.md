Mac OS X
========
I've gone through a lot of trouble compiling the necessary stuff for a Mac OS X build of OpenSkyscraper.


Universal Binaries
------------------
Ideally you build the required stuff as static libraries universally, i.e. for i386, x86 and x86_64. Doing so can be easy or with some libraries rather annoying.

The easy way:
`export CFLAGS="-arch i386 -arch x86 -arch ppc"`

The hard way:

### DevIL
Also do the export thingy but tell DevIL upon `configure` to `--enable-x86 --enable-x86_64 --enable-ppc`.


SDK Incompatibilities
---------------------
I just ran into a problem when trying to compile the OpenSkyscraper project against the 10.5 SDK, without properly building DevIL against it. The resulting error is:
`"_fopen$UNIX2003", referenced from: …`

According to http://cocoawithlove.com/2009/09/building-for-earlier-os-versions-in.html, this is caused by linking different object files that use the same symbol yet from two different SDK versions.

I have yet to find a way to fix this issue...
