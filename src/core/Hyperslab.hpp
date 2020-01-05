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


//        template<class T>
//        class Hyperslab {
//        public:
//            using ivec=std::vector<size_t>;
//            using shareptr=std::shared_ptr<std::vector<T>>;
//            Hyperslab()=default;
//            template<size_t ndim>
//            Hyperslab(const boost::multi_array_ref<T,ndim> &arr){
//                ndim_=ndim;
//                offset_=ivec(ndim_,0);
//                stride_=ivec(ndim_,1);
//                auto shape=arr.shape();
//                count_=ivec(shape,shape+ndim_);
//
//            }
//            template<size_t len>
//            Hyperslab(const std::array<T,len> &ar){
//                ndim_=1;
//                cdata_=ar.data();
//                offset_=ivec(1,0);
//                stride_=ivec(1,1);
//                count_=ivec(1,len);
//
//            }
//
//            Hyperslab(const std::vector<size_t> &extents){
//                ndim_=extents.size();
//                offset_=ivec(ndim_,0);
//                stride_=ivec(ndim_,1);
//                count_=ivec(&extents[0],&extents[0]+ndim_);
//                size_t nvals=std::accumulate(count_.cbegin(),count_.cend(),0);
//
//                //allocate data
////                data_=std::make_shared<std::vector<T>>(nvals);
//            }
//
//            Hyperslab(const slice & slc){
//                ndim_=1;
//                offset_=ivec(ndim_,slc[0]);
//                count_=ivec(ndim_,slc[1]);
//                stride_=ivec(ndim_,slc[2]);
//
//            }
//
//            Hyperslab(const std::vector<slice> & slcvec){
//                ndim_=slcvec.size();
//                offset_=ivec(ndim_);
//                count_=ivec(ndim_);
//                stride_=ivec(ndim_);
//
//                for(int i=0;i<ndim_;++i){
//                    offset_[i]=slcvec[i][0];
//                    count_[i]=slcvec[i][1];
//                    stride_[i]=slcvec[i][2];
//                }
//
//            }
//
//        size_t num_elements()const{
//            return std::accumulate(count_.cbegin(),count_.cend(),1,std::multiplies<size_t>());
//        }
//
//        const T* data()const{return data_->data();}
//        T* data(){return data_->data();}
//        const ivec & extents()const{return count_;}
//        //alias to be consistent with multi_array terminology
//        const ivec & shape()const{return count_;}
//        const ivec & stride()const{return stride_;}
//        const ivec bytestride()const{
//            ivec bvec=stride();
//            std::for_each(bvec.begin(),bvec.end(),[](size_t & str){
//                str*=sizeof(T);
//            });
//            return bvec;
//        }
//        const ivec & offset()const{return offset_;}
//
//        size_t ndim()const{return ndim_;};
//            shareptr & getshared(){
//                return data_;
//            }
//
//            const shareptr & getshared()const{
//                return data_;
//            }
////        void setdata(T *const ptr){
////                cdata_=ptr;
////        }
//        private:
//
////            void allocate(){
////                //allocates data
////
////                ownsdata=true;
////            }
//            size_t ndim_=0;
////            T *data_ = nullptr;
//            const T* cdata_=nullptr;
//            shareptr data_{};
//            ivec offset_{};
//            ivec count_{};
//            ivec stride_{};
////            std::vector <size_t> map_{};
//        };

        //forward declare needed
        template<class T>
        class HyperSlabRef;

        //forward declare needed
        template<class T>
        class HyperSlabConstRef;

        //forward declare needed
        template<class T>
        class HyperSlab;

        template<class T>
                class HyperSlabBase{
                public:
                    using ivec=std::vector<size_t>;
                    using svec=std::vector<ptrdiff_t>;
                    HyperSlabBase(){}
                    HyperSlabBase(const ivec &extents){
                        ndim_=extents.size();
                        offset_=ivec(ndim_,0);
                        stride_=svec(ndim_,1);
                        count_=ivec(&extents[0],&extents[0]+ndim_);
                    }
                    template<size_t ndim>
                    HyperSlabBase(const boost::multi_array_ref<T,ndim> &arr){
                        ndim_=ndim;
                        offset_=ivec(ndim_,0);
                        stride_=svec(ndim_,1);
                        auto shape=arr.shape();
                        count_=ivec(shape,shape+ndim_);
                    }

                    HyperSlabBase(const slice & slc){
                        ndim_=1;
                        offset_=ivec(ndim_,slc[0]);
                        count_=ivec(ndim_,slc[1]);
                        stride_=svec(ndim_,slc[2]);

                    }

                    HyperSlabBase(const std::vector<slice> & slcvec){
                        ndim_=slcvec.size();
                        offset_=ivec(ndim_);
                        count_=ivec(ndim_);
                        stride_=svec(ndim_);

                        for(int i=0;i<ndim_;++i){
                            offset_[i]=slcvec[i][0];
                            count_[i]=slcvec[i][1];
                            stride_[i]=slcvec[i][2];
                        }

                    }

                    virtual void useData(const HyperSlab<T> & hslab){THROWMETHODEXCEPTION("cannot use data from HyperSlab");}
                    virtual void useData(const HyperSlabRef<T> & hslab){THROWMETHODEXCEPTION("cannot use data from HyperSlabRef");}
                    virtual void useData(const HyperSlabConstRef<T> & hslab){THROWMETHODEXCEPTION("cannot use data from HyperSlabConstRef");}

                    size_t num_elements()const{
                        return std::accumulate(count_.cbegin(),count_.cend(),1,std::multiplies<size_t>());
                    }
                    virtual const T* data()const{return nullptr;}
                    virtual T* data(){return nullptr;}
                    const ivec & extents()const{return count_;}
                    //alias to be consistent with multi_array terminology
                    const ivec & shape()const{return count_;}
                    size_t shape(int i){return count_[i];}
                    const svec & stride()const{return stride_;}
                    const ivec bytestride()const{
                        ivec bvec(stride().begin(),stride().end());
                        std::for_each(bvec.begin(),bvec.end(),[](size_t & str){
                            str*=sizeof(T);
                        });
                        return bvec;
                    }
                    const ivec & offset()const{return offset_;}

                    size_t ndim()const{return ndim_;};
                    void setExtent(const ivec & ext){count_=ext;}
                    void setStride(const svec & str){stride_=str;}
                    void setOffset(const ivec & offset){offset_=offset;}

                    bool sameMapping(const HyperSlabBase & hslab)const{
                        if (ndim_ != hslab.ndim_)return false;

                        for(int i=0;i<ndim_;++i){
                            if ( offset_[i] != hslab.offset_[i] ) return false;
                            if ( count_[i] != hslab.count_[i] ) return false;
                            if ( stride_[i] != hslab.stride_[i] ) return false;
                        }
                        return true;

                    }
                protected:
                    inline size_t sizeAlloc()const{ return std::inner_product(count_.cbegin(),count_.cend(),stride_.begin(),0);}
                    size_t ndim_=0;
                    ivec offset_{};
                    ivec count_{};
                    svec stride_{};
                };


        ///@brief hyperslab object which does not own the data which it points to
        template<class T>
        class HyperSlabRef:public HyperSlabBase<T>{
        public:
            using typename HyperSlabBase<T>::ivec;
            using typename HyperSlabBase<T>::svec;
            template<size_t ndim>
            HyperSlabRef(const boost::multi_array_ref<T,ndim> &arr):HyperSlabBase<T>(arr){
                //assign data
                data_=const_cast<T*>(arr.data());
            }
            template<size_t len>
            HyperSlabRef(const std::array<T,len> &ar){
                ndim_=1;
                data_=ar.data();
                offset_=ivec(1,0);
                stride_=svec(1,1);
                count_=ivec(1,len);

            }

            HyperSlabRef(const slice & slc, T* data=nullptr):HyperSlabBase<T>(slc),data_(data){}
            HyperSlabRef(const std::vector<slice> & slcvec,T* data=nullptr):HyperSlabBase<T>(slcvec),data_(data){}

            const T* data()const override{return data_;}
            T* data()override{return data_;}
            void setData(T* data){
                data_=data;
            }

//            void getDataFrom(const HyperSlab<T> & hslab)override;
            void useData(const HyperSlabRef<T> & hslab)override;
//            void copyData(const HyperSlabConstRef<T> & hslab)override;

        private:
            using HyperSlabBase<T>::ndim_;
            using HyperSlabBase<T>::offset_;
            using HyperSlabBase<T>::stride_;
            using HyperSlabBase<T>::count_;
            using HyperSlabBase<T>::sizeAlloc;

            //internal storage is just a pointer
            T* data_=nullptr;
        };


        ///@brief hyperslab object which does not own the data which it points to
        template<class T>
        class HyperSlabConstRef:public HyperSlabBase<T>{
        public:
            using typename HyperSlabBase<T>::ivec;
            using typename HyperSlabBase<T>::svec;
            template<size_t ndim>
            HyperSlabConstRef(const boost::multi_array_ref<T,ndim> &arr):HyperSlabBase<T>(arr){
                //assign data
                data_=arr.data();
            }
            template<size_t len>
            HyperSlabConstRef(const std::array<T,len> &ar){
                ndim_=1;
                data_=ar.data();
                offset_=ivec(1,0);
                stride_=svec(1,1);
                count_=ivec(1,len);

            }

            HyperSlabConstRef(const slice & slc, T const * data=nullptr):HyperSlabBase<T>(slc),data_(data){}
            HyperSlabConstRef(const std::vector<slice> & slcvec,T const * data=nullptr):HyperSlabBase<T>(slcvec),data_(data){}

            const T* data()const override{return data_;}
            void setData(T * const data){
                data_=data;
            }
        private:
            using HyperSlabBase<T>::ndim_;
            using HyperSlabBase<T>::offset_;
            using HyperSlabBase<T>::stride_;
            using HyperSlabBase<T>::count_;
            using HyperSlabBase<T>::sizeAlloc;

            //internal storage is just a pointer
            const T* data_=nullptr;
        };


        ///@brief hyperslab object with shared_ownership to the underlying data through a shared_ptr
        template<class T>
        class HyperSlab:public HyperSlabBase<T>{
        public:
            using dvec=T[];
            using dshared=std::shared_ptr<dvec>;
            using ivec=typename HyperSlabBase<T>::ivec;
            template<size_t ndim>
            HyperSlab(const boost::multi_array_ref<T,ndim> &arr,dshared data):HyperSlabBase<T>(arr),data_(data){
            }

            HyperSlab(const ivec &extents,bool alloc=true):HyperSlabBase<T>(extents){
                //also allocate data
                if(alloc)allocate();
            }
            HyperSlab(const slice & slc,bool alloc=true):HyperSlabBase<T>(slc){
                if(alloc)allocate();

            }

            HyperSlab(const slice & slc, dshared data):HyperSlabBase<T>(slc),data_(data){

            }

            HyperSlab(const std::vector<slice> & slcvec,bool alloc=true):HyperSlabBase<T>(slcvec){
                if(alloc)allocate();
            }

            HyperSlab(const std::vector<slice> & slcvec, dshared data):HyperSlabBase<T>(slcvec),data_(data){
            }

            void useData(const HyperSlab<T> & hslab)override{
                if (!this->sameMapping(hslab)) THROWINPUTEXCEPTION("hyperslab mappings do not agree");
                *this=hslab;
            }
            void useData(const HyperSlabRef<T> & hslab)override {
                if (!this->sameMapping(hslab)) THROWINPUTEXCEPTION("hyperslab mappings do not agree");

                if (!data_.get())allocate();
                std::copy(hslab.data(), hslab.data() + sizeAlloc(), data_.get());
            }
//
//            void useData(const HyperSlabConstRef<T> & hslab)override {
//                if (!this->sameMapping(hslab)) THROWINPUTEXCEPTION("hyperslab mappings do not agree");
//                if (!data_.get())allocate();
//                std::copy(hslab.data(), hslab.data() + sizeAlloc(), data_.get());
//            }

            const T* data()const override{return data_.get();}
            T* data()override{return data_.get();}
            dshared & shrdptr(){return data_;}
            const dshared & shrdptr()const{return data_;}
        private:
            using HyperSlabBase<T>::ndim_;
            using HyperSlabBase<T>::offset_;
            using HyperSlabBase<T>::stride_;
            using HyperSlabBase<T>::count_;
            using HyperSlabBase<T>::sizeAlloc;
            dshared data_{};
            ///@brief allocate data within this hyperslab
            void allocate(){
                data_=std::make_shared<dvec>(sizeAlloc());
            }
        };

    template<class T>
    void HyperSlabRef<T>::useData(const HyperSlabRef<T> & hslab){
        if (!this->sameMapping(hslab)) THROWINPUTEXCEPTION("hyperslab mappings do not agree");
        if (data_){
            //data is already pointing somewhere -> copy data
            std::copy(hslab.data(), hslab.data() + sizeAlloc(), data_);
        }else{
            //just point to the same memroy
            data_=hslab.data_;
        }
    }


    }
}
#endif //FROMMLE_HYPERSLAB_HPP
