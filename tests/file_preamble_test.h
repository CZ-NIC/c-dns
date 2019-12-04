#pragma once

#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {

    TEST(FilePreambleTest, FilePreambleCTest) {
        FilePreamble fp;

        EXPECT_EQ(fp.m_major_format_version, 1);
        EXPECT_EQ(fp.m_minor_format_version, 0);
        EXPECT_FALSE(fp.m_private_version);
        ASSERT_EQ(fp.m_block_parameters.size(), 1);

        EXPECT_FALSE(fp.m_block_parameters[0].collection_parameters.has_value());
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
        EXPECT_FALSE(sp.storage_flags.has_value());
        EXPECT_FALSE(sp.client_address_prefix_ipv4.has_value());
        EXPECT_FALSE(sp.client_address_prefix_ipv6.has_value());
        EXPECT_FALSE(sp.server_address_prefix_ipv4.has_value());
        EXPECT_FALSE(sp.server_address_prefix_ipv6.has_value());
        EXPECT_FALSE(sp.sampling_method.has_value());
        EXPECT_FALSE(sp.anonymization_method.has_value());
    }

    TEST(FilePreambleTest, CollectionParametersCTest) {
        CollectionParameters cp;

        EXPECT_FALSE(cp.query_timeout.has_value());
        EXPECT_FALSE(cp.skew_timeout.has_value());
        EXPECT_FALSE(cp.snaplen.has_value());
        EXPECT_FALSE(cp.promisc.has_value());
        EXPECT_EQ(cp.interfaces.size(), 0);
        EXPECT_EQ(cp.server_address.size(), 0);
        EXPECT_EQ(cp.vlan_ids.size(), 0);
        EXPECT_FALSE(cp.filter.has_value());
        EXPECT_FALSE(cp.generator_id.has_value());
        EXPECT_FALSE(cp.host_id.has_value());
    }

    TEST(FilePreambleTest, BPSizeTest) {
        FilePreamble fp;
        EXPECT_EQ(fp.block_parameters_size(), 1);
        
        BlockParameters bp;
        fp.add_block_parameters(bp);
        EXPECT_EQ(fp.block_parameters_size(), 2);
    }
}