.. vim: set fileencoding=utf-8 :
.. Andre Anjos <andre.anjos@idiap.ch>
.. Thu 29 Aug 2013 16:07:57 CEST

.. image:: https://travis-ci.org/bioidiap/xbob.io.matlab.svg?branch=master
   :target: https://travis-ci.org/bioidiap/xbob.io.matlab
.. image:: https://coveralls.io/repos/bioidiap/xbob.io.matlab/badge.png
   :target: https://coveralls.io/r/bioidiap/xbob.io.matlab
.. image:: http://img.shields.io/github/tag/bioidiap/xbob.io.matlab.png
   :target: https://github.com/bioidiap/xbob.io.matlab
.. image:: http://img.shields.io/pypi/v/xbob.io.matlab.png
   :target: https://pypi.python.org/pypi/xbob.io.matlab
.. image:: http://img.shields.io/pypi/dm/xbob.io.matlab.png
   :target: https://pypi.python.org/pypi/xbob.io.matlab

===================================
 Support for Matlab I/O in bob::io
===================================

This package contains no bindings. By importing it, you activate a transparent
plugin that makes possible reading and writing to Matlab(R) files using
:py:mod:`xbob.io.base` functionality.

Installation
------------

Install it through normal means, via PyPI or use ``zc.buildout`` to bootstrap
the package and run test units.

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
    >>> import xbob.io.matlab
    >>> xbob.io.base.open('myfile.mat', 'r')

Testing
-------

You can run a set of tests using the nose test runner::

  $ nosetests -sv xbob.io.matlab

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

  $ nosetests --with-coverage --cover-package=xbob.io.matlab

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
