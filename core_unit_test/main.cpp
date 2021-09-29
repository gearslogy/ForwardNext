#include <iostream>



#define BOOST_TEST_MODULE header-only multiunit test
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API

#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;

#undef OUT
#undef IN

void make_use_of(char**)
{
    std::cout << "Using custom entry point..." << std::endl;
}

int main(int argc, char* argv[], char* envp[])
{
    make_use_of(envp);
    return utf::unit_test_main(init_unit_test, argc, argv);
}

