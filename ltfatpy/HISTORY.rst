.. :changelog:

History
=======


0.1.0 (2014-04-30)
------------------
Creation

1.0.0 (2015-12-15)
------------------
Based on ltfat commit 0f9c83d96b (version 2.1.0)

1.0.1 (2023-02-27)
------------------
Update the .c file with a newer version of cython

.. code:: bash

    find ltfatpy/comp -name '*.pyx' -exec cython {} \;

Pip install now works on newer version of python >= 3.8.
The procedure has been tested on mac with an arm processor.