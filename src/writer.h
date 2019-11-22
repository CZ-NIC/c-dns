#pragma once

#include <string>
#include <fstream>
#include <cstdint>

#include <zlib.h>
#include <lzma.h>

namespace CDNS {

    /**
     * @enum CborOutputCompression
     * @brief Enumerates types of compression for the CDNS output file
     */
    enum class CborOutputCompression : uint8_t {
        NO_COMPRESSION = 0,
        GZIP,
        XZ
    };

    /**
     * @brief Exception thrown if there's some issue with export of CBOR data to file
     */
    class CborOutputException : public std::runtime_error {
        public:
        explicit CborOutputException(const char* msg) : std::runtime_error(msg) {}
        explicit CborOutputException(const std::string& msg) : std::runtime_error(msg) {}
    };

    /**
     * @brief Basic output writer for writing uncompressed data to output file
     */
    class BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new BaseCborOutputWriter object
         * @param name Name of the new output file without compression file extensions
         */
        BaseCborOutputWriter(const std::string& name) : m_name(name) {}

        /**
         * @brief Destroy the BaseCborOutputWriter object and close any open output file
         */
        virtual ~BaseCborOutputWriter() { close(); }

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets 
         * closed before opening the new one.
         * @param name Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file failed
         */
        virtual void open(const std::string& name);

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets
         * closed before opening the new one.
         * @throw CborOutputException if opening of output file failed
         */
        virtual void open () { open(m_name); }

        /**
         * @brief Close the current output file if there's any.
         */
        virtual void close() {
            if (m_out.is_open()) {
                m_out.flush();
                m_out.close();
            }
        };

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if no output file is opened for writing
         */
        virtual void write(const char* p, std::size_t size);

        protected:
        std::string m_name;

        private:
        std::string m_extension = ".cdns";
        std::ofstream m_out;
    };

    /**
     * @brief Output writer that uses gzip compression for the data it writes to output file
     */
    class GzipCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new GzipCborOutputWriter object
         * @param name Name of the new output file without compression file extensions
         */
        GzipCborOutputWriter(const std::string& name) : BaseCborOutputWriter(name), m_gzip(nullptr) {}

        /**
         * @brief Destroy the GzipCborOutputWriter object and close any open output file
         */
        ~GzipCborOutputWriter() { close(); }

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets 
         * closed before opening the new one.
         * @param name Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file failed
         */
        void open(const std::string& name) override;

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets
         * closed before opening the new one.
         * @throw CborOutputException if opening of output file failed
         */
        void open() override { open(m_name); }

        /**
         * @brief Close the current output file if there's any
         */
        void close() override {
            if (m_gzip) {
                gzclose(m_gzip);
                m_gzip = nullptr;
            }
        }

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with the data
         * @param size Size of the data in bytes
         * @throw CborOutputException if no file is opened or writing to file fails
         */
        void write(const char* p, std::size_t size) override;

        private:
        std::string m_extension = ".cdns.gz";
        gzFile m_gzip;
    };

    /**
     * @brief Output writer that uses LZMA2 compression for the data it writes to output file
     */
    class XzCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new XzCborOutputWriter object
         * @param name Name of the new output file without compression file extensions
         */
        XzCborOutputWriter(const std::string& name) : BaseCborOutputWriter(name), m_lzma(LZMA_STREAM_INIT) {}

        /**
         * @brief Destroy the XzCborOutputWriter object and close any open output file
         */
        ~XzCborOutputWriter() { close(); }

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets
         * closed before opening the new one.
         * @param name Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file failed
         */
        void open(const std::string& name) override;

        /**
         * @brief Open new output file to write to. If there's already a file open, it gets
         * closed before opening the new one.
         * @throw CborOutputException if opening of output file failed
         */
        void open() override { open(m_name); }

        /**
         * @brief Close the current output file if there's any
         */
        void close() override;

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if no file is opened or writing to file failed
         */
        void write(const char* p, std::size_t size) override;

        private:
        /**
         * @brief Compress data with LZMA2 and write them to output file
         * @param in_size Size of the uncompressed data in bytes
         * @param action What to do with LZMA stream (LZMA_RUN, LZMA_FINISH)
         * @throw CborOutputException if no file is opened or writing to file
         * @return LZMA return code
         */
        lzma_ret write_lzma(std::size_t in_size, lzma_action action);

        std::string m_extension = ".cdns.xz";
        lzma_stream m_lzma;
        std::ofstream m_out;
    };
}