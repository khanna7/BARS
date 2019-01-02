/*
*MRSA Model
*
*Copyright (c) 2013 University of Chicago and Argonne National Laboratory
*   All rights reserved.
*  
*   Redistribution and use in source and binary forms, with 
*   or without modification, are permitted provided that the following 
*   conditions are met:
*  
*  	 Redistributions of source code must retain the above copyright notice,
*  	 this list of conditions and the following disclaimer.
*  
*  	 Redistributions in binary form must reproduce the above copyright notice,
*  	 this list of conditions and the following disclaimer in the documentation
*  	 and/or other materials provided with the distribution.
*  
*  	 Neither the name of the Argonne National Laboratory nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*  
*   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE TRUSTEES OR
*   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
*   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * FileOutput.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: nick
 */

#include <boost/filesystem.hpp>
#include <sstream>

#include "FileOutput.h"
#include "file_utils.h"


namespace fs = boost::filesystem;

namespace TransModel {

FileOutput::FileOutput(const std::string& filename) :
        out(), open(true) {

    std::string fname = unique_file_name(filename);
    fs::path filepath(fname);
    if (!fs::exists(filepath.parent_path()))
        fs::create_directories(filepath.parent_path());
    out.open(filepath.string().c_str());
}


void FileOutput::close() {
    if (open) {
        out.flush();
        out.close();
        open = false;
    }
}

FileOutput::~FileOutput() {
    close();
}

std::ostream& FileOutput::ostream() {
    if (!open) {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

std::ostream& FileOutput::operator<<(const std::string& val) {
    if (open) {
        out << val;
    } else {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

std::ostream& FileOutput::operator<<(unsigned int val) {
    if (open) {
        out << val;
    } else {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

std::ostream& FileOutput::operator<<(int val) {
    if (open) {
        out << val;
    } else {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

std::ostream& FileOutput::operator<<(double val) {
    if (open) {
        out << val;
    } else {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

std::ostream& FileOutput::operator<<(float val) {
    if (open) {
        out << val;
    } else {
        throw std::logic_error("Cannot write to closed FileOutput");
    }
    return out;
}

} /* namespace mrsa */
