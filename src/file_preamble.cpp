#include "file_preamble.h"
#include "cdns_encoder.h"

void CDNS::StorageHints::write(CdnsEncoder& enc)
{
    // Start Storage hints map
    enc.write_map_start(get_map_index(CDNS::StorageHintsMapIndex::storage_hints_size));

    // Write Query Response hints
    enc.write(get_map_index(CDNS::StorageHintsMapIndex::query_response_hints));
    enc.write(query_response_hints);

    // Write Query Respone Signature hints
    enc.write(get_map_index(CDNS::StorageHintsMapIndex::query_response_signature_hints));
    enc.write(query_response_signature_hints);

    // Write RR hints
    enc.write(get_map_index(CDNS::StorageHintsMapIndex::rr_hints));
    enc.write(rr_hints);

    // Write Other data hints
    enc.write(get_map_index(CDNS::StorageHintsMapIndex::other_data_hints));
    enc.write(other_data_hints);
}

void CDNS::StorageParameters::write(CdnsEncoder& enc)
{
    std::size_t fields = 5 + !!storage_flags + !!client_address_prefix_ipv4 + !!client_address_prefix_ipv6
                         + !!server_address_prefix_ipv4 + !!server_address_prefix_ipv6 + !!sampling_method
                         + !!anonymization_method;

    // Start Storage parameters map
    enc.write_map_start(fields);

    // Write Ticks per second
    enc.write(get_map_index(CDNS::StorageParametersMapIndex::ticks_per_second));
    enc.write(ticks_per_second);

    // Write Max block items
    enc.write(get_map_index(CDNS::StorageParametersMapIndex::max_block_items));
    enc.write(max_block_items);

    // Write Storage hints
    enc.write(get_map_index(CDNS::StorageParametersMapIndex::storage_hints));
    storage_hints.write(enc);

    // Write OpCodes
    enc.write(get_map_index(CDNS::StorageParametersMapIndex::opcodes));
    enc.write_array_start(opcodes.size());
    for (auto& opcode : opcodes) {
        enc.write(get_map_index(opcode));
    }

    // Write RR types
    enc.write(get_map_index(CDNS::StorageParametersMapIndex::rr_types));
    enc.write_array_start(rr_types.size());
    for (auto& rr : rr_types) {
        enc.write(get_map_index(rr));
    }

    // Write Storage flags
    if (storage_flags) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::storage_flags));
        enc.write(storage_flags.value());
    }

    // Write Client address prefix IPv4
    if (client_address_prefix_ipv4) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::client_address_prefix_ipv4));
        enc.write(client_address_prefix_ipv4.value());
    }

    // Write Client address prefix IPv6
    if (client_address_prefix_ipv6) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::client_address_prefix_ipv6));
        enc.write(client_address_prefix_ipv6.value());
    }

    // Write Server address prefix IPv4
    if (server_address_prefix_ipv4) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::server_address_prefix_ipv4));
        enc.write(server_address_prefix_ipv4.value());
    }

    // Write Server address prefix IPv6
    if (server_address_prefix_ipv6) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::server_address_prefix_ipv6));
        enc.write(server_address_prefix_ipv6.value());
    }

    // Write Sampling method
    if (sampling_method) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::sampling_method));
        enc.write_textstring(sampling_method.value());
    }

    // Write Anonymization method
    if (anonymization_method) {
        enc.write(get_map_index(CDNS::StorageParametersMapIndex::anonymization_method));
        enc.write_textstring(anonymization_method.value());
    }
}

void CDNS::CollectionParameters::write(CdnsEncoder& enc)
{
    std::size_t fields = !!query_timeout + !!skew_timeout + !!snaplen + !!promisc + !!interfaces.size()
                         + !!server_address.size() + !!vlan_ids.size() + !!filter + !!generator_id + !!host_id;

    if (fields == 0)
        return;

    // Start Collection parameters map
    enc.write_map_start(fields);

    // Write Query timeout
    if (query_timeout) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::query_timeout));
        enc.write(query_timeout.value());
    }

    // Write Skew timeout
    if (skew_timeout) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::skew_timeout));
        enc.write(skew_timeout.value());
    }

    // Write Snaplen
    if (snaplen) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::snaplen));
        enc.write(snaplen.value());
    }

    // Write Promisc
    if (promisc) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::promisc));
        enc.write(promisc.value());
    }

    // Write Interfaces
    if (!!interfaces.size()) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::interfaces));
        enc.write_array_start(interfaces.size());
        for (auto& iface : interfaces) {
            enc.write_textstring(iface);
        }
    }

    // Write Server address
    if (!!server_address.size()) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::server_address));
        enc.write_array_start(server_address.size());
        for (auto& addr : server_address) {
            enc.write_bytestring(addr);
        }
    }

    // Write VLAN IDs
    if (!!vlan_ids.size()) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::vland_ids));
        enc.write_array_start(vlan_ids.size());
        for (auto& id : vlan_ids) {
            enc.write(id);
        }
    }

    // Write Filter
    if (filter) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::filter));
        enc.write_textstring(filter.value());
    }

    // Write Generator ID
    if (generator_id) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::generator_id));
        enc.write_textstring(generator_id.value());
    }

    // Write Host ID
    if (host_id) {
        enc.write(get_map_index(CDNS::CollectionParametersMapIndex::host_id));
        enc.write_textstring(host_id.value());
    }
}

void CDNS::BlockParameters::write(CdnsEncoder& enc)
{
    std::size_t fields = collection_parameters ? 2 : 1;

    // Start Block parameters map
    enc.write_map_start(fields);

    // Write Storage parameters
    enc.write(get_map_index(CDNS::BlockParametersMapIndex::storage_parameters));
    storage_parameters.write(enc);

    // Write Collection parameters
    if (collection_parameters) {
        enc.write(get_map_index(CDNS::BlockParametersMapIndex::collection_parameters));
        collection_parameters->write(enc);
    }
}

void CDNS::FilePreamble::write(CdnsEncoder& enc)
{
    std::size_t fields = m_private_version ? 4 : 3;

    // Start File preamble map
    enc.write_map_start(fields);

    // Write Major format version
    enc.write(get_map_index(CDNS::FilePreambleMapIndex::major_format_version));
    enc.write(m_major_format_version);

    // Write Minor format version
    enc.write(get_map_index(CDNS::FilePreambleMapIndex::minor_format_version));
    enc.write(m_minor_format_version);

    // Write Private format version
    if (m_private_version) {
        enc.write(get_map_index(CDNS::FilePreambleMapIndex::private_version));
        enc.write(m_private_version.value());
    }

    // Write Block parameters
    enc.write(get_map_index(CDNS::FilePreambleMapIndex::block_parameters));
    enc.write_array_start(m_block_parameters.size());
    for (auto& bp : m_block_parameters) {
        bp.write(enc);
    }
}