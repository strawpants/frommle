/*! \file
 \brief Holds the base for the  Operator class
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
 */

#include <boost/iostreams/filtering_stream.hpp>
#include <iostream>
#include <sstream>
#ifndef FROMMLE_LINEBUFFER_HPP
#define FROMMLE_LINEBUFFER_HPP

namespace bio=boost::iostreams;

namespace frommle{
    namespace io{
        ///@brief A class which allows reading lines from (gzipped) text files, while keeping part of it in a buffer
        class LineBuffer{
        public:
            explicit LineBuffer(const std::string &filename="-");
            LineBuffer(const std::string &filename,bool gzip);
            ~LineBuffer();
            class iterator {
            public:
                //iterator traits

                using iterator_category = std::forward_iterator_tag;
                using value_type = std::stringstream;
                using difference_type = std::ptrdiff_t;
                using pointer = std::stringstream *;
                using reference = std::stringstream &;
                iterator &operator++();
                bool operator==(const iterator &other) const;
                bool operator!=(const iterator & other) const {return !(*this == other);}
                std::stringstream & operator*() {return currentLine_;}
                iterator(){}
                iterator(std::istream *stream):strbuf(new char [maxchar]),currentLine_(strbuf.get()),stream_(stream){
                    this->operator++();
                }

            private:
                std::streamsize maxchar=10000;
                std::unique_ptr<char[]> strbuf{};
                std::stringstream currentLine_{};
                std::istream * stream_=nullptr;
            };
            iterator begin(){return iterator(inStream);}
            iterator end(){return iterator();}

        private:
            std::string fname_="-";
            std::istream * inStream = { nullptr };
            bool gzip_=false;
            bio::filtering_istream bioin = {};
            std::unique_ptr<std::ifstream> file = { nullptr };
            ///@brief opens a file for reading
            void openfile();

        };

    }


}


#endif //FROMMLE_LINEBUFFER_HPP
