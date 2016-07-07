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
        $ git checkout Simbody-3.5.3

        - Build with cmake & make (for other options look at their README.md)
        $ cmake ~/simbody-source -DCMAKE_INSTALL_PREFIX=~/simbody -DCMAKE_BUILD_TYPE=RelWithDebInfo
        $ make 
        $ sudo make install

        - Simbody should now be installed, but for some reason they don't seem to add pkg-config by default
        $ sudo cp ~/simbody-source/cmake/pkgconfig/simbody.pc /usr/lib/pkgconfig/
        $ sudo chmod 644 /usr/lib/pkgconfig/simbody.pc

        - It also didn't install the headers so you may need to manually copy them.. (ugh.. there are more than this too)
        $ sudo chmod -R 644 ~/simbody-source/Simbody/include/
        $ sudo chmod -R 644 ~/simbody-source/SimTKcommon/include/
        $ sudo chmod -R 644 ~/simbody-source/SimTKmath/include/
        $ sudo cp -R ~/simbody-source/Simbody/include/* /usr/local/include/
        $ sudo cp -R ~/simbody-source/SimTKcommon/include/* /usr/local/include/
        $ sudo cp -R ~/simbody-source/SimTKmath/include/* /usr/local/include/
        $ for i in $(find .  |grep '/SimTKcommon/internal');do sudo cp $i /usr/local/include/SimTKcommon/internal/;done

        - Some additional libraries are also required for the linker.
        $ sudo apt-get install liblapack-dev

        # Now it should be good to go, you can test by trying to make the Makefile

