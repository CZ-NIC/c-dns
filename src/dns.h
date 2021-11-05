/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief IANA assignments
 * https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
 */
namespace CDNS {

    /**
     * @enum OpCodes
     * @brief All supported DNS OpCodes
     */
    enum class OpCodes : uint8_t
    {
        OP_QUERY = 0,
        OP_IQUERY,
        OP_STATUS,
        OP_NOTIFY = 4,
        OP_UPDATE,
        OP_DSO
    };

    /**
     * @brief Vector with all OpCodes supported by the library, used as default for Storage Parameters
     */
    static const std::vector<OpCodes> OpCodesDefault {
        OpCodes::OP_QUERY,
        OpCodes::OP_IQUERY,
        OpCodes::OP_STATUS,
        OpCodes::OP_NOTIFY,
        OpCodes::OP_UPDATE,
        OpCodes::OP_DSO
    };

    /**
     * @enum RrTypes
     * @brief All supported Resource records types
     */
    enum class RrTypes : uint16_t
    {
        A = 1,
        NS,
        MD,
        MF,
        CNAME,
        SOA,
        MB,
        MG,
        MR,
        NULL_R,
        WKS,
        PTR,
        HINFO,
        MINFO,
        MX,
        TXT,
        RP,
        AFSDB,
        X25,
        ISDN,
        RT,
        NSAP,
        NSAP_PTR,
        SIG,
        KEY,
        PX,
        GPOS,
        AAAA,
        LOC,
        NXT,
        EID,
        NIMLOC,
        SRV,
        ATMA,
        NAPTR,
        KX,
        CERTIFICATE,
        A6,
        DNAME,
        SINK,
        OPT,
        APL,
        DS,
        SSHFP,
        IPSECKEY,
        RRSIG,
        NSEC,
        DNSKEY,
        DHCID,
        NSEC3,
        NSEC3PARAM,
        TLSA,
        SMIMEA,
        HIP = 55,
        NINFO,
        RKEY,
        TALINK,
        CDS,
        CDNSKEY,
        OPENPGPKEY,
        CSYNC,
        ZONEMD,
        SVCB,
        HTTPS,
        SPF = 99,
        UINFO,
        UID,
        GID,
        UNSPEC,
        NID,
        L32,
        L64,
        LP,
        EUI48,
        EUI64,
        TKEY = 249,
        TSIG,
        IXFR,
        AXFR,
        MAILB,
        MAILA,
        TYPE_ANY,
        URI,
        CAA,
        AVC,
        DOA,
        AMTRELAY,
        TA = 32768,
        DLV,

        NSEC3PARAMS = NSEC3PARAM,
        CERT = CERTIFICATE
    };

    static const std::vector<RrTypes> RrTypesDefault {
        RrTypes::A,
        RrTypes::NS,
        RrTypes::MD,
        RrTypes::MF,
        RrTypes::CNAME,
        RrTypes::SOA,
        RrTypes::MB,
        RrTypes::MG,
        RrTypes::MR,
        RrTypes::NULL_R,
        RrTypes::WKS,
        RrTypes::PTR,
        RrTypes::HINFO,
        RrTypes::MINFO,
        RrTypes::MX,
        RrTypes::TXT,
        RrTypes::RP,
        RrTypes::AFSDB,
        RrTypes::X25,
        RrTypes::ISDN,
        RrTypes::RT,
        RrTypes::NSAP,
        RrTypes::NSAP_PTR,
        RrTypes::SIG,
        RrTypes::KEY,
        RrTypes::PX,
        RrTypes::GPOS,
        RrTypes::AAAA,
        RrTypes::LOC,
        RrTypes::NXT,
        RrTypes::EID,
        RrTypes::NIMLOC,
        RrTypes::SRV,
        RrTypes::ATMA,
        RrTypes::NAPTR,
        RrTypes::KX,
        RrTypes::CERTIFICATE,
        RrTypes::A6,
        RrTypes::DNAME,
        RrTypes::SINK,
        RrTypes::OPT,
        RrTypes::APL,
        RrTypes::DS,
        RrTypes::SSHFP,
        RrTypes::IPSECKEY,
        RrTypes::RRSIG,
        RrTypes::NSEC,
        RrTypes::DNSKEY,
        RrTypes::DHCID,
        RrTypes::NSEC3,
        RrTypes::NSEC3PARAM,
        RrTypes::TLSA,
        RrTypes::SMIMEA,
        RrTypes::HIP,
        RrTypes::NINFO,
        RrTypes::RKEY,
        RrTypes::TALINK,
        RrTypes::CDS,
        RrTypes::CDNSKEY,
        RrTypes::OPENPGPKEY,
        RrTypes::CSYNC,
        RrTypes::ZONEMD,
        RrTypes::SVCB,
        RrTypes::HTTPS,
        RrTypes::SPF,
        RrTypes::UINFO,
        RrTypes::UID,
        RrTypes::GID,
        RrTypes::UNSPEC,
        RrTypes::NID,
        RrTypes::L32,
        RrTypes::L64,
        RrTypes::LP,
        RrTypes::EUI48,
        RrTypes::EUI64,
        RrTypes::TKEY,
        RrTypes::TSIG,
        RrTypes::IXFR,
        RrTypes::AXFR,
        RrTypes::MAILB,
        RrTypes::MAILA,
        RrTypes::TYPE_ANY,
        RrTypes::URI,
        RrTypes::CAA,
        RrTypes::AVC,
        RrTypes::DOA,
        RrTypes::AMTRELAY,
        RrTypes::TA,
        RrTypes::DLV
    };
}