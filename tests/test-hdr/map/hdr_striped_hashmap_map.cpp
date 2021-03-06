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

#include "map/hdr_striped_map.h"
#include <cds/container/striped_map/std_map.h>
#include <cds/container/striped_map.h>
#include <cds/sync/spinlock.h>

namespace map {

    namespace {
        typedef std::map<StripedMapHdrTest::key_type, StripedMapHdrTest::value_type, StripedMapHdrTest::less > map_t;

        struct my_copy_policy {
            typedef map_t::iterator iterator;

            void operator()( map_t& m, iterator /*itInsert*/, iterator itWhat )
            {
                m.insert( *itWhat );
            }
        };
    }

    void StripedMapHdrTest::Striped_map()
    {
        CPPUNIT_MESSAGE( "cmp");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::compare< cmp >
            ,co::mutex_policy< cc::striped_set::striping<> >
        >   map_cmp;
        test_striped< map_cmp >();

        CPPUNIT_MESSAGE( "less");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
        >   map_less;
        test_striped< map_less >();

        CPPUNIT_MESSAGE( "cmpmix");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::compare< cmp >
            , co::less< less >
        >   map_cmpmix;
        test_striped< map_cmpmix >();

        // Spinlock as lock policy
        CPPUNIT_MESSAGE( "spinlock");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::mutex_policy< cc::striped_set::striping<cds::sync::spin> >
        >   map_spin;
        test_striped< map_spin >();

        // Resizing policy
        CPPUNIT_MESSAGE( "load_factor_resizing<0>(1024)");
        {
            typedef cc::StripedMap< map_t
                , co::hash< hash_int >
                , co::less< less >
                , co::resizing_policy< cc::striped_set::load_factor_resizing<0> >
            >   map_less_resizing_lf;
            map_less_resizing_lf m(30, cc::striped_set::load_factor_resizing<0>(1024));
            test_striped_with(m);
        }

        CPPUNIT_MESSAGE( "load_factor_resizing<256>");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
        >   map_less_resizing_lf16;
        test_striped< map_less_resizing_lf16 >();

        CPPUNIT_MESSAGE( "single_bucket_size_threshold<0>(1024)");
        {
            typedef cc::StripedMap< map_t
                , co::hash< hash_int >
                , co::compare< cmp >
                , co::resizing_policy< cc::striped_set::single_bucket_size_threshold<0> >
            >   map_less_resizing_sbt;
            map_less_resizing_sbt m(30, cc::striped_set::single_bucket_size_threshold<0>(1024));
            test_striped_with(m);
        }

        CPPUNIT_MESSAGE( "single_bucket_size_threshold<256>");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::single_bucket_size_threshold<256> >
        >   map_less_resizing_sbt16;
        test_striped< map_less_resizing_sbt16 >();

        // Copy policy
        CPPUNIT_MESSAGE( "load_factor_resizing<256>, copy_item");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::copy_item >
        >   set_copy_item;
        test_striped< set_copy_item >();

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, swap_item");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::swap_item >
        >   set_swap_item;
        test_striped< set_swap_item >();

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, move_item");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< cc::striped_set::move_item >
        >   set_move_item;
        test_striped< set_move_item >();

        CPPUNIT_MESSAGE( "load_factor_resizing<256>, special copy_policy");
        typedef cc::StripedMap< map_t
            , co::hash< hash_int >
            , co::less< less >
            , co::resizing_policy< cc::striped_set::load_factor_resizing<256> >
            , co::copy_policy< my_copy_policy >
        >   set_special_copy_item;
        test_striped< set_special_copy_item >();
    }

}   // namespace map
