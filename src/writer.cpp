/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cdns_encoder.h"

void CDNS::CborOutputWriter::write(const char* p, std::size_t size)
{
    if (m_is_fd) {
        int ret = ::write(m_fd, p, size);
        if (ret != size) {
            throw CborOutputException("Given " + std::to_string(size) + " bytes to write, but "
                                       "only " + std::to_string(ret) + " bytes were written!");
        }
    }
    else {
        m_out.write(p, size);
    }
}

void CDNS::CborOutputWriter::open()
{
    if (m_is_fd) {
        struct stat buffer;
        if (fstat(m_fd, &buffer) != 0)
            throw CborOutputException("Given file descriptor is invalid!");
    }
    else {
        m_out.open(m_name + m_extension);
        if (m_out.fail())
            throw CborOutputException("Couldn't open the output file!");
    }
}

void CDNS::CborOutputWriter::close()
{
    if (m_is_fd) {
        if (m_fd != -1)
            ::close(m_fd);
    }
    else {
        if (m_out.is_open()) {
            m_out.flush();
            m_out.close();
        }
    }
}

void CDNS::GzipCborOutputWriter::write(const char* p, std::size_t size)
{
    m_gzip.next_in = reinterpret_cast<const unsigned char*>(p);
    m_gzip.avail_in = size;

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
        throw CborOutputException("Couldn't initialize GZIP compression!");

    // Open output
    if (m_is_fd) {
        struct stat buffer;
        if (fstat(m_fd, &buffer) != 0)
            throw CborOutputException("Given file descriptor is invalid!");
    }
    else {
        m_out.open(m_name + m_extension);
        if (m_out.fail())
            throw CborOutputException("Couldn't open the output file!");
    }
}

void CDNS::GzipCborOutputWriter::close()
{
    // Close GZIP stream
    try {
        if (m_gzip.state) {
            while(write_gzip(2048, Z_FINISH) != Z_STREAM_END);
            deflateEnd(&m_gzip);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Close output
    if (m_is_fd) {
        if (m_fd != -1)
            ::close(m_fd);
    }
    else {
        if (m_out.is_open()) {
            m_out.flush();
            m_out.close();
        }
    }
}

int CDNS::GzipCborOutputWriter::write_gzip(std::size_t in_size, int action)
{
    std::size_t size = in_size + in_size / 3 + 128;
    uint8_t buff[size];

    // Set output buffer
    m_gzip.next_out = buff;
    m_gzip.avail_out = size;

    int ret = deflate(&m_gzip, action);
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Write output
        if (m_is_fd) {
            std::size_t to_write = sizeof(buff) - m_gzip.avail_out;
            std::size_t ret = ::write(m_fd, buff, to_write);
            if (ret != to_write)
                throw CborOutputException("Couldn't write to output file!");
        }
        else
            m_out.write(reinterpret_cast<const char*>(buff), sizeof(buff) - m_gzip.avail_out);
    }
    else
        throw CborOutputException("Couldn't write to output file!");

    return ret;
}

void CDNS::XzCborOutputWriter::write(const char* p, std::size_t size)
{
    m_lzma.next_in = reinterpret_cast<const uint8_t*>(p);
    m_lzma.avail_in = size;

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

    // Open output
    if (m_is_fd) {
        struct stat buffer;
        if (fstat(m_fd, &buffer) != 0)
            throw CborOutputException("Given file descriptor is invalid!");
    }
    else {
        m_out.open(m_name + m_extension);
        if (m_out.fail())
            throw CborOutputException("Couldn't open the output file!");
    }
}

void CDNS::XzCborOutputWriter::close()
{
    // Close LZMA stream
    try {
        if (m_lzma.internal) {
            while(write_lzma(2048, LZMA_FINISH) != LZMA_STREAM_END);
            lzma_end(&m_lzma);
        }
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Close output
    if (m_is_fd) {
        if (m_fd != -1)
            ::close(m_fd);
    }
    else {
        if (m_out.is_open()) {
            m_out.flush();
            m_out.close();
        }
    }
}

lzma_ret CDNS::XzCborOutputWriter::write_lzma(std::size_t in_size, lzma_action action)
{
    std::size_t size = in_size + in_size / 3 + 128;
    uint8_t buff[size];

    // Set output buffer
    m_lzma.next_out = buff;
    m_lzma.avail_out = size;

    // Compress data to output buffer
    lzma_ret ret = lzma_code(&m_lzma, action);
    if (ret == LZMA_OK || ret == LZMA_STREAM_END) {
        // Write to output
        if (m_is_fd) {
            std::size_t to_write = sizeof(buff) - m_lzma.avail_out;
            std::size_t ret = ::write(m_fd, buff, to_write);
            if (ret != to_write)
                throw CborOutputException("Couldn't write to output file!");
        }
        else
            m_out.write(reinterpret_cast<const char*>(buff), sizeof(buff) - m_lzma.avail_out);
    }
    else
        throw CborOutputException("Couldn't write to output file!");

    return ret;
}
