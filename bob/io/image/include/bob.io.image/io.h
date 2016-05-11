/**
 * @date Wed May 11 12:39:37 MDT 2016
 * @author Manuel Gunther <siebenkopf@googlemail.com>
 *
 * @brief The file provides an easy C++ interface to read and write images
 *
 * Copyright (c) 2016 , Regents of the University of Colorado on behalf of the University of Colorado Colorado Springs.
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BOB_IO_IMAGE_IO_H
#define BOB_IO_IMAGE_IO_H

#include <stdexcept>
#include <string>

#include <boost/shared_ptr.hpp>
#include <blitz/array.h>

/**
 * @brief Array submodule API of the I/O module
 */
namespace bob { namespace io { namespace image {

#ifdef HAVE_LIBJPEG
  template <int N>
  blitz::Array<uint8_t,N> read_jpeg(const std::string& filename);

  template <int N>
  void write_jpeg(const blitz::Array<uint8_t,N>& image, const std::string& filename);
#endif

}}}

#endif /* BOB_IO_IMAGE_IO_H */
