/*! \file
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
 */


#include <boost/multi_array.hpp>
#include "core/Exceptions.hpp"
#ifndef FROMMLE_HYPERSLAB_HPP
#define FROMMLE_HYPERSLAB_HPP
namespace frommle{
    namespace core {

        using slice=std::array<ptrdiff_t,3>;
        slice make_slice(ptrdiff_t start,ptrdiff_t stop, ptrdiff_t stride);


        template<class T>
        class Hyperslab {
        public:
            using ivec=std::vector<size_t>;
            using shareptr=std::shared_ptr<std::vector<T>>;
            Hyperslab()=default;
            template<size_t ndim>
            Hyperslab(const boost::multi_array_ref<T,ndim> &arr){
                ndim_=ndim;
                offset_=ivec(ndim_,0);
                stride_=ivec(ndim_,1);
                auto shape=arr.shape();
                count_=ivec(shape,shape+ndim_);

            }
            template<size_t len>
            Hyperslab(const std::array<T,len> &ar){
                ndim_=1;
                cdata_=ar.data();
                offset_=ivec(1,0);
                stride_=ivec(1,1);
                count_=ivec(1,len);

            }

            Hyperslab(const std::vector<size_t> &extents){
                ndim_=extents.size();
                offset_=ivec(ndim_,0);
                stride_=ivec(ndim_,1);
                count_=ivec(&extents[0],&extents[0]+ndim_);
                size_t nvals=std::accumulate(count_.cbegin(),count_.cend(),0);

                //allocate data
//                data_=std::make_shared<std::vector<T>>(nvals);
            }

            Hyperslab(const slice & slc){
                ndim_=1;
                offset_=ivec(ndim_,slc[0]);
                count_=ivec(ndim_,slc[1]);
                stride_=ivec(ndim_,slc[2]);

            }

            Hyperslab(const std::vector<slice> & slcvec){
                ndim_=slcvec.size();
                offset_=ivec(ndim_);
                count_=ivec(ndim_);
                stride_=ivec(ndim_);

                for(int i=0;i<ndim_;++i){
                    offset_[i]=slcvec[i][0];
                    count_[i]=slcvec[i][1];
                    stride_[i]=slcvec[i][2];
                }

            }

        size_t num_elements()const{
            return std::accumulate(count_.cbegin(),count_.cend(),1,std::multiplies<size_t>());
        }

        const T* data()const{return data_->data();}
        T* data(){return data_->data();}
        const ivec & extents()const{return count_;}
        //alias to be consistent with multi_array terminology
        const ivec & shape()const{return count_;}
        const ivec & stride()const{return stride_;}
        const ivec bytestride()const{
            ivec bvec=stride();
            std::for_each(bvec.begin(),bvec.end(),[](size_t & str){
                str*=sizeof(T);
            });
            return bvec;
        }
        const ivec & offset()const{return offset_;}

        size_t ndim()const{return ndim_;};
            shareptr & getshared(){
                return data_;
            }

            const shareptr & getshared()const{
                return data_;
            }
//        void setdata(T *const ptr){
//                cdata_=ptr;
//        }
        private:

//            void allocate(){
//                //allocates data
//
//                ownsdata=true;
//            }
            size_t ndim_=0;
//            T *data_ = nullptr;
            const T* cdata_=nullptr;
            shareptr data_{};
            ivec offset_{};
            ivec count_{};
            ivec stride_{};
//            std::vector <size_t> map_{};
        };

        template<class T>
                class HyperSlabBase{
                public:
                    using ivec=std::vector<size_t>;
                    HyperSlabBase(const std::vector<size_t> &extents){
                        ndim_=extents.size();
                        offset_=ivec(ndim_,0);
                        stride_=ivec(ndim_,1);
                        count_=ivec(&extents[0],&extents[0]+ndim_);
                    }

                    HyperSlabBase(const slice & slc){
                        ndim_=1;
                        offset_=ivec(ndim_,slc[0]);
                        count_=ivec(ndim_,slc[1]);
                        stride_=ivec(ndim_,slc[2]);

                    }

                    HyperSlabBase(const std::vector<slice> & slcvec){
                        ndim_=slcvec.size();
                        offset_=ivec(ndim_);
                        count_=ivec(ndim_);
                        stride_=ivec(ndim_);

                        for(int i=0;i<ndim_;++i){
                            offset_[i]=slcvec[i][0];
                            count_[i]=slcvec[i][1];
                            stride_[i]=slcvec[i][2];
                        }

                    }


                    size_t num_elements()const{
                        return std::accumulate(count_.cbegin(),count_.cend(),1,std::multiplies<size_t>());
                    }

                    virtual const T* data()const=0;
                    virtual T* data()=0;
                    const ivec & extents()const{return count_;}
                    //alias to be consistent with multi_array terminology
                    const ivec & shape()const{return count_;}
                    const ivec & stride()const{return stride_;}
                    const ivec bytestride()const{
                        ivec bvec=stride();
                        std::for_each(bvec.begin(),bvec.end(),[](size_t & str){
                            str*=sizeof(T);
                        });
                        return bvec;
                    }
                    const ivec & offset()const{return offset_;}

                    size_t ndim()const{return ndim_;};
                protected:
                    size_t ndim_=0;
                    ivec offset_{};
                    ivec count_{};
                    ivec stride_{};
                };


        ///@brief hyperslab object which does not own the data which it points to
        template<class T>
        class HyperSlabRef:public HyperSlabBase<T>{
        public:

            template<size_t ndim>
            HyperSlabRef(const boost::multi_array_ref<T,ndim> &arr){
                ndim_=ndim;
                offset_=ivec(ndim_,0);
                stride_=ivec(ndim_,1);
                auto shape=arr.shape();
                count_=ivec(shape,shape+ndim_);
                data_=ar.data();
            }
            template<size_t len>
            HyperSlabRef(const std::array<T,len> &ar){
                ndim_=1;
                data_=ar.data();
                offset_=ivec(1,0);
                stride_=ivec(1,1);
                count_=ivec(1,len);

            }

            const T* data()const override{return data_;}
            T* data()override{return data_;}

        private:
            //internal storage is just a pointer
            const T* data_=nullptr;
        };

        ///@brief hyperslab object with shared_ownership to the underlying data through a shared_ptr
        template<class T>
        class HyperSlab:public HyperSlabBase<T>{
        public:
            using dvec=std::vector<T>;
            const T* data()const override{return data_->data();}
            T* data()override{return data_->data();}
            std::shared_ptr<dvec> & getshared(){return data_;}
            const std::shared_ptr<dvec> & getshared()const{return data_;}
        private:
            std::shared_ptr<dvec> data_{};
            ///@brief allocate data within this hyperslab
            void allocate(){
                size_t sz=std::inner_product(count_.cbegin(),count_.cend(),stride_.begin(),0);
                data_=std::make_shared<std::vector<T>>(sz);
            }
        };
    }
}
#endif //FROMMLE_HYPERSLAB_HPP
