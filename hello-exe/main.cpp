#pragma region OLD MAIN
#include <iostream>
#include "src1.h"
#include "src2_lib2.h"
#include "src_lib3.h"
#include <say-hello/hello.h>
#include <boost/lambda/lambda.hpp>
#include <boost/timer/timer.hpp>
#include <iterator>
#include <algorithm>
#include <boost-hello/boost-hello.hpp>

using namespace std;

int main()
{
    cout << helloboost() << endl;
    cout << "Hello World!" << endl;

    cout << lib1::hello_world() << endl;

    lib2::src2_lib2 foo{"CMake test"};

    cout << foo.my_name_is() << endl;


    lib3::person p{"Bibi", 0};

    cout << "Name: " << p.my_name_is() << " and age is " << p.get_age() << endl;

    hello::say_hello();

    std::cout << "say-hello version is " << SAY_HELLO_VERSION << std::endl;

    // using namespace boost::lambda;
    // typedef std::istream_iterator<int> in;

    // std::for_each(
    //     in(std::cin), in(), std::cout << (_1 * 3) << " " );

    boost::timer::cpu_timer aclock{};


    size_t j = 0;
    for (size_t i = 0; i < 1000000000; ++i)
    {
        j = i * 2;
    }
    
    aclock.stop();

    std::cout << "Elapsed time " << aclock.format() << std::endl;
    std::cout << j << std::endl;
    return 0;
}
#pragma endregion