/*! \file LineBuffer.cpp
 \brief 
 \copyright Roelof Rietbroek 2019
 \license
 This file is part of Frommle.
 frommle is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 Frommle is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with Frommle; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 Created by Roelof Rietbroek,  2019

 */

#include "io/LineBuffer.hpp"
#include "core/Exceptions.hpp"
#include "LineBuffer.hpp"

#include <boost/iostreams/filter/gzip.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace frommle{
    namespace io {

        void LineBuffer::openfile() {
            if (inStream) {
                return;
            }
            //initialize streams after setting options
            if (fname_ == "-") {
                if (gzip_) {
                    bioin.push(bio::gzip_decompressor());
                    bioin.push(std::cin);
                    inStream = &bioin;

                } else {
                    inStream = &std::cin;
                }
            } else {
                //possibly apply a gzip filter  to the stream

                if (gzip_) {

                    file = std::unique_ptr<std::ifstream>(
                            new std::ifstream(fname_, std::ios_base::in | std::ios_base::binary));
                    if (!file->good()) {
                        THROWIOEXCEPTION("Could not open file");
                    }
                    bioin.push(bio::gzip_decompressor());
                    bioin.push(*file);
                    inStream = &bioin;
                } else {
                    file = std::unique_ptr<std::ifstream>(new std::ifstream(fname_, std::ios_base::in));
                    if (!file->good()) {
                        THROWIOEXCEPTION("Could not open file");
                    }
                    inStream = file.get();
                }
            }
        }

        LineBuffer::LineBuffer(const std::string &filename) :fname_(filename) {
            openfile();
        }

        LineBuffer::LineBuffer(const std::string &filename, bool gzip):fname_(filename),gzip_(gzip){
            openfile();
        }

        LineBuffer::~LineBuffer() {

        }

        LineBuffer::iterator &LineBuffer::iterator::operator++() {
            //reset internal stringstream and put a new line in it
            currentLine_.clear();
            stream_->getline(strbuf.get(), maxchar);
            if(stream_->eof()){
                stream_ = nullptr;
                return *this;
            }else if ( (stream_->rdstate() & std::ios_base::failbit) != 0){
                currentLine_.str({});
                currentLine_ << strbuf.get();
                while ((stream_->rdstate() & std::ios_base::failbit) !=0) {
                    //increase the line buffer size
                    maxchar = maxchar * 2;
                    strbuf = std::unique_ptr<char[]>(new char[maxchar]);
                    //read other stuff from line
                    stream_->clear();
                    stream_->getline(strbuf.get(), maxchar);
                    //append data to string stream
                    currentLine_ << strbuf.get();
                    if (stream_->eof()) {
                        stream_ = nullptr;
                    }
                }
                return *this;
            }else {
                currentLine_.str(strbuf.get());
                return *this;
            }
        }

        bool LineBuffer::iterator::operator==(const LineBuffer::iterator &other) const {
            if (stream_ and other.stream_) {
                return stream_->tellg() == other.stream_->tellg();
            }else if (!stream_ and ! other.stream_){
                return true;
            }else{
                return false;
            }
        }
    }
}
