#ifndef src2
#define src2

#include <string>

namespace lib2 {
    
   std::string hello_world ();

    class src2_lib2
    {
    private:
        std::string my_name;
    public:
        src2_lib2(std::string name);

        const std::string &my_name_is();
    };    
}

#endif
