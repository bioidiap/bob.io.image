/**
 * @date Wed May 25 13:05:58 MDT 2016
 * @author Manuel Gunther <siebenkopf@googlemail.com>
 *
 * @brief The file provides a generic interface to read any kind of images that we support
 *
 * Copyright (c) 2016, Regents of the University of Colorado on behalf of the University of Colorado Colorado Springs.
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BOB_IO_IMAGE_IMAGE_H
#define BOB_IO_IMAGE_IMAGE_H

#include <bob.io.image/bmp.h>
#include <bob.io.image/png.h>
#include <bob.io.image/gif.h>
#include <bob.io.image/jpeg.h>
#include <bob.io.image/netpbm.h>
#include <bob.io.image/tiff.h>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <algorithm>

namespace bob { namespace io { namespace image {

inline const std::string& get_correct_image_extension(const std::string& image_name){
  // global map of known magic numbers
  // these numbers are based on: http://stackoverflow.com/questions/26350342/determining-the-extension-type-of-an-image-file-using-binary/26350431#26350431
  static std::map<std::string, std::vector<std::vector<std::uint8_t>>> known_magic_numbers;

  // initialize list, if not done yet
  if (known_magic_numbers.empty()){
    // BMP
    known_magic_numbers[".bmp"].push_back({ 0x42, 0x4D });
    // NetPBM, see https://en.wikipedia.org/wiki/Netpbm_format
    known_magic_numbers[".pbm"].push_back({ 0x50, 0x31 });// P1
    known_magic_numbers[".pbm"].push_back({ 0x50, 0x34 });// P4
    known_magic_numbers[".pgm"].push_back({ 0x50, 0x32 });// P2
    known_magic_numbers[".pgm"].push_back({ 0x50, 0x35 });// P5
    known_magic_numbers[".ppm"].push_back({ 0x50, 0x33 });// P3
    known_magic_numbers[".ppm"].push_back({ 0x50, 0x36 });// P6
    // TODO: what about P7?

#ifdef HAVE_GIFLIB
    // GIF
    known_magic_numbers[".gif"].push_back({ 0x47, 0x49, 0x46, 0x38, 0x37, 0x61 });
    known_magic_numbers[".gif"].push_back({ 0x47, 0x49, 0x46, 0x38, 0x39, 0x61 });
#endif // HAVE_GIFLIB
#ifdef HAVE_LIBPNG
    // PNG
    known_magic_numbers[".png"].push_back({ 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A });
#endif // HAVE_LIB_PNG
#ifdef HAVE_LIBJPEG
    // JPEG
    known_magic_numbers[".jpg"].push_back({ 0xFF, 0xD8, 0xFF });
    known_magic_numbers[".jpg"].push_back({ 0x00, 0x00, 0x00, 0x0C, 0x6A, 0x50, 0x20, 0x20 });
#endif // HAVE_LIBJPEG
#ifdef HAVE_LIBTIFF
    // TIFF
    known_magic_numbers[".tiff"].push_back({ 0x0C, 0xED });
    known_magic_numbers[".tiff"].push_back({ 0x49, 0x20, 0x49 });
    known_magic_numbers[".tiff"].push_back({ 0x49, 0x49, 0x2A, 0x00 });
    known_magic_numbers[".tiff"].push_back({ 0x4D, 0x4D, 0x00, 0x2A });
    known_magic_numbers[".tiff"].push_back({ 0x4D, 0x4D, 0x00, 0x2B });
#endif // HAVE_LIBTIFF
  }

  // read first 8 bytes from file
  static uint8_t image_bytes[8];
  std::ifstream f(image_name);
  if (!f) throw std::runtime_error("The given image '" + image_name + "' could not be opened for reading");
  f.read(reinterpret_cast<char*>(image_bytes), 8);

  // iterate over all extensions
  for (auto eit = known_magic_numbers.begin(); eit != known_magic_numbers.end(); ++eit){
    // iterate over all magic bytes
    for (auto mit = eit->second.begin(); mit != eit->second.end(); ++mit){
      // check magic number
      if (std::equal(mit->begin(), mit->end(), image_bytes))
        return eit->first;
    }
  }

  throw std::runtime_error("The given image '" + image_name + "' does not contain an image of a known type");
}

inline bool is_color_image(const std::string& filename, std::string extension=""){
  if (extension.empty())
    extension = boost::filesystem::path(filename).extension().string();
  boost::algorithm::to_lower(extension);
  if (extension == ".bmp") return true;
#ifdef HAVE_GIFLIB
  if (extension == ".gif") return true;
#endif
#ifdef HAVE_LIBPNG
  if (extension == ".png") return is_color_png(filename);
#endif
#ifdef HAVE_LIBJPEG
  if (extension == ".jpg" || extension == ".jpeg") return is_color_jpeg(filename);
#endif
#ifdef HAVE_LIBTIFF
  if (extension == ".tif" || extension == ".tiff") return is_color_tiff(filename);
#endif
  if (extension == ".pgm" || extension == ".pbm") return false;
  if (extension == ".ppm") return true;

  throw std::runtime_error("The filename extension '" + extension + "' is not known");
}

inline blitz::Array<uint8_t,3> read_color_image(const std::string& filename, std::string extension=""){
  if (extension.empty())
    extension = boost::filesystem::path(filename).extension().string();
  boost::algorithm::to_lower(extension);
  if (extension == ".bmp") return read_bmp(filename);
#ifdef HAVE_GIFLIB
  if (extension == ".gif") return read_gif(filename);
#endif
#ifdef HAVE_LIBPNG
  if (extension == ".png") return read_png<uint8_t,3>(filename);
#endif
#ifdef HAVE_LIBJPEG
  if (extension == ".jpg" || extension == ".jpeg") return read_jpeg<3>(filename);
#endif
#ifdef HAVE_LIBTIFF
  if (extension == ".tif" || extension == ".tiff") return read_tiff<uint8_t,3>(filename);
#endif
  if (extension == ".ppm") return read_ppm<uint8_t>(filename);

  throw std::runtime_error("The filename extension '" + extension + "' is not known or not supported for color images");
}

blitz::Array<uint8_t,2> read_gray_image(const std::string& filename, std::string extension=""){
  if (extension.empty())
    extension = boost::filesystem::path(filename).extension().string();
  boost::algorithm::to_lower(extension);
#ifdef HAVE_LIBPNG
  if (extension == ".png") return read_png<uint8_t,2>(filename);
#endif
#ifdef HAVE_LIBJPEG
  if (extension == ".jpg" || extension == ".jpeg") return read_jpeg<2>(filename); // this will only work for T=uint8_t
#endif
#ifdef HAVE_LIBTIFF
  if (extension == ".tif" || extension == ".tiff") return read_tiff<uint8_t,2>(filename);
#endif
  if (extension == ".pgm") return read_pgm<uint8_t>(filename);
  if (extension == ".pbm") return read_pbm<uint8_t>(filename);

  throw std::runtime_error("The filename extension '" + extension + "' is not known or not supported for gray images");
}


void write_color_image(const blitz::Array<uint8_t,3>& image, const std::string& filename, std::string extension=""){
  if (extension.empty())
    extension = boost::filesystem::path(filename).extension().string();
  boost::algorithm::to_lower(extension);
  if (extension == ".bmp") return write_bmp(image, filename); // this will only work for T=uint8_t
#ifdef HAVE_GIFLIB
  if (extension == ".gif") return write_gif(image, filename); // this will only work for T=uint8_t
#endif
#ifdef HAVE_LIBPNG
  if (extension == ".png") return write_png(image, filename);
#endif
#ifdef HAVE_LIBJPEG
  if (extension == ".jpg" || extension == ".jpeg") return write_jpeg(image, filename); // this will only work for T=uint8_t
#endif
#ifdef HAVE_LIBTIFF
  if (extension == ".tif" || extension == ".tiff") return write_tiff(image, filename);
#endif
  if (extension == ".ppm") return write_ppm(image, filename);

  throw std::runtime_error("The filename extension '" + extension + "' is not known or not supported for color images");
}

void write_gray_image(const blitz::Array<uint8_t,2>& image, const std::string& filename, std::string extension=""){
  if (extension.empty())
    extension = boost::filesystem::path(filename).extension().string();
  boost::algorithm::to_lower(extension);
#ifdef HAVE_LIBPNG
  if (extension == ".png") return write_png(image, filename);
#endif
#ifdef HAVE_LIBJPEG
  if (extension == ".jpg" || extension == ".jpeg") return write_jpeg(image, filename); // this will only work for T=uint8_t
#endif
#ifdef HAVE_LIBTIFF
  if (extension == ".tif" || extension == ".tiff") return write_tiff(image, filename);
#endif
  if (extension == ".pgm") return write_pgm(image, filename);
  if (extension == ".pbm") return write_pbm(image, filename);

  throw std::runtime_error("The filename extension '" + extension + "' is not known or not supported for gray images");
}

} } } // namespace

#endif // BOB_IO_IMAGE_IMAGE_H
