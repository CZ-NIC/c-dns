/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "file_preamble.h"
#include "cdns_encoder.h"
#include "cdns_decoder.h"

std::size_t CDNS::StorageHints::write(CdnsEncoder& enc)
{
    std::size_t written = 0;

    // Start Storage hints map
    written += enc.write_map_start(get_map_index(CDNS::StorageHintsMapIndex::storage_hints_size));

    // Write Query Response hints
    written += enc.write(get_map_index(CDNS::StorageHintsMapIndex::query_response_hints));
    written += enc.write(query_response_hints);

    // Write Query Respone Signature hints
    written += enc.write(get_map_index(CDNS::StorageHintsMapIndex::query_response_signature_hints));
    written += enc.write(query_response_signature_hints);

    // Write RR hints
    written += enc.write(get_map_index(CDNS::StorageHintsMapIndex::rr_hints));
    written += enc.write(rr_hints);

    // Write Other data hints
    written += enc.write(get_map_index(CDNS::StorageHintsMapIndex::other_data_hints));
    written += enc.write(other_data_hints);

    return written;
}

void CDNS::StorageHints::read(CdnsDecoder& dec)
{
    reset();
    bool is_query_response_hints = false;
    bool is_query_response_signature_hints = false;
    bool is_rr_hints = false;
    bool is_other_data_hints = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(StorageHintsMapIndex::query_response_hints):
                query_response_hints = dec.read_unsigned();
                is_query_response_hints = true;
                break;
            case get_map_index(StorageHintsMapIndex::query_response_signature_hints):
                query_response_signature_hints = dec.read_unsigned();
                is_query_response_signature_hints = true;
                break;
            case get_map_index(StorageHintsMapIndex::rr_hints):
                rr_hints = dec.read_unsigned();
                is_rr_hints = true;
                break;
            case get_map_index(StorageHintsMapIndex::other_data_hints):
                other_data_hints = dec.read_unsigned();
                is_other_data_hints = true;
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_query_response_hints || !is_query_response_signature_hints || !is_rr_hints || !is_other_data_hints)
        throw CdnsDecoderException("StorageHints from input stream missing one of mandatory items");
}

void CDNS::StorageHints::reset()
{
    query_response_hints = DEFAULT_QR_HINTS;
    query_response_signature_hints = DEFAULT_QR_SIG_HINTS;
    rr_hints = DEFAULT_RR_HINTS;
    other_data_hints = DEFAULT_OTHER_DATA_HINTS;
}

std::size_t CDNS::StorageParameters::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = 5 + !!storage_flags + !!client_address_prefix_ipv4 + !!client_address_prefix_ipv6
                         + !!server_address_prefix_ipv4 + !!server_address_prefix_ipv6 + !!sampling_method
                         + !!anonymization_method;

    // Start Storage parameters map
    written += enc.write_map_start(fields);

    // Write Ticks per second
    written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::ticks_per_second));
    written += enc.write(ticks_per_second);

    // Write Max block items
    written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::max_block_items));
    written += enc.write(max_block_items);

    // Write Storage hints
    written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::storage_hints));
    written += storage_hints.write(enc);

    // Write OpCodes
    written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::opcodes));
    written += enc.write_array_start(opcodes.size());
    for (auto& opcode : opcodes) {
        written += enc.write(get_map_index(opcode));
    }

    // Write RR types
    written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::rr_types));
    written += enc.write_array_start(rr_types.size());
    for (auto& rr : rr_types) {
        written += enc.write(get_map_index(rr));
    }

    // Write Storage flags
    if (storage_flags) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::storage_flags));
        written += enc.write(storage_flags.value());
    }

    // Write Client address prefix IPv4
    if (client_address_prefix_ipv4) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::client_address_prefix_ipv4));
        written += enc.write(client_address_prefix_ipv4.value());
    }

    // Write Client address prefix IPv6
    if (client_address_prefix_ipv6) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::client_address_prefix_ipv6));
        written += enc.write(client_address_prefix_ipv6.value());
    }

    // Write Server address prefix IPv4
    if (server_address_prefix_ipv4) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::server_address_prefix_ipv4));
        written += enc.write(server_address_prefix_ipv4.value());
    }

    // Write Server address prefix IPv6
    if (server_address_prefix_ipv6) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::server_address_prefix_ipv6));
        written += enc.write(server_address_prefix_ipv6.value());
    }

    // Write Sampling method
    if (sampling_method) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::sampling_method));
        written += enc.write_textstring(sampling_method.value());
    }

    // Write Anonymization method
    if (anonymization_method) {
        written += enc.write(get_map_index(CDNS::StorageParametersMapIndex::anonymization_method));
        written += enc.write_textstring(anonymization_method.value());
    }

    return written;
}

void CDNS::StorageParameters::read(CdnsDecoder& dec)
{
    reset();
    bool is_ticks_per_second = false;
    bool is_max_block_items = false;
    bool is_storage_hints = false;
    bool is_opcodes = false;
    bool is_rr_types = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(StorageParametersMapIndex::ticks_per_second):
                ticks_per_second = dec.read_unsigned();
                is_ticks_per_second = true;
                break;
            case get_map_index(StorageParametersMapIndex::max_block_items):
                max_block_items = dec.read_unsigned();
                is_max_block_items = true;
                break;
            case get_map_index(StorageParametersMapIndex::storage_hints):
                storage_hints.read(dec);
                is_storage_hints = true;
                break;
            case get_map_index(StorageParametersMapIndex::opcodes):
                opcodes.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    opcodes.push_back(static_cast<OpCodes>(dec.read_unsigned()));
                });
                is_opcodes = true;
                break;
            case get_map_index(StorageParametersMapIndex::rr_types):
                rr_types.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    rr_types.push_back(static_cast<RrTypes>(dec.read_unsigned()));
                });
                is_rr_types = true;
                break;
            case get_map_index(StorageParametersMapIndex::storage_flags):
                storage_flags = static_cast<StorageFlagsMask>(dec.read_unsigned());
                break;
            case get_map_index(StorageParametersMapIndex::client_address_prefix_ipv4):
                client_address_prefix_ipv4 = dec.read_unsigned();
                break;
            case get_map_index(StorageParametersMapIndex::client_address_prefix_ipv6):
                client_address_prefix_ipv6 = dec.read_unsigned();
                break;
            case get_map_index(StorageParametersMapIndex::server_address_prefix_ipv4):
                server_address_prefix_ipv4 = dec.read_unsigned();
                break;
            case get_map_index(StorageParametersMapIndex::server_address_prefix_ipv6):
                server_address_prefix_ipv6 = dec.read_unsigned();
                break;
            case get_map_index(StorageParametersMapIndex::sampling_method):
                sampling_method = dec.read_textstring();
                break;
            case get_map_index(StorageParametersMapIndex::anonymization_method):
                anonymization_method = dec.read_textstring();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_ticks_per_second || !is_max_block_items || !is_storage_hints || !is_opcodes || !is_rr_types)
        throw CdnsDecoderException("StorageParameters from input stream missing one of mandatory items");
}

void CDNS::StorageParameters::reset()
{
    ticks_per_second = DEFAULT_TICKS_PER_SECOND;
    max_block_items = DEFAULT_MAX_BLOCK_ITEMS;
    storage_hints.reset();
    opcodes = OpCodesDefault;
    rr_types = RrTypesDefault;
    storage_flags = boost::none;
    client_address_prefix_ipv4 = boost::none;
    client_address_prefix_ipv6 = boost::none;
    server_address_prefix_ipv4 = boost::none;
    server_address_prefix_ipv6 = boost::none;
    sampling_method = boost::none;
    anonymization_method = boost::none;
}

std::size_t CDNS::CollectionParameters::write(CdnsEncoder& enc)
{
    std::size_t fields = !!query_timeout + !!skew_timeout + !!snaplen + !!promisc + !!interfaces.size()
                         + !!server_address.size() + !!vlan_ids.size() + !!filter + !!generator_id + !!host_id;

    if (fields == 0)
        return 0;

    std::size_t written = 0;

    // Start Collection parameters map
    written += enc.write_map_start(fields);

    // Write Query timeout
    if (query_timeout) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::query_timeout));
        written += enc.write(query_timeout.value());
    }

    // Write Skew timeout
    if (skew_timeout) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::skew_timeout));
        written += enc.write(skew_timeout.value());
    }

    // Write Snaplen
    if (snaplen) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::snaplen));
        written += enc.write(snaplen.value());
    }

    // Write Promisc
    if (promisc) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::promisc));
        written += enc.write(promisc.value());
    }

    // Write Interfaces
    if (!!interfaces.size()) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::interfaces));
        written += enc.write_array_start(interfaces.size());
        for (auto& iface : interfaces) {
            written += enc.write_textstring(iface);
        }
    }

    // Write Server address
    if (!!server_address.size()) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::server_address));
        written += enc.write_array_start(server_address.size());
        for (auto& addr : server_address) {
            written += enc.write_bytestring(addr);
        }
    }

    // Write VLAN IDs
    if (!!vlan_ids.size()) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::vland_ids));
        written += enc.write_array_start(vlan_ids.size());
        for (auto& id : vlan_ids) {
            written += enc.write(id);
        }
    }

    // Write Filter
    if (filter) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::filter));
        written += enc.write_textstring(filter.value());
    }

    // Write Generator ID
    if (generator_id) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::generator_id));
        written += enc.write_textstring(generator_id.value());
    }

    // Write Host ID
    if (host_id) {
        written += enc.write(get_map_index(CDNS::CollectionParametersMapIndex::host_id));
        written += enc.write_textstring(host_id.value());
    }

    return written;
}

void CDNS::CollectionParameters::read(CdnsDecoder& dec)
{
    reset();
    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(CollectionParametersMapIndex::query_timeout):
                query_timeout = dec.read_unsigned();
                break;
            case get_map_index(CollectionParametersMapIndex::skew_timeout):
                skew_timeout = dec.read_unsigned();
                break;
            case get_map_index(CollectionParametersMapIndex::snaplen):
                snaplen = dec.read_unsigned();
                break;
            case get_map_index(CollectionParametersMapIndex::promisc):
                promisc = dec.read_bool();
                break;
            case get_map_index(CollectionParametersMapIndex::interfaces):
                interfaces.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    interfaces.push_back(dec.read_textstring());
                });
                break;
            case get_map_index(CollectionParametersMapIndex::server_address):
                server_address.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    server_address.push_back(dec.read_bytestring());
                });
                break;
            case get_map_index(CollectionParametersMapIndex::vland_ids):
                vlan_ids.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    vlan_ids.push_back(dec.read_unsigned());
                });
                break;
            case get_map_index(CollectionParametersMapIndex::filter):
                filter = dec.read_textstring();
                break;
            case get_map_index(CollectionParametersMapIndex::generator_id):
                generator_id = dec.read_textstring();
                break;
            case get_map_index(CollectionParametersMapIndex::host_id):
                host_id = dec.read_textstring();
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }
}

void CDNS::CollectionParameters::reset()
{
    query_timeout = boost::none;
    skew_timeout = boost::none;
    snaplen = boost::none;
    promisc = boost::none;
    interfaces.clear();
    server_address.clear();
    vlan_ids.clear();
    filter = boost::none;
    generator_id = boost::none;
    host_id = boost::none;
}

std::size_t CDNS::BlockParameters::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = collection_parameters ? 2 : 1;

    // Start Block parameters map
    written += enc.write_map_start(fields);

    // Write Storage parameters
    written += enc.write(get_map_index(CDNS::BlockParametersMapIndex::storage_parameters));
    written += storage_parameters.write(enc);

    // Write Collection parameters
    if (collection_parameters) {
        written += enc.write(get_map_index(CDNS::BlockParametersMapIndex::collection_parameters));
        written += collection_parameters->write(enc);
    }

    return written;
}

void CDNS::BlockParameters::read(CdnsDecoder& dec)
{
    reset();
    bool is_storage_parameters = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(BlockParametersMapIndex::storage_parameters):
                storage_parameters.read(dec);
                is_storage_parameters = true;
                break;
            case get_map_index(BlockParametersMapIndex::collection_parameters):
                collection_parameters = CollectionParameters();
                collection_parameters->read(dec);
                break;
            default:
                dec.skip_item();
                break;
        }

        length--;
    }

    if (!is_storage_parameters)
        throw CdnsDecoderException("BlockParameters from input stream missing one of mandatory items");
}

void CDNS::BlockParameters::reset()
{
    storage_parameters.reset();
    collection_parameters = boost::none;
}

std::size_t CDNS::FilePreamble::write(CdnsEncoder& enc)
{
    std::size_t written = 0;
    std::size_t fields = m_private_version ? 4 : 3;

    // Start File preamble map
    written += enc.write_map_start(fields);

    // Write Major format version
    written += enc.write(get_map_index(CDNS::FilePreambleMapIndex::major_format_version));
    written += enc.write(m_major_format_version);

    // Write Minor format version
    written += enc.write(get_map_index(CDNS::FilePreambleMapIndex::minor_format_version));
    written += enc.write(m_minor_format_version);

    // Write Private format version
    if (m_private_version) {
        written += enc.write(get_map_index(CDNS::FilePreambleMapIndex::private_version));
        written += enc.write(m_private_version.value());
    }

    // Write Block parameters
    written += enc.write(get_map_index(CDNS::FilePreambleMapIndex::block_parameters));
    written += enc.write_array_start(m_block_parameters.size());
    for (auto& bp : m_block_parameters) {
        written += bp.write(enc);
    }

    return written;
}

void CDNS::FilePreamble::read(CdnsDecoder& dec)
{
    reset();
    bool is_m_major_format_version = false;
    bool is_m_minor_format_version = false;
    bool is_m_block_parameters = false;

    bool indef = false;
    uint64_t length = dec.read_map_start(indef);

    while (length > 0 || indef) {
        if (indef && dec.peek_type() == CborType::BREAK) {
            dec.read_break();
            break;
        }

        switch (dec.read_integer()) {
            case get_map_index(FilePreambleMapIndex::major_format_version):
                m_major_format_version = dec.read_unsigned();
                is_m_major_format_version = true;
                break;
            case get_map_index(FilePreambleMapIndex::minor_format_version):
                m_minor_format_version = dec.read_unsigned();
                is_m_minor_format_version = true;
                break;
            case get_map_index(FilePreambleMapIndex::private_version):
                m_private_version = dec.read_unsigned();
                break;
            case get_map_index(FilePreambleMapIndex::block_parameters):
                m_block_parameters.clear();
                dec.read_array([this](CdnsDecoder& dec){
                    BlockParameters tmp;
                    tmp.read(dec);
                    m_block_parameters.push_back(tmp);
                });
                is_m_block_parameters = true;
                break;
            default:
                dec.skip_item();
                break;
        }

        length --;
    }

    if (!is_m_major_format_version || !is_m_minor_format_version || !is_m_block_parameters ||
        m_block_parameters.size() == 0)
        throw CdnsDecoderException("FilePreamble from input stream missing one of mandatory items");
}

void CDNS::FilePreamble::reset()
{
    m_major_format_version = VERSION_MAJOR;
    m_minor_format_version = VERSION_MINOR;
    m_private_version = boost::none;
    m_block_parameters = {BlockParameters()};
}