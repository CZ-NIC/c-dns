#include "file_preamble.h"
#include "cdns_encoder.h"

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