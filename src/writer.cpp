#include <iostream>

#include "cdns_encoder.h"

void CDNS::BaseCborOutputWriter::open(const std::string& name)
{
    // Check if some file isn't already opened
    if (m_out.is_open())
        close();

    m_name = name;
    m_out.open(m_name + m_extension);
    if (m_out.fail())
        throw CborOutputException("Couldn't open the output file\n");
}

void CDNS::BaseCborOutputWriter::write(const char* p, std::size_t size)
{
    // Check if output file is opened
    if (!m_out.is_open())
        open();

    m_out.write(p, size);
}

void CDNS::GzipCborOutputWriter::open(const std::string& name)
{
    // Check if some file isn't already opened
    if (m_gzip)
        close();

    m_name = name;
    m_gzip = gzopen(std::string(m_name + m_extension).c_str(), "wb");
    if (!m_gzip)
        throw CborOutputException("Couldn't open the output file\n");
}

void CDNS::GzipCborOutputWriter::write(const char* p, std::size_t size)
{
    // Check if output file is opened
    if (!m_gzip)
        open();

    int ret = gzwrite(m_gzip, p, size);
    if (!ret)
        throw CborOutputException("Couldn't write to file\n");
}

void CDNS::XzCborOutputWriter::open(const std::string& name)
{
    // Check if some file isnt't already opened
    if (m_out.is_open() || m_lzma.internal)
        close();

    // Initialize LZMA stream
    m_lzma = LZMA_STREAM_INIT;
    lzma_ret ret = lzma_easy_encoder(&m_lzma, 6 /* XZ utils default */, LZMA_CHECK_CRC64);
    if (ret != LZMA_OK)
        throw CborOutputException("Couldn't initialize LZMA compression\n");

    // Open output file
    m_name = name;
    m_out.open(m_name + m_extension);
    if (m_out.fail())
        throw CborOutputException("Couldn't open the output file\n");
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
        if (!m_out.is_open())
            throw CborOutputException("No output file is opened\n");
        
        // Write compressed data from output buffer to file
        m_out.write(reinterpret_cast<const char*>(buff), sizeof(buff) - m_lzma.avail_out);
    }
    else
        throw CborOutputException("Couldn't write to output file\n");
    
    return ret;
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
        std::cerr << e.what();
    }

    // Close output file
    if (m_out.is_open()) {
        m_out.flush();
        m_out.close();
    }
}

void CDNS::XzCborOutputWriter::write(const char* p, std::size_t size)
{
    // Check if output file and LZMA stream are opened
    if (!m_out.is_open() || !m_lzma.internal)
        open();

    m_lzma.next_in = reinterpret_cast<const uint8_t*>(p);
    m_lzma.avail_in = size;

    while(m_lzma.avail_in > 0) {
        write_lzma(size, LZMA_RUN);
    }
}