To build/run RubySFML in Windows:

1) Install Ruby for Windows using the one-click installer.
   (RubySFML can't use it until it starts being built with VS 2005,
    but it needs to be installed to run the Rakefile.)

2) Run "gem install rake" to install Ruby's version of make.

3) Open a DOS box to this folder.

4) Make sure all the VS2005 environment vars are set. If necessary, run:
   <VS2005 Path>\VC\bin\vcvars32.bat

5) Run "rake" in this folder. The default build target will:
   * Download/unpack/compile/install the ruby-1.8.6 source tarball.
   * Install Ruby Gems (not strictly necessary, but everyone will want it).
   * Configure/compile/install the RubySFML extension.
   * Build RubySFML.zip with everything you need to distribute Windows binaries.
     (At under 3.7MB, it has all the Ruby bins/libs plus all the SFML bins.)

6) At this point you can use any of these commands to run the test program:
   * test.bat
   * rake test
   * bin\ruby test.rb

NOTE: This build of Ruby does not have all the extensions included in the
      one-click installer. Some notable ones missing at the moment are
      DBI, OpenSSL, and Expat.
