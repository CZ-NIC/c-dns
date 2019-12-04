#pragma once

#include <stdint.h>
#include "gtest/gtest.h"

#include "../src/cdns.h"

namespace CDNS {
    TEST(HashTest, HTest) {
        StringItem si;
        si.data = "Test";
        CDNS::hash<std::string> hash_func;
        std::size_t hash = hash_func(si.data);
        std::size_t hash2 = hash_func(si.data);

        EXPECT_TRUE(hash <= UINT32_MAX);
        EXPECT_TRUE(hash2 <= UINT32_MAX);
        EXPECT_EQ(hash, hash2);

        si.data = "Test2";
        std::size_t hash3 = hash_func(si.data);
        EXPECT_TRUE(hash3 <= UINT32_MAX);
        EXPECT_NE(hash, hash3);
    }

    TEST(HashTest, HClassTypeTest) {
        ClassType ct, ct2, ct3;
        CDNS::hash<ClassType> hash_func;
        ct2.type = 3;

        std::size_t hash = hash_func(ct.key());
        std::size_t hash_dup = hash_func(ct.key());
        std::size_t hash2 = hash_func(ct2.key());
        std::size_t hash3 = hash_func(ct3.key());
        ct.class_ = 2;
        std::size_t hash4 = hash_func(ct.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HQueryResponseSignatureTest) {
        QueryResponseSignature qrs, qrs2, qrs3;
        CDNS::hash<QueryResponseSignature> hash_func;
        qrs2.server_port = 53;
        qrs2.response_rcode = 3;

        std::size_t hash = hash_func(qrs.key());
        std::size_t hash_dup = hash_func(qrs.key());
        std::size_t hash2 = hash_func(qrs2.key());
        std::size_t hash3 = hash_func(qrs3.key());
        qrs.query_qdcount = 3;
        std::size_t hash4 = hash_func(qrs.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HQuestionTest) {
        Question q, q2, q3;
        CDNS::hash<Question> hash_func;
        q2.classtype_index = 3;

        std::size_t hash = hash_func(q.key());
        std::size_t hash_dup = hash_func(q.key());
        std::size_t hash2 = hash_func(q2.key());
        std::size_t hash3 = hash_func(q3.key());
        q.classtype_index = 3;
        std::size_t hash4 = hash_func(q.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HRRTest) {
        RR rr, rr2, rr3;
        CDNS::hash<RR> hash_func;
        rr2.name_index = 3;
        rr2.ttl = 233;

        std::size_t hash = hash_func(rr.key());
        std::size_t hash_dup = hash_func(rr.key());
        std::size_t hash2 = hash_func(rr2.key());
        std::size_t hash3 = hash_func(rr3.key());
        rr.rdata_index = 3;
        std::size_t hash4 = hash_func(rr.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HMalformedMessageDataTest) {
        MalformedMessageData mmd, mmd2, mmd3;
        CDNS::hash<MalformedMessageData> hash_func;
        mmd2.server_port = 53;

        std::size_t hash = hash_func(mmd.key());
        std::size_t hash_dup = hash_func(mmd.key());
        std::size_t hash2 = hash_func(mmd2.key());
        std::size_t hash3 = hash_func(mmd3.key());
        mmd.server_address_index = 3;
        std::size_t hash4 = hash_func(mmd.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HAddressEventCountTest) {
        AddressEventCount aec, aec2, aec3;
        CDNS::hash<AddressEventCount> hash_func;
        aec2.ae_code = 3;

        std::size_t hash = hash_func(aec.key());
        std::size_t hash_dup = hash_func(aec.key());
        std::size_t hash2 = hash_func(aec2.key());
        std::size_t hash3 = hash_func(aec3.key());
        aec.ae_address_index = 3;
        std::size_t hash4 = hash_func(aec.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_NE(hash, hash2);
        EXPECT_EQ(hash, hash3);
        EXPECT_NE(hash, hash4);
    }

    TEST(HashTest, HStringItemTest) {
        std::string ip = "8.8.8.8";
        std::string ip2 = "8.8.8.8";
        std::string ip3 = "192.168.0.1";
        StringItem si, si2, si3, si4;
        CDNS::hash<StringItem> hash_func;
        si.data = ip;
        si2.data = ip2;
        si3.data = ip3;
        si4.data = ip;

        std::size_t hash = hash_func(si.key());
        std::size_t hash_dup = hash_func(si.key());
        std::size_t hash2 = hash_func(si2.key());
        std::size_t hash3 = hash_func(si3.key());
        std::size_t hash4 = hash_func(si4.key());
        si.data = ip3;
        std::size_t hash5 = hash_func(si.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_EQ(hash, hash2);
        EXPECT_NE(hash, hash3);
        EXPECT_EQ(hash, hash4);
        EXPECT_NE(hash, hash5);
        EXPECT_EQ(hash3, hash5);
    }

    TEST(HashTest, HIndexListItemTest) {
        std::vector<index_t> list = {1, 2};
        std::vector<index_t> list2 = {1, 2};
        std::vector<index_t> list3 = {1, 2, 5};
        IndexListItem ili, ili2, ili3, ili4;
        CDNS::hash<IndexListItem> hash_func;
        ili.list = list;
        ili2.list = list2;
        ili3.list = list3;
        ili4.list = list;

        std::size_t hash = hash_func(ili.key());
        std::size_t hash_dup = hash_func(ili.key());
        std::size_t hash2 = hash_func(ili2.key());
        std::size_t hash3 = hash_func(ili3.key());
        std::size_t hash4 = hash_func(ili4.key());
        ili.list.push_back(5);
        std::size_t hash5 = hash_func(ili.key());

        EXPECT_EQ(hash, hash_dup);
        EXPECT_EQ(hash, hash2);
        EXPECT_NE(hash, hash3);
        EXPECT_EQ(hash, hash4);
        EXPECT_NE(hash, hash5);
        EXPECT_EQ(hash3, hash5);
    }
}
