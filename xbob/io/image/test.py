#!/usr/bin/env python
# vim: set fileencoding=utf-8 :
# Laurent El Shafey <laurent.el-shafey@idiap.ch>
# Wed Aug 14 12:27:57 CEST 2013
#
# Copyright (C) 2011-2014 Idiap Research Institute, Martigny, Switzerland

"""Runs some image tests
"""

from . import load, test_utils

# These are some global parameters for the test.
PNG_INDEXED_COLOR = test_utils.datafile('img_indexed_color.png', __name__)

def test_png_indexed_color():

  # Read an indexed color PNG image, and compared with hardcoded values
  img = load(PNG_INDEXED_COLOR)
  assert img.shape == (3,22,32)
  assert img[0,0,0] == 255
  assert img[0,17,17] == 117
