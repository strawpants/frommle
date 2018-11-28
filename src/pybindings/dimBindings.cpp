#include <iostream>

using namespace std;

    void say_hello(const char* name) {
    cout << "Hello " <<  name << "!\n";
    }



#include <boost/python.hpp>
using namespace boost::python;

void pyexport_core(){

    def("say_hello", say_hello);
}