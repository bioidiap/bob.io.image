/**
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @date Fri 16 May 12:33:38 2014 CEST
 *
 * @brief Pythonic bindings
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

#include <bob.io.image/bmp.h>
#include <bob.io.image/jpeg.h>


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
    gray_image(i, 100-i) = 127;
    gray_image(i,i) = 255;
  }

  blitz::Array<uint8_t, 3> color_image(3, 100, 100);
  for (int i = 0; i < 3; ++i)
    color_image(i, blitz::Range::all(), blitz::Range::all()) = gray_image(blitz::Range::all(), blitz::Range::all());

  // BMP; only color images are supported
  boost::filesystem::path bmp_color(tempdir); bmp_color /= std::string("color.bmp");
  bob::io::image::write_bmp(color_image, bmp_color.string());
  blitz::Array<uint8_t, 3> color_bmp = bob::io::image::read_bmp<uint8_t>(bmp_color.string());

  if (blitz::any(blitz::abs(color_image - color_bmp) > 0))
    throw std::runtime_error("BMP color image IO did not succeed, check " + bmp_color.string());

#ifdef HAVE_LIBJPEG
  // JPEG
  boost::filesystem::path jpeg_gray(tempdir); jpeg_gray /= std::string("gray.jpg");
  bob::io::image::write_jpeg(gray_image, jpeg_gray.string());
  blitz::Array<uint8_t, 2> gray_jpeg = bob::io::image::read_jpeg<uint8_t, 2>(jpeg_gray.string());

  if (blitz::any(blitz::abs(gray_image - gray_jpeg) > 10))
    throw std::runtime_error("JPEG gray image IO did not succeed, check " + jpeg_gray.string());

  boost::filesystem::path jpeg_color(tempdir); jpeg_color /= std::string("color.jpg");
  bob::io::image::write_jpeg(color_image, jpeg_color.string());
  blitz::Array<uint8_t, 3> color_jpeg = bob::io::image::read_jpeg<uint8_t, 3>(jpeg_color.string());

  if (blitz::any(blitz::abs(color_image - color_jpeg) > 10))
    throw std::runtime_error("JPEG color image IO did not succeed, check " + jpeg_color.string());
#endif

  Py_RETURN_NONE;
BOB_CATCH_FUNCTION("_test_io", 0)
}


static PyMethodDef module_methods[] = {
  {
    s_test_io.name(),
    (PyCFunction)_test_io,
    METH_VARARGS|METH_KEYWORDS,
    s_test_io.doc(),
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

  if (!PyBobIoCodec_Register(".gif", "GIF (giflib)", &make_gif_file)) {
    PyErr_Print();
  }

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
