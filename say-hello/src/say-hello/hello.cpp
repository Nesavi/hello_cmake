#include <iostream>
#include "hello.h"

namespace hello{
    void say_hello(){
        std::cout << "Hello from say-hello VERSION " << SAY_HELLO_VERSION << std::endl;
    }
}