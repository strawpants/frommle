Developer documentation
***********************

Software Design Documentation 
=============================
This section describes the general idea and workings of the Frommle software.
 
 
 C++ coding style
 ---------------------
 Within **Frommle** boost and templates are heavily used and encouraged. Furthermore, the C+11 standard is adhered to. 
 Concerning coding practices the following is used:
 * Tabs instead of spaces
 * `K & R indentation style <https://en.wikipedia.org/wiki/Indentation_style#K.26R)>`_. I don't have a particular preference but this one seems ok and widely implemented in automatic formatters (e.g. vim, eclipse, etc.).
 * in-source documentation using Doxygen
 * C++ header files end with `*.hpp`
 * C++ implementation files end with `*.cpp` 
 * Function names are mixed case starting with lower case e.g.: `funcDoSomething()`
 * Class names  are mixed case starting with a capital e.g.: ` class MyClass..`
 * Class Instances are mixed case starting with a capital
 * Normal variables (int, double, enums, ..) are mixed case starting with lower case
 * private class members and functions have a `_` as suffix
 * Global variables (hope we don't need those)
 * Header files should use include guards
 * Namespaces are all lowercase

 #### Documentating style for C++ Classes

Python
======


C++
===

