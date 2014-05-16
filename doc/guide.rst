.. vim: set fileencoding=utf-8 :
.. Andre Anjos <andre.dos.anjos@gmail.com>
.. Fri 16 May 11:48:13 2014 CEST

.. testsetup:: *

   import numpy
   import xbob.io.base
   import xbob.io.image
   import tempfile
   import os

   current_directory = os.path.realpath(os.curdir)
   temp_dir = tempfile.mkdtemp(prefix='bob_doctest_')
   os.chdir(temp_dir)

============
 User Guide
============

By importing this package, you can use |project| native array reading and
writing routines to load and save files using various image formats.

.. code-block:: python

   >> import xbob.io.base
   >> import xbob.io.image #under the hood: loads Bob plugin for image files
   >> x = xbob.io.base.load('myfile.jpg')

In the following example, an image generated randomly using the method `NumPy`
:py:meth:`numpy.random.random_integers`, is saved in JPEG format. The image
must be of type ``uint8`` or ``uint16``:

.. doctest::

  >>> my_image = numpy.random.random_integers(0,255,(3,256,256))
  >>> xbob.io.base.save(my_image.astype('uint8'), 'testimage.jpg') # saving the image in jpeg format
  >>> my_image_copy = xbob.io.base.load('testimage.jpg')

The loaded image files can be 3D arrays (for RGB format) or 2D arrays (for
greyscale) of type ``uint8`` or ``uint16``.

.. testcleanup:: *

  import shutil
  os.chdir(current_directory)
  shutil.rmtree(temp_dir)
