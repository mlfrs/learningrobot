    # learningrobot - Machine Learning for Robotic Simulation

    We're going to try to teach some simulated robots to stand up and walk around in a virtual world
    This doc lists requirements and installation details.

    Tested against gentoo and debian.

    - Software Requirements. The Makefile assumes all packages should be available via pkg-config

    - Google Protobuffers https://developers.google.com/protocol-buffers/
		- Protobuffers are used as a message passing interface between our components.
		# Be sure to install protobuf 3.0 or greater.
        $ sudo apt-get install libprotobuf-dev
        # or build from source with cmake by following 'protobuf/src/README.md'
        $ git clone https://github.com/google/protobuf
        $ cd protobuf
		$ ./autogen.sh
		$ ./configure
		$ make
		$ make check
		$ sudo make install
		$ sudo ldconfig # refresh shared library cache.

	- GRPC http://www.grpc.io/
		- GRPC enables client server communications.
		# Install instructions are here.
		https://github.com/grpc/grpc/tree/release-0_14/src/cpp
		# If you want to install it in /usr rather than /usr/local
		# Edit the Makefile:258 and change 'prefix ?= /usr/local' to 'prefix ?= /usr'
		$ git clone https://github.com/grpc/grpc.git
		$ cd grpc
		$ git submodule update --init
		$ sudo make install
		
    - Simbody https://github.com/simbody/simbody/releases
		- Simbody is our primary physics simulating software (TODO: add bullet)
        # Simbody is big and you may have some issues bulding it.
        # You may need to install some other dependencies for visualization(glut) 
		# and documentation(doxygen)
        $ sudo apt-get install freeglut3-dev libxmu-dev libxi-dev doxygen

        # Download latest release eg. simbody-Simbody-3.5.3.tar.gz
        $ mv simbody-Simbody-3.5.3/ ~/simbody-source
        # or 
        $ git clone https://github.com/simbody/simbody.git ~/simbody-source
        $ cd ~/simbody-source
		$ git checkout Simbody-3.5.3

        # Build with cmake & make (for other options look at their README.md)
        $ cmake ~/simbody-source -DCMAKE_INSTALL_PREFIX=/usr/local/ -DCMAKE_BUILD_TYPE=RelWithDebInfo
        $ make 
        $ sudo make install

        # Simbody should now be installed, but for me it didn't seem to work..
		# So I did it manually.
		$ sudo cp ~/simbody/lib64/lib* /usr/local/lib/
		$ sudo cp ~/simbody/lib64/pkgconfig/simbody.pc /usr/lib/pkgconfig/
        $ sudo chmod 644 /usr/lib/pkgconfig/simbody.pc

        # It also didn't install the headers, this is messy but something like this..
		$ sudo cp -R ~/simbody/include/* /usr/local/include/
		$ sudo cp -R ~/simbody/include/simbody/* /usr/local/include/
		$ sudo chmod -R 755 /usr/local/include/sim*
		$ sudo chmod -R 755 /usr/local/include/Sim*

        # Some additional libraries are also required for the linker.
        $ sudo apt-get install liblapack-dev

        # Now it should be good to go, you can test by trying to make the Makefile
		# note if when running the simulation the visualizer does not display, that
		# may indicate that the visualizer is not available at runtime. For example
		# it may be installed somewhere that your user does not have permissions to 
		# access. To workaround you can either copy it(simbody-visualizer) to the
		# correct libexec directory or just place it in the root directors of the 
		# runtime application directory.

    - jsoncpp https://github.com/open-source-parsers/jsoncpp
		- Json is nicely readable, currently using for robot model format.
		#(required scons) I built from source, but should be something like.
		$ sudo apt-get install scons
		$ sudo apt-get install libjsoncpp-dev

	- Eigen3 https://github.com/open-source-parsers/jsoncpp
		- Excellent linear algebra library.
		# recomment build from source but should easily installed with
		$ sudo apt-get install libeigen3-dev


	# Troubleshooting
	
	- Linking issues.
		- json throws linker errors.
		# Recomend copying the makefile and manually linking it in.. 
		eg.
		${LINKER} -o mlfrs $(BUILDDIR)....o /path/to/libjsoncpp.a
		- SimTK throws linker errors.
		# Unsure but check that pkg-config is setp correctly.
		# I noticed simbody.pc linking to strange places like build dir.
	
	- GRPC complains at install time that..
		# Warning: it looks like protoc 3.0.0+ isn't installed on your system..
		# It seems like you can ignore as it worked fine.
