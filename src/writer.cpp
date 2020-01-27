/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>

#include "writer.h"

void CDNS::GzipCborOutputWriter::write(const char* p, std::size_t size)
{
    m_gzip.next_in = reinterpret_cast<const unsigned char*>(p);
    m_gzip.avail_in = size;

    // Loop until all input data is compressed and written to output
    while (m_gzip.avail_in > 0) {
        write_gzip(size, Z_NO_FLUSH);
    }
}

void CDNS::GzipCborOutputWriter::open()
{
    // Initialize GZIP stream
    m_gzip.zalloc = Z_NULL;
    m_gzip.zfree = Z_NULL;
    m_gzip.opaque = Z_NULL;
    int ret = deflateInit2(&m_gzip, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK)
        throw CborOutputException("Couldn't initialize GZIP compression");
}

void CDNS::GzipCborOutputWriter::close()
{
    try {
        if (m_gzip.state) {
            // Finish compression of all remaining data and close the GZIP stream
            while (write_gzip(2048, Z_FINISH) != Z_STREAM_END);
            deflateEnd(&m_gzip);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int CDNS::GzipCborOutputWriter::write_gzip(std::size_t in_size, int action)
{
    std::size_t size = in_size + in_size / 3 + 128;
    uint8_t buff[size];

    // Set output buffer
    m_gzip.next_out = buff;
    m_gzip.avail_out = size;

    // Compress data to output
    int ret = deflate(&m_gzip, action);
    if (ret == Z_OK || ret == Z_STREAM_END)
        m_writer->write(reinterpret_cast<const char*>(buff), sizeof(buff) - m_gzip.avail_out);
    else
        throw CborOutputException("Couldn't write to output file!");

    return ret;
}

void CDNS::XzCborOutputWriter::write(const char* p, std::size_t size)
{
    m_lzma.next_in = reinterpret_cast<const uint8_t*>(p);
    m_lzma.avail_in = size;

    // Loop until all input data is compressed and written to output
    while (m_lzma.avail_in > 0) {
        write_lzma(size, LZMA_RUN);
    }
}

void CDNS::XzCborOutputWriter::open()
{
    // Initialize LZMA stream
    m_lzma = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_easy_encoder(&m_lzma, 6 /* XZ utils default */, LZMA_CHECK_CRC64);
    if (ret != LZMA_OK)
        throw CborOutputException("Couldn't initialize LZMA compression!");
}

void CDNS::XzCborOutputWriter::close()
{
    try {
        if (m_lzma.internal) {
            // Finish compression of all remaining data and close the LZMA stream
            while (write_lzma(2048, LZMA_FINISH) != LZMA_STREAM_END);
            lzma_end(&m_lzma);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

lzma_ret CDNS::XzCborOutputWriter::write_lzma(std::size_t in_size, lzma_action action)
{
    std::size_t size = in_size + in_size / 3 + 128;
    uint8_t buff[size];

    // Set output buffer
    m_lzma.next_out = buff;
    m_lzma.avail_out = size;

    // Compress data to output
    lzma_ret ret = lzma_code(&m_lzma, action);
    if (ret == LZMA_OK || ret == LZMA_STREAM_END)
        m_writer->write(reinterpret_cast<const char*>(buff), sizeof(buff) - m_lzma.avail_out);
    else
        throw CborOutputException("Couldn't write to output file!");

    return ret;
}
