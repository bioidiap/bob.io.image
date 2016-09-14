/**
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @author Manuel Gunther <siebenkopf@googlemail.com>
 * @date Fri 16 May 12:33:38 2014 CEST
 *
 * @brief Pythonic bindings
 *
 * Copyright (C) 2011-2013 Idiap Research Institute, Martigny, Switzerland
 * Copyright (c) 2016, Regents of the University of Colorado on behalf of the University of Colorado Colorado Springs.
 */

#ifdef NO_IMPORT_ARRAY
#undef NO_IMPORT_ARRAY
#endif

#include <bob.blitz/capi.h>
#include <bob.blitz/cleanup.h>
#include <bob.core/api.h>
#include <bob.io.base/api.h>

#include "file.h"

#include <bob.extension/documentation.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <bob.io.image/image.h>


#ifdef HAVE_LIBJPEG
#include <jpeglib.h>
#endif

static auto s_test_io = bob::extension::FunctionDoc(
  "_test_io",
  "Tests the C++ API of reading and writing images"
)
.add_prototype("tempdir")
.add_parameter("tempdir", "str", "A temporary directory to write data to")
;
static PyObject* _test_io(PyObject*, PyObject *args, PyObject* kwds) {
BOB_TRY
  static char** kwlist = s_test_io.kwlist();

  const char* tempdir;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &tempdir)) return 0;

  blitz::Array<uint8_t, 2> gray_image(100, 100);
  gray_image = 0;
  for (int i = 0; i < 100; ++i){
    gray_image(i, 99-i) = 127;
    gray_image(i,i) = 255;
  }

  blitz::Array<uint8_t, 3> color_image(3, 100, 100);
  for (int i = 0; i < 3; ++i)
    color_image(i, blitz::Range::all(), blitz::Range::all()) = gray_image(blitz::Range::all(), blitz::Range::all());

  // BMP; only color images are supported
  boost::filesystem::path bmp(tempdir); bmp /= std::string("color.bmp");
  bob::io::image::write_color_image(color_image, bmp.string());
  if (bob::io::image::get_correct_image_extension(bmp.string()) != ".bmp")
    throw std::runtime_error("BMP image type check did not succeed, check " + bmp.string());
  if (!bob::io::image::is_color_image(bmp.string()))
    throw std::runtime_error("BMP image " + bmp.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_bmp = bob::io::image::read_color_image(bmp.string(), ".bmp");
  if (blitz::any(blitz::abs(color_image - color_bmp) > 0))
    throw std::runtime_error("BMP image IO did not succeed, check " + bmp.string());


#ifdef HAVE_GIFLIB
  // GIF; only color images are supported
  boost::filesystem::path gif(tempdir); gif /= std::string("color.gif");
  bob::io::image::write_color_image(color_image, gif.string());
  if (bob::io::image::get_correct_image_extension(gif.string()) != ".gif")
    throw std::runtime_error("GIF image type check did not succeed, check " + gif.string());
  if (!bob::io::image::is_color_image(gif.string(), ".gif"))
    throw std::runtime_error("GIF image " + gif.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_gif = bob::io::image::read_color_image(gif.string(), ".gif");
  if (blitz::any(blitz::abs(color_image - color_gif) > 8)) // TODO: why is GIF not lossless?
    throw std::runtime_error("GIF image IO did not succeed, check " + gif.string());
#endif

  // NetPBM
  boost::filesystem::path pgm(tempdir); pgm /= std::string("gray.pgm");
  bob::io::image::write_gray_image(gray_image, pgm.string());
  if (bob::io::image::get_correct_image_extension(pgm.string()) != ".pgm")
    throw std::runtime_error("PGM image type check did not succeed, check " + pgm.string());
  if (bob::io::image::is_color_p_m(pgm.string()) || bob::io::image::is_color_image(pgm.string(), ".pgm"))
    throw std::runtime_error("PGM image " + pgm.string() + " is not gray as expected");

  blitz::Array<uint8_t, 2> gray_pgm = bob::io::image::read_gray_image(pgm.string(), ".pgm");
  if (blitz::any(blitz::abs(gray_image - gray_pgm) > 0))
    throw std::runtime_error("PGM image IO did not succeed, check " + pgm.string());

  boost::filesystem::path ppm(tempdir); ppm /= std::string("color.ppm");
  bob::io::image::write_color_image(color_image, ppm.string());
  if (bob::io::image::get_correct_image_extension(ppm.string()) != ".ppm")
    throw std::runtime_error("PPM image type check did not succeed, check " + ppm.string());
  if (!bob::io::image::is_color_p_m(ppm.string()) || !bob::io::image::is_color_image(ppm.string()))
    throw std::runtime_error("PPM image " + ppm.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_ppm = bob::io::image::read_color_image(ppm.string(), ".ppm");
  if (blitz::any(blitz::abs(color_image - color_ppm) > 0))
    throw std::runtime_error("PPM image IO did not succeed, check " + ppm.string());


#ifdef HAVE_LIBJPEG
  // JPEG
  boost::filesystem::path jpeg_gray(tempdir); jpeg_gray /= std::string("gray.jpg");
  bob::io::image::write_gray_image(gray_image, jpeg_gray.string());
  if (bob::io::image::get_correct_image_extension(jpeg_gray.string()) != ".jpg")
    throw std::runtime_error("JPEG image type check did not succeed, check " + jpeg_gray.string());
  if (bob::io::image::is_color_image(jpeg_gray.string()) || bob::io::image::is_color_image(jpeg_gray.string()))
    throw std::runtime_error("JPEG image " + jpeg_gray.string() + " is not gray as expected");

  blitz::Array<uint8_t, 2> gray_jpeg = bob::io::image::read_gray_image(jpeg_gray.string());
  if (blitz::any(blitz::abs(gray_image - gray_jpeg) > 10))
    throw std::runtime_error("JPEG gray image IO did not succeed, check " + jpeg_gray.string());

  boost::filesystem::path jpeg_color(tempdir); jpeg_color /= std::string("color.jpg");
  bob::io::image::write_color_image(color_image, jpeg_color.string());
  if (bob::io::image::get_correct_image_extension(jpeg_color.string()) != ".jpg")
    throw std::runtime_error("JPEG image type check did not succeed, check " + jpeg_color.string());
  if (!bob::io::image::is_color_image(jpeg_color.string()) || !bob::io::image::is_color_image(jpeg_color.string(), ".jpeg"))
    throw std::runtime_error("JPEG image " + jpeg_color.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_jpeg = bob::io::image::read_color_image(jpeg_color.string());
  if (blitz::any(blitz::abs(color_image - color_jpeg) > 10))
    throw std::runtime_error("JPEG color image IO did not succeed, check " + jpeg_color.string());
#endif

#ifdef HAVE_LIBPNG
  // PNG
  boost::filesystem::path png_gray(tempdir); png_gray /= std::string("gray.png");
  bob::io::image::write_gray_image(gray_image, png_gray.string());
  if (bob::io::image::get_correct_image_extension(png_gray.string()) != ".png")
    throw std::runtime_error("PNG image type check did not succeed, check " + png_gray.string());
  if (bob::io::image::is_color_image(png_gray.string()) || bob::io::image::is_color_image(png_gray.string(), ".png"))
    throw std::runtime_error("PNG image " + png_gray.string() + " is not gray as expected");

  blitz::Array<uint8_t, 2> gray_png = bob::io::image::read_gray_image(png_gray.string());
  if (blitz::any(blitz::abs(gray_image - gray_png) > 1))
    throw std::runtime_error("PNG gray image IO did not succeed, check " + png_gray.string());

  boost::filesystem::path png_color(tempdir); png_color /= std::string("color.png");
  bob::io::image::write_color_image(color_image, png_color.string());
  if (bob::io::image::get_correct_image_extension(png_color.string()) != ".png")
    throw std::runtime_error("PNG image type check did not succeed, check " + png_color.string());
  if (!bob::io::image::is_color_png(png_color.string()) || !bob::io::image::is_color_image(png_color.string()))
    throw std::runtime_error("PNG image " + png_color.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_png = bob::io::image::read_color_image(png_color.string());
  if (blitz::any(blitz::abs(color_image - color_png) > 1))
    throw std::runtime_error("PNG color image IO did not succeed, check " + png_color.string());
#endif

#ifdef HAVE_LIBTIFF
  // TIFF
  boost::filesystem::path tiff_gray(tempdir); tiff_gray /= std::string("gray.tiff");
  bob::io::image::write_gray_image(gray_image, tiff_gray.string());
  if (bob::io::image::get_correct_image_extension(tiff_gray.string()) != ".tiff")
    throw std::runtime_error("TIFF image type check did not succeed, check " + tiff_gray.string());
  if (bob::io::image::is_color_image(tiff_gray.string()) || bob::io::image::is_color_image(tiff_gray.string(), ".tif"))
    throw std::runtime_error("TIFF image " + tiff_gray.string() + " is not gray as expected");

  blitz::Array<uint8_t, 2> gray_tiff = bob::io::image::read_gray_image(tiff_gray.string());
  if (blitz::any(blitz::abs(gray_image - gray_tiff) > 1))
    throw std::runtime_error("TIFF gray image IO did not succeed, check " + tiff_gray.string());

  boost::filesystem::path tiff_color(tempdir); tiff_color /= std::string("color.tiff");
  bob::io::image::write_color_image(color_image, tiff_color.string());
  if (bob::io::image::get_correct_image_extension(tiff_color.string()) != ".tiff")
    throw std::runtime_error("TIFF image type check did not succeed, check " + tiff_color.string());
  if (!bob::io::image::is_color_image(tiff_color.string()) || !bob::io::image::is_color_image(tiff_color.string(), ".tiff"))
    throw std::runtime_error("TIFF image " + tiff_color.string() + " is not color as expected");

  blitz::Array<uint8_t, 3> color_tiff = bob::io::image::read_color_image(tiff_color.string());
  if (blitz::any(blitz::abs(color_image - color_tiff) > 1))
    throw std::runtime_error("TIFF color image IO did not succeed, check " + tiff_color.string());
#endif

  Py_RETURN_NONE;
BOB_CATCH_FUNCTION("_test_io", 0)
}


static auto s_image_extension = bob::extension::FunctionDoc(
  "get_correct_image_extension",
  "Estimates the image type and return a corresponding extension based on file content",
  "This function loads the first bytes of the given image, and matches it with known magic numbers of image files. "
  "If a match is found, it returns the corresponding image extension (including the leading ``'.'`` that can be used, e.g., in :py:func:`bob.io.image.load`."
)
.add_prototype("image_name", "extension")
.add_parameter("image_name", "str", "The name (including path) of the image to check")
.add_return("extension", "str", "The extension of the image based on the file content")
;
static PyObject* image_extension(PyObject*, PyObject *args, PyObject* kwds) {
BOB_TRY
  static char** kwlist = s_image_extension.kwlist();

  const char* image_name;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &image_name)) return 0;

  return Py_BuildValue("s", bob::io::image::get_correct_image_extension(image_name).c_str());

BOB_CATCH_FUNCTION("get_correct_image_extension", 0)
}


static PyMethodDef module_methods[] = {
  {
    s_test_io.name(),
    (PyCFunction)_test_io,
    METH_VARARGS|METH_KEYWORDS,
    s_test_io.doc(),
  },
  {
    s_image_extension.name(),
    (PyCFunction)image_extension,
    METH_VARARGS|METH_KEYWORDS,
    s_image_extension.doc(),
  },
  {0}  /* Sentinel */
};

PyDoc_STRVAR(module_docstr, "Image I/O support for Bob");

#if PY_VERSION_HEX >= 0x03000000
static PyModuleDef module_definition = {
  PyModuleDef_HEAD_INIT,
  BOB_EXT_MODULE_NAME,
  module_docstr,
  -1,
  module_methods,
  0, 0, 0, 0
};
#endif

static PyObject* create_module (void) {

# if PY_VERSION_HEX >= 0x03000000
  PyObject* m = PyModule_Create(&module_definition);
  auto m_ = make_xsafe(m);
  const char* ret = "O";
# else
  PyObject* m = Py_InitModule3(BOB_EXT_MODULE_NAME, module_methods, module_docstr);
  const char* ret = "N";
# endif
  if (!m) return 0;

  /* imports dependencies */
  if (import_bob_blitz() < 0) return 0;
  if (import_bob_core_logging() < 0) return 0;
  if (import_bob_io_base() < 0) return 0;

  /* activates image plugins */
  if (!PyBobIoCodec_Register(".tif", "TIFF, compresssed (libtiff)",
        &make_tiff_file)) {
    PyErr_Print();
  }

  if (!PyBobIoCodec_Register(".tiff", "TIFF, compresssed (libtiff)",
        &make_tiff_file)) {
    PyErr_Print();
  }

#ifdef HAVE_LIBJPEG
  if (BITS_IN_JSAMPLE == 8) {
    if (!PyBobIoCodec_Register(".jpg", "JPEG, compresssed (libjpeg)",
          &make_jpeg_file)) {
      PyErr_Print();
    }
    if (!PyBobIoCodec_Register(".jpeg", "JPEG, compresssed (libjpeg)",
          &make_jpeg_file)) {
      PyErr_Print();
    }
  }
  else {
    PyErr_Format(PyExc_RuntimeError, "libjpeg compiled with `%d' bits depth (instead of 8). JPEG images are hence not supported.", BITS_IN_JSAMPLE);
    PyErr_Print();
  }
#endif

#ifdef HAVE_GIFLIB
  if (!PyBobIoCodec_Register(".gif", "GIF (giflib)", &make_gif_file)) {
    PyErr_Print();
  }
#endif // HAVE_GIFLIB

  if (!PyBobIoCodec_Register(".pbm", "PBM, indexed (libnetpbm)",
        &make_netpbm_file)) {
    PyErr_Print();
  }

  if (!PyBobIoCodec_Register(".pgm", "PGM, indexed (libnetpbm)",
        &make_netpbm_file)) {
    PyErr_Print();
  }

  if (!PyBobIoCodec_Register(".ppm", "PPM, indexed (libnetpbm)",
        &make_netpbm_file)) {
    PyErr_Print();
  }

  if (!PyBobIoCodec_Register(".png", "PNG, compressed (libpng)", &make_png_file)) {
    PyErr_Print();
  }

  if (!PyBobIoCodec_Register(".bmp", "BMP, (built-in codec)", &make_bmp_file)) {
    PyErr_Print();
  }

  return Py_BuildValue(ret, m);
}

PyMODINIT_FUNC BOB_EXT_ENTRY_NAME (void) {
# if PY_VERSION_HEX >= 0x03000000
  return
# endif
    create_module();
}
