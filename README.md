    # learningrobot - Machine Learning for Robotic Simulation

    We're going to try to teach some simulated robots to stand up and walk around in a virtual world
    This doc lists requirements and installation details.
    The doc assumes debian. I'm using gentoo personally but I tested with debian.

    - Software Requirements. The Makefile assumes all packages should be available via pkg-config

    - Google Protobuffers https://developers.google.com/protocol-buffers/

        $ sudo apt-get install libprotobuf-dev
        - or build from source with cmake, maybe something like this.
        $ git clone https://github.com/google/protobuf ~/protobuf-source
        $ cmake .
        $ make 
        $ sudo make install


    - Simbody https://github.com/simbody/simbody/releases

        - Simbody is big and you may have some issues bulding it.
        - You may need to install some other dependencies for visualization(glut) and documentation(doxygen)
        $ sudo apt-get install freeglut3-dev libxmu-dev libxi-dev doxygen

        - Download latest release eg. simbody-Simbody-3.5.3.tar.gz
        $ mv simbody-Simbody-3.5.3/ ~/simbody-source
        - or 
        $ git clone https://github.com/simbody/simbody.git ~/simbody-source
        $ cd ~/simbody-source
		$ git checkout Simbody-3.5.3

        - Build with cmake & make (for other options look at their README.md)
        $ cmake ~/simbody-source -DCMAKE_INSTALL_PREFIX=~/simbody -DCMAKE_BUILD_TYPE=RelWithDebInfo
        $ make 
        $ sudo make install

        - Simbody should now be installed, but for me it didn't seem to work..
		- So I did it manually.
		$ sudo cp ~/simbody/lib64/lib* /usr/local/lib/
		$ sudo cp ~/simbody/lib64/pkgconfig/simbody.pc /usr/lib/pkgconfig/
        $ sudo chmod 644 /usr/lib/pkgconfig/simbody.pc

        - It also didn't install the headers, this is messy but something like this..
		$ sudo cp -R ~/simbody/include/* /usr/local/include/
		$ sudo cp -R ~/simbody/include/simbody/* /usr/local/include/
		$ sudo chmod -R 755 /usr/local/include/sim*
		$ sudo chmod -R 755 /usr/local/include/Sim*

        - Finally some additional libraries are also required for the linker.
        $ sudo apt-get install liblapack-dev

        # Now it should be good to go, you can test by trying to make the Makefile

