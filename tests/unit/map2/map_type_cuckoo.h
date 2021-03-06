/*
    This file is a part of libcds - Concurrent Data Structures library

    (C) Copyright Maxim Khizhinsky (libcds.dev@gmail.com) 2006-2016

    Source code repo: http://github.com/khizmax/libcds/
    Download: http://sourceforge.net/projects/libcds/files/
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     
*/

#ifndef CDSUNIT_MAP_TYPE_CUCKOO_H
#define CDSUNIT_MAP_TYPE_CUCKOO_H

#include "map2/map_type.h"
#include <cds/container/cuckoo_map.h>
#include "print_cuckoo_stat.h"

namespace map2 {

    template <typename K, typename V, typename Traits>
    class CuckooMap: public cc::CuckooMap< K, V, Traits >
    {
    public:
        typedef cc::CuckooMap< K, V, Traits > base_class;

    public:
        template <typename Config>
        CuckooMap( Config const& cfg )
            : base_class(

                cfg.c_nCuckooInitialSize,
                static_cast<unsigned int>( cfg.c_nCuckooProbesetSize ),
                static_cast<unsigned int>( cfg.c_nCuckooProbesetThreshold )
            )
        {}

        template <typename Q, typename Pred>
        bool erase_with( Q const& key, Pred /*pred*/ )
        {
            return base_class::erase_with( key, typename std::conditional< base_class::c_isSorted, Pred, typename Pred::equal_to>::type() );
        }

        // for testing
        static CDS_CONSTEXPR bool const c_bExtractSupported = false;
        static CDS_CONSTEXPR bool const c_bLoadFactorDepended = false;
        static CDS_CONSTEXPR bool const c_bEraseExactKey = false;
    };

    struct tag_CuckooMap;

    template <typename Key, typename Value>
    struct map_type< tag_CuckooMap, Key, Value >: public map_type_base< Key, Value >
    {
        typedef map_type_base< Key, Value > base_class;
        typedef typename base_class::compare    compare;
        typedef typename base_class::less       less;
        typedef typename base_class::equal_to   equal_to;
        typedef typename base_class::key_hash   key_hash;
        typedef typename base_class::hash       hash;
        typedef typename base_class::hash2      hash2;

        template <typename Traits>
        struct traits_CuckooStripedMap: public Traits
        {
            typedef cc::cuckoo::striping<> mutex_policy;
        };
        template <typename Traits>
        struct traits_CuckooRefinableMap : public Traits
        {
            typedef cc::cuckoo::refinable<> mutex_policy;
        };

        struct traits_CuckooMap_list_unord :
            public cc::cuckoo::make_traits <
                cc::cuckoo::probeset_type< cc::cuckoo::list >
                , co::equal_to< equal_to >
                , co::hash< std::tuple< hash, hash2 > >
            > ::type
        {};
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_unord>> CuckooStripedMap_list_unord;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_unord>> CuckooRefinableMap_list_unord;

        struct traits_CuckooMap_list_unord_stat : public traits_CuckooMap_list_unord
        {
            typedef cc::cuckoo::stat stat;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_unord_stat>> CuckooStripedMap_list_unord_stat;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_unord_stat>> CuckooRefinableMap_list_unord_stat;

        struct traits_CuckooMap_list_unord_storehash : public traits_CuckooMap_list_unord
        {
            static CDS_CONSTEXPR const bool store_hash = true;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_unord_storehash>> CuckooStripedMap_list_unord_storehash;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_unord_storehash>> CuckooRefinableMap_list_unord_storehash;

        struct traits_CuckooMap_list_ord :
            public cc::cuckoo::make_traits <
                cc::cuckoo::probeset_type< cc::cuckoo::list >
                , co::compare< compare >
                , co::hash< std::tuple< hash, hash2 > >
            >::type
        {};
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_ord>> CuckooStripedMap_list_ord;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_ord>> CuckooRefinableMap_list_ord;

        struct traits_CuckooMap_list_ord_stat : public traits_CuckooMap_list_ord
        {
            typedef cc::cuckoo::stat stat;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_ord_stat>> CuckooStripedMap_list_ord_stat;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_ord_stat>> CuckooRefinableMap_list_ord_stat;

        struct traits_CuckooMap_list_ord_storehash : public traits_CuckooMap_list_ord
        {
            static CDS_CONSTEXPR const bool store_hash = true;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_list_ord_storehash>> CuckooStripedMap_list_ord_storehash;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_list_ord_storehash>> CuckooRefinableMap_list_ord_storehash;

        struct traits_CuckooMap_vector_unord :
            public cc::cuckoo::make_traits <
                cc::cuckoo::probeset_type< cc::cuckoo::vector<4> >
                , co::equal_to< equal_to >
                , co::hash< std::tuple< hash, hash2 > >
            >::type
        {};
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_unord>> CuckooStripedMap_vector_unord;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_unord>> CuckooRefinableMap_vector_unord;

        struct traits_CuckooMap_vector_unord_stat : public traits_CuckooMap_vector_unord
        {
            typedef cc::cuckoo::stat stat;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_unord_stat>> CuckooStripedMap_vector_unord_stat;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_unord_stat>> CuckooRefinableMap_vector_unord_stat;

        struct traits_CuckooMap_vector_unord_storehash : public traits_CuckooMap_vector_unord
        {
            static CDS_CONSTEXPR const bool store_hash = true;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_unord_storehash>> CuckooStripedMap_vector_unord_storehash;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_unord_storehash>> CuckooRefinableMap_vector_unord_storehash;

        struct traits_CuckooMap_vector_ord :
            public cc::cuckoo::make_traits <
                cc::cuckoo::probeset_type< cc::cuckoo::vector<4> >
                , co::compare< compare >
                , co::hash< std::tuple< hash, hash2 > >
            >::type
        {};
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_ord>> CuckooStripedMap_vector_ord;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_ord>> CuckooRefinableMap_vector_ord;

        struct traits_CuckooMap_vector_ord_stat : public traits_CuckooMap_vector_ord
        {
            typedef cc::cuckoo::stat stat;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_ord_stat>> CuckooStripedMap_vector_ord_stat;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_ord_stat>> CuckooRefinableMap_vector_ord_stat;

        struct traits_CuckooMap_vector_ord_storehash : public traits_CuckooMap_vector_ord
        {
            static CDS_CONSTEXPR const bool store_hash = true;
        };
        typedef CuckooMap< Key, Value, traits_CuckooStripedMap<traits_CuckooMap_vector_ord_storehash>> CuckooStripedMap_vector_ord_storehash;
        typedef CuckooMap< Key, Value, traits_CuckooRefinableMap<traits_CuckooMap_vector_ord_storehash>> CuckooRefinableMap_vector_ord_storehash;

    };

    template <typename K, typename T, typename Traits >
    static inline void print_stat( cc::CuckooMap< K, T, Traits > const& m )
    {
        CPPUNIT_MSG( m.statistics() << m.mutex_policy_statistics() );
    }

    template <typename K, typename V, typename Traits>
    static inline void print_stat( CuckooMap< K, V, Traits > const& m )
    {
        typedef CuckooMap< K, V, Traits > map_type;
        print_stat( static_cast<typename map_type::base_class const&>(m) );
    }

}   // namespace map2

#endif // ifndef CDSUNIT_MAP_TYPE_CUCKOO_H
