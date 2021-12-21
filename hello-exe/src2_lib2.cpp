#include <string>

#include "src2_lib2.h"

namespace lib2 {
    
    std::string hello_world (){
        return "Hello, world. This is src1 calling.";
    }

    lib2::src2_lib2::src2_lib2(std::string name) : my_name{name}
    {

    }

    const std::string &lib2::src2_lib2::my_name_is()
    {
        return my_name;
    }

}




