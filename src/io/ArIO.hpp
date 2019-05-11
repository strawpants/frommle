//
// Created by roelof on 10.05.19.
//

#include "io/ArItem.hpp"

#ifndef FROMMLE_ARIO_HPP
#define FROMMLE_ARIO_HPP

namespace frommle{
    namespace io{

        //forward declare some goodies here to prevent recursive header lookups
        class InputArchiveBase;
        
        class Group;
        using GrpRef=std::shared_ptr<Group>;
        using GrpIterator=ArItemiterator<Group,InputArchiveBase>;

        class VarItem;
        using VarRef=std::shared_ptr<VarItem>;
        using VarIterator=ArItemiterator<VarItem,Group>;

        class ValueItem;
        using ValueRef=std::shared_ptr<ValueItem>;
        using ValueIterator=ArItemiterator<ValueItem,VarItem>;


    }
}

#endif //FROMMLE_ARIO_HPP
