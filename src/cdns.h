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
#include "writer.h"

namespace CDNS {

    /* Other */

    typedef enum
    {
        E_SUCCESS = 0,
        E_ERROR   = -1
    } cdns_ret_t;

    /* Functions */

    int cdns_push(CdnsBlock &ctx, const QueryResponse &qr);

    int cdns_serialize_file_preamble(const FilePreamble &ctx, unsigned char **buff, size_t *buff_size);

    int cdns_serialize_block(CdnsBlock &ctx, unsigned char **buff, size_t *buff_size);

    int cdns_serialize_end(CdnsBlock &ctx, unsigned char **buff, size_t *buff_size);
}
