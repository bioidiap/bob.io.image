/**
 * @author Manuel Guenther <manuel.guenther@idiap.ch>
 * @date Thu 21 May 16:23:42 CEST 2015
 *
 * @brief Versioning information for bob.io.image
 */

#ifndef BOB_IO_IMAGE_CONFIG_H
#define BOB_IO_IMAGE_CONFIG_H

#ifdef BOB_IMPORT_VERSION

  /***************************************
  * Here we define some functions that should be used to build version dictionaries in the version.cpp file
  * There will be a compiler warning, when these functions are not used, so use them!
  ***************************************/

  #include <Python.h>
  #include <boost/preprocessor/stringize.hpp>

  extern "C" {
    #include <jpeglib.h>

    #define PNG_SKIP_SETJMP_CHECK
    // #define requires because of the problematic pngconf.h.
    // Look at the thread here:
    // https://bugs.launchpad.net/ubuntu/+source/libpng/+bug/218409
    #include <png.h>
    #include <gif_lib.h>
    #include <tiffio.h>
  }

  /**
   * LibJPEG version
   */
  static PyObject* libjpeg_version() {
    boost::format f("%d (compiled with %d bits depth)");
    f % LIBJPEG_VERSION % BITS_IN_JSAMPLE;
    return Py_BuildValue("s", f.str().c_str());
  }

  /**
   * Libpng version
   */
  static PyObject* libpng_version() {
    return Py_BuildValue("s", PNG_LIBPNG_VER_STRING);
  }

  /**
   * Libtiff version
   */
  static PyObject* libtiff_version() {
    static const std::string beg_str("LIBTIFF, Version ");
    static const size_t beg_len = beg_str.size();
    std::string vtiff(TIFFGetVersion());

    // Remove first part if it starts with "LIBTIFF, Version "
    if(vtiff.compare(0, beg_len, beg_str) == 0)
      vtiff = vtiff.substr(beg_len);

    // Remove multiple (copyright) lines if any
    size_t end_line = vtiff.find("\n");
    if(end_line != std::string::npos)
      vtiff = vtiff.substr(0,end_line);

    return Py_BuildValue("s", vtiff.c_str());
  }

  /**
   * Version of giflib support
   */
  static PyObject* giflib_version() {
  #ifdef GIF_LIB_VERSION
   return Py_BuildValue("s", GIF_LIB_VERSION);
  #else
    boost::format f("%s.%s.%s");
    f % BOOST_PP_STRINGIZE(GIFLIB_MAJOR) % BOOST_PP_STRINGIZE(GIFLIB_MINOR) % BOOST_PP_STRINGIZE(GIFLIB_RELEASE);
    return Py_BuildValue("s", f.str().c_str());
  #endif
  }

#endif // BOB_IMPORT_VERSION

#endif /* BOB_IO_IMAGE_CONFIG_H */
