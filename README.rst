.. vim: set fileencoding=utf-8 :
.. Andre Anjos <andre.anjos@idiap.ch>
.. Thu 29 Aug 2013 16:07:57 CEST

.. image:: https://travis-ci.org/bioidiap/xbob.io.image.svg?branch=master
   :target: https://travis-ci.org/bioidiap/xbob.io.image
.. image:: https://coveralls.io/repos/bioidiap/xbob.io.image/badge.png
   :target: https://coveralls.io/r/bioidiap/xbob.io.image
.. image:: http://img.shields.io/github/tag/bioidiap/xbob.io.image.png
   :target: https://github.com/bioidiap/xbob.io.image
.. image:: http://img.shields.io/pypi/v/xbob.io.image.png
   :target: https://pypi.python.org/pypi/xbob.io.image
.. image:: http://img.shields.io/pypi/dm/xbob.io.image.png
   :target: https://pypi.python.org/pypi/xbob.io.image

===========================
 Image I/O Support for Bob
===========================

This package contains no bindings. By importing it, you activate a transparent
plugin that makes possible reading and writing to image files using
:py:mod:`xbob.io.base` functionality.

Installation
------------

Install it through normal means, via PyPI or use ``zc.buildout`` to bootstrap
the package and run test units.

External Library Requirements
=============================

To properly install this package, you will need the following C/C++ components
pre-installed:

1. ``bob-io >= 2.0.0a2``
2. ``libtiff-4``
3. ``libpng``
4. ``libjpeg``
5. ``giflib``
6. ``netpbm``

To test for the availability of the libraries listed above, use the command
``pkg-config``::

  $ pkg-config --modversion libtiff-4
  4.0.2
  $ pkg-config --modversion bob-io
  2.0.0a3

.. note::

   Unfortunately, ``libjpeg``, ``giflib`` and ``netpbm`` do **not** offer
   ``pkg-config`` files as of this time. Detection is based on an algorithm
   implemented inside ``setup.py``. There are no version requirements on these
   libraries. Any version will work.

   If these libraries are installed in non-standard paths on your system, make
   sure to set ``prefixes`` variable on your buildout.

If any of the components is not installed on paths accessible by
``pkg-config``, you have two options:

1. Set the environment variable ``PKG_CONFIG_PATH`` so that ``.pc`` files for
   each of those distributions can be properly located (see ``man pkg-config``
   for details);

2. If you are using ``zc.buildout`` to setup your working environment, you can
   use the buildout variable ``prefixes`` to define the path to the
   installation area for your external packages.

Documentation
-------------

You can generate the documentation for this package, after installation, using
Sphinx::

  $ sphinx-build -b html doc sphinx

This shall place in the directory ``sphinx``, the current version for the
documentation of the package.

Usage
-----

In order to enable support for Matlab(R) file reading and writing in your
application, make sure to import this module, before calling
:py:func:`xbob.io.base.open` or similar::

    >>> import xbob.io.base
    >>> import xbob.io.image
    >>> xbob.io.base.open('myfile.mat', 'r')

Testing
-------

You can run a set of tests using the nose test runner::

  $ nosetests -sv xbob.io.image

.. warning::

   If Bob <= 1.2.1 is installed on your python path, nose will automatically
   load the old version of the insulate plugin available in Bob, which will
   trigger the loading of incompatible shared libraries (from Bob itself), in
   to your working binary. This will cause a stack corruption. Either remove
   the centrally installed version of Bob, or build your own version of Python
   in which Bob <= 1.2.1 is not installed.

You can run our documentation tests using sphinx itself::

  $ sphinx-build -b doctest doc sphinx

You can test overall test coverage with::

  $ nosetests --with-coverage --cover-package=xbob.io.image

The ``coverage`` egg must be installed for this to work properly.

Development
-----------

To develop this package, install using ``zc.buildout``, using the buildout
configuration found on the root of the package::

  $ python bootstrap.py
  ...
  $ ./bin/buildout

Tweak the options in ``buildout.cfg`` to disable/enable verbosity and debug
builds.
