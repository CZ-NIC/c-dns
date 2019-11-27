#pragma once

#define _DEFAULT_SOURCE

#include <cbor.h>
#include <time.h>
#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "format_specification.h"
#include "dns.h"
#include "block_table.h"
#include "file_preamble.h"
#include "block.h"
#include "hash.h"
#include "interface.h"
#include "timestamp.h"
#include "writer.h"
#include "cdns_encoder.h"

namespace CDNS {
    /* Other */

    typedef enum
    {
        E_SUCCESS = 0,
        E_ERROR   = -1
    } cdns_ret_t;

    /* Functions */

    int cdns_push(CdnsBlock& ctx, const QueryResponse& qr);

    int cdns_serialize_file_preamble(const FilePreamble& ctx, unsigned char** buff, size_t* buff_size);

    int cdns_serialize_block(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size);

    int cdns_serialize_end(CdnsBlock& ctx, unsigned char** buff, size_t* buff_size);

    class CdnsExporter {
        public:

        /**
         * @brief Buffer new DNS record to C-DNS block
         * @param qr New DNS record to buffer
         * @return `true` if DNS record was successfully buffered, `false` otherwise
         */
        bool buffer_qr(const GenericQueryResponse& qr) {
            return m_block.add_question_response_record(qr);
        }

         /**
         * @todo
         * bool buffer_aec(generic_aec& aec);
         * bool buffer_mm(generic_mm& mm);
         */

        /**
         * @todo
         * void write_file_start();
         * void write_file_preamble();
         * void write_block();
         * void open_file();
         * void close_file();
         * void rotate_file(const std::string& new_output_name);
         */

        private:
        FilePreamble m_file_preamble;
        CdnsBlock m_block;
        CdnsEncoder m_encoder;
    };
}
