/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {

    TEST(FilePreambleTest, FilePreambleCTest) {
        FilePreamble fp;

        EXPECT_EQ(fp.m_major_format_version, VERSION_MAJOR);
        EXPECT_EQ(fp.m_minor_format_version, VERSION_MINOR);
        EXPECT_EQ(*fp.m_private_version, VERSION_PRIVATE);
        ASSERT_EQ(fp.m_block_parameters.size(), 1);

        EXPECT_FALSE(fp.m_block_parameters[0].collection_parameters);
        // BlockParameters tested in separate test
    }

    TEST(FilePreambleTest, StorageHintsCTest) {
        StorageHints sh;

        EXPECT_EQ(sh.query_response_hints, DEFAULT_QR_HINTS);
        EXPECT_EQ(sh.query_response_signature_hints, DEFAULT_QR_SIG_HINTS);
        EXPECT_EQ(sh.rr_hints, DEFAULT_RR_HINTS);
        EXPECT_EQ(sh.other_data_hints, DEFAULT_OTHER_DATA_HINTS);
    }

    TEST(FilePreambleTest, StorageParametersCTest) {
        StorageParameters sp;

        EXPECT_EQ(sp.max_block_items, DEFAULT_MAX_BLOCK_ITEMS);
        EXPECT_EQ(sp.ticks_per_second, DEFAULT_TICKS_PER_SECOND);
        // StorageHints tested in separate test
        EXPECT_EQ(sp.opcodes, OpCodesDefault);
        EXPECT_EQ(sp.rr_types, RrTypesDefault);
        EXPECT_FALSE(sp.storage_flags);
        EXPECT_FALSE(sp.client_address_prefix_ipv4);
        EXPECT_FALSE(sp.client_address_prefix_ipv6);
        EXPECT_FALSE(sp.server_address_prefix_ipv4);
        EXPECT_FALSE(sp.server_address_prefix_ipv6);
        EXPECT_FALSE(sp.sampling_method);
        EXPECT_FALSE(sp.anonymization_method);
    }

    TEST(FilePreambleTest, CollectionParametersCTest) {
        CollectionParameters cp;

        EXPECT_FALSE(cp.query_timeout);
        EXPECT_FALSE(cp.skew_timeout);
        EXPECT_FALSE(cp.snaplen);
        EXPECT_FALSE(cp.promisc);
        EXPECT_EQ(cp.interfaces.size(), 0);
        EXPECT_EQ(cp.server_address.size(), 0);
        EXPECT_EQ(cp.vlan_ids.size(), 0);
        EXPECT_FALSE(cp.filter);
        EXPECT_FALSE(cp.generator_id);
        EXPECT_FALSE(cp.host_id);
    }

    TEST(FilePreambleTest, BPSizeTest) {
        FilePreamble fp;
        EXPECT_EQ(fp.block_parameters_size(), 1);
        
        BlockParameters bp;
        std::size_t index = fp.add_block_parameters(bp);
        EXPECT_EQ(fp.block_parameters_size(), 2);
        EXPECT_EQ(index, 1);
    }
}