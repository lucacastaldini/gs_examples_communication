# gs_examples_communication

### ZMQ

    tested on RP installing from sources libzmq and cppzmq from master branch. 

    dependencies provided as submodules in ./zeromq/deps

    ```
    git submodule update --init
    cd ./zeromq/deps
    ```

    then follow the build guide: https://github.com/zeromq/cppzmq/blob/master/README.md

    to install software:
```
    cd zeromq/
    mkdir build
    cd build 
    cmake ..
    make
```

configuration file "config.txt" stored the ip and port of the tcp connection as `<ip>:<port>` place in the same folder of the executables.