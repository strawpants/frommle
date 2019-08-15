Developer documentation
***********************

Software Design Documentation 
=============================
This section describes the general idea and workings of the Frommle software.
 
 
Coding style and practices for C++
---------------------
Within **Frommle** boost and templates are heavily used and encouraged. Furthermore, the C+11 standard is adhered to. 
Concerning coding practices the following is used:

* Spaces instead of tabs (4 spaces = 1 tab)
* `K & R indentation style <https://en.wikipedia.org/wiki/Indentation_style#K.26R)>`_. I don't have a particular preference but this one seems ok and widely implemented in automatic formatters (e.g. vim, eclipse, etc.).
* in-source documentation using Doxygen
* C++ header files end with `*.hpp`
* C++ implementation files end with `*.cpp` 
* Function names are mixedCase starting with lower case e.g.: `funcDoSomething()`
* Class names  are CamelCase starting with a capital e.g.: `class MyClass..`
* Class Instances are CamelCase starting with a capital e.g.: `MyClass InstanceX=..`
* Normal variables (int, double, enums, ..) are mixedCase starting with lower case
* private and protected class members and functions have a `_` as suffix
* Global variables (hope we don't need too many of those) are CamelCase
* Preprocessor variables are all capitalized e.g.:`NDEBUG`
* Header files should use unique include guards indicating the path of the file. e.g.: `#ifndef SRC_CORE_USER_SETTINGS_HPP_`
* Namespaces are all lowercase in order ot be consistent with the python module convention (see below)


Coding Style and practices for Python
-------------------------------------
We follow `the guidelines from PEP8 <https://www.python.org/dev/peps/pep-0008/>`_

* module names are all lowercase possibly augemented with underscores
* Class and typenames should be CamelCase
* Functions and variables should be lowercase possibly augmented with underscores
* Constants are all uppercase


Coding style for C++ Python interoperability
--------------------------------------------
* C++ modules (shared libraries) which can be imported in python have a `_` prefixed. c.f. `from frommle._core import MyType (import C++)` with `from frommle.core import MyOtherType (import Python)` 

