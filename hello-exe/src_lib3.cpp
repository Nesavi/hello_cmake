#include <string>
#include "src_lib3.h"

namespace lib3 {
    person::person(std::string name, const int age) : my_name{name}, age{age}{};

    const std::string &person::my_name_is(){
        return my_name;
    }

    const int person::get_age(){
        return age;
    }

    std::string hello_world_3()
    {
           return "Hello from lib3";
       }
}
