# gs_examples_communication

### ZMQ

    tested on RP installing from sources libzmq and cppzmq from master branch. 

    zeromq folder has the required code to send packets via cppzmq

    it is provided a standalone example to stream integers.
    Examples streaming data packets with serialization are provided and require libraries built on 
    the project https://github.com/lucacastaldini/gs_examples_serialization.git, provided as submodule.


    other dependencies provided as submodules in ./zeromq/deps

    ```
    git submodule update --init
    cd ./zeromq/deps
    ```

    then follow the build guide: https://github.com/zeromq/cppzmq/blob/master/README.md

    

    to install software, create a build folder and change dir into:
```
    ...gs_examples_communication$ mkdir build
    ...gs_examples_communication$ cd build 
```
    to build all:
```
    cmake ..
    make
```
    to build only specific tests, it is possbile to select the build target by specifying them via command line arguments. It is shown how to compile only Avro and CCSDS tests:
```
    cmake .. -DBUILD_EXAMPLES=OFF -DBUILD_AVRO=ON -DBUILD_PB=OFF -DBUILD_CCSDS=ON -DBUILD_SHM=OFF
    make
```

configuration file "config.txt" stored the ip and port of the tcp connection as `<ip>:<port>` place in the same folder of the executables.