#!/usr/bin/env python
# vim: set fileencoding=utf-8 :
# Andre Anjos <andre.anjos@idiap.ch>
# Mon 16 Apr 08:18:08 2012 CEST

from setuptools import setup, find_packages, dist
dist.Distribution(dict(setup_requires=['xbob.blitz', 'xbob.io.base']))
from xbob.blitz.extension import Extension
import xbob.io.base

include_dirs = [xbob.io.base.get_include()]

packages = ['bob-io >= 2.0.0a2']
version = '2.0.0a0'

# Check if python-imaging means pil or pillow
pil_or_pillow = []
try:
  import pkg_resources
  pkg_resources.require('PIL')
  pil_or_pillow.append('pil')
except pkg_resources.DistributionNotFound as e:
  pil_or_pillow.append('pillow')

setup(

    name='xbob.io.image',
    version=version,
    description='Image I/O support for Bob',
    url='http://github.com/bioidiap/xbob.io.image',
    license='BSD',
    author='Andre Anjos',
    author_email='andre.anjos@idiap.ch',

    long_description=open('README.rst').read(),

    packages=find_packages(),
    include_package_data=True,

    install_requires=[
      'setuptools',
      'xbob.blitz',
      'xbob.io.base',
    ] + pil_or_pillow,

    namespace_packages=[
      "xbob",
      "xbob.io",
      ],

    ext_modules = [
      Extension("xbob.io.image.version",
        [
          "xbob/io/image/version.cpp",
          ],
        packages = packages,
        include_dirs = include_dirs,
        version = version,
        ),
      Extension("xbob.io.image._library",
        [
          "xbob/io/image/tiff.cpp",
          "xbob/io/image/gif.cpp",
          "xbob/io/image/png.cpp",
          "xbob/io/image/jpeg.cpp",
          "xbob/io/image/bmp.cpp",
          "xbob/io/image/netpbm.cpp",
          "xbob/io/image/file.cpp",
          "xbob/io/image/main.cpp",
          ],
        packages = packages,
        include_dirs = include_dirs,
        version = version,
        ),
      ],

    classifiers = [
      'Development Status :: 3 - Alpha',
      'Intended Audience :: Developers',
      'License :: OSI Approved :: BSD License',
      'Natural Language :: English',
      'Programming Language :: Python',
      'Programming Language :: Python :: 3',
      'Topic :: Software Development :: Libraries :: Python Modules',
      'Environment :: Plugins',
      ],

    )
