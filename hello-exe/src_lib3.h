#ifndef src3
#define src3

#include <string>

namespace lib3 {
    
   std::string hello_world_3 ();

    class person
    {
    private:
        std::string my_name;
        int age;
    public:
        person(std::string name, const int age);

        const std::string &my_name_is();

        const int get_age();
    };    
}

#endif
