.. frommle documentation master file, created by
   sphinx-quickstart on Sat Nov 17 22:08:19 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Frommle: a python/C++ toolbox for (geodetic) Earth Science
==========================================================
What does Frommle do?
---------------------
Frommle is a python/C++ software suite to perform scientific computations in the field of geodesy and Earth Sciences. The general goal is to bundle general low-level building blocks in a library designed with scientific computing and needs in mind, and add specialized functionality related to stuff we use all the time in Earth Sciences. 

What does Frommle mean?
-----------------------
Frommle comes from the Dutch word:

From-me-len (frommelde, heeft gefrommeld)
-
1 op knoeierige manier wegstoppen

`Source Van Dale <http://www.vandale.nl/opzoeken?pattern=frommel&lang=nn>`_

Which means: "to tuck away in a unorderly fashion".

Ironically, this is the opposite what Frommle is supposed to represent. To put this in perspective, the name orginated from my growing frustration with sofware I've previously written as a scientist. Although the software did its job, it became clear that scaling the software (mostly fortran90 stitched up with bash scripts) to larger scientific problems, would require a disproportionate amount of unsatisfying refactoring. Another issue is that, as a starting PhD student, I didn't envision in which way my software would evolve let alone consider that it might be useful for other people. It is from this mindset, that I started to develop Frommle hoping to order my thoughts and software (hence the ordely coat hanger).


R. Rietbroek, Nov 2018
roelof(at)wobbly.earth

How can you use frommle?
------------------------
Frommle is licensed under the GNU LESSER GENERAL PUBLIC LICENSE v3. This means that you're free to use the software and also link to the libraries from commercial software. You can have a more detailed look at `the license <license.html>`_


.. toctree::
   :maxdepth: 2
   :caption: Contents:
   
   installation
   license
   developer
   cpp

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`




