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
     * @brief Abstract class serving as common interface for output writers
     */
    class BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new BaseCborOutputWriter object with output file
         * @param filename Name of the new output file without comperssion file extensions
         */
        BaseCborOutputWriter(const std::string& filename) : m_name(filename), m_is_fd(false) {}

        /**
         * @brief Construct a new BaseCborOutputWriter object with ouput file descriptor
         * @param fd Valid file descriptor to output data
         */
        BaseCborOutputWriter(const int fd) : m_fd(fd), m_is_fd(true) {}

        /**
         * @brief Destroy the BaseCborOutputWriter object
         */
        virtual ~BaseCborOutputWriter() {}

        /**
         * @brief Write data in buffer to output
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         */
        virtual void write(const char* p, std::size_t size) = 0;

        /**
         * @brief Rotate the output file (currently opened file or file descriptor is closed)
         * @param filename Name of the new output file without compression file extensions
         */
        void rotate_output(const std::string& filename) {
            close();
            m_name = filename;
            m_is_fd = false;
            open();
        }

        /**
         * @brief Rotate the output file descriptor (currently opened file or file descriptor is closed)
         * @param fd Valid file descriptor to output data
         */
        void rotate_output(const int fd)
        {
            close();
            m_fd = fd;
            m_is_fd = true;
            open();
        }

        protected:
        /**
         * @brief Open the output file with given filename or check if given file descriptor is valid
         */
        virtual void open() = 0;

        /**
         * @brief Close the opened output file or file descriptor
         */
        virtual void close() = 0;

        std::string m_name;
        int m_fd;
        bool m_is_fd;
    };

    /**
     * @brief Basic output writer for writing uncompressed data to output
     */
    class CborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new CborOutputWriter object with output file
         * @param filename Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file fails
         */
        CborOutputWriter(const std::string& filename) : BaseCborOutputWriter(filename) { open(); }

        /**
         * @brief Construct a new CborOutputWriter object with output file descriptor
         * @param fd Valid file descriptor to output data
         * @throw CborOutputException if given file descriptor isn't valid
         */
        CborOutputWriter(const int fd) : BaseCborOutputWriter(fd) { open(); }

        /**
         * @brief Destroy the CborOutputWriter object and close any opened file or file descriptor
         */
        ~CborOutputWriter() override { close(); }

        /**
         * @brief Write data in buffer to output
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if writing to output file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         */
         void write(const char* p, std::size_t size) override;

        private:
        /**
         * @brief Open the output file with given filename or check if given file descriptor is valid
         * @throw CborOutputException if opening of the file or checking of file descriptor fails
         */
        void open() override;

        /**
         * @brief Close the opened output file or file descriptor
         */
        void close() override;

        std::string m_extension = ".cdns";
        std::ofstream m_out;
    };

    /**
     * @brief Output writer that uses gzip compression for the data it writes to output
     */
    class GzipCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new GzipCborOutputWriter object with output file
         * @param filename Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file fails
         */
        GzipCborOutputWriter(const std::string& filename) : BaseCborOutputWriter(filename), m_gzip(nullptr) { open(); }

        /**
         * @brief Construct a new GzipCborOutputWriter object with output file descriptor
         * @param fd Valid file descriptor to output data
         * @throw CborOutputException if given file descriptor isn't valid
         */
        GzipCborOutputWriter(const int fd) : BaseCborOutputWriter(fd), m_gzip(nullptr) { open(); }

        /**
         * @brief Destroy the GzipCborOutputWriter object and close any opened output file or file descriptor
         */
        ~GzipCborOutputWriter() override { close(); }

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with the data
         * @param size Size of the data in bytes
         * @throw CborOutputException if writing to output fails
         */
        void write(const char* p, std::size_t size) override;

        private:
        /**
         * @brief Open the output file with given filename or check if given file descriptor is valid
         * @throw CborOutputException if opening of the file or checking of file descriptor fails
         */
        void open() override;

        /**
         * @brief Close the opened output file or file descriptor
         */
        void close() override;

        std::string m_extension = ".cdns.gz";
        gzFile m_gzip;
    };

    /**
     * @brief Output writer that uses LZMA2 compression for the data it writes to output file
     */
    class XzCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new XzCborOutputWriter object with output file
         * @param filename Name of the new output file without compression file extensions
         * @throw CborOutputException if opening of output file fails
         */
        XzCborOutputWriter(const std::string& filename) : BaseCborOutputWriter(filename),
                                                          m_lzma(LZMA_STREAM_INIT) { open(); }

        /**
         * @brief Construct a new XzCborOutputWriter object with output file descriptor
         * @param fd Valid file descriptor to output data
         * @throw CborOutputException if given file descriptor isn't valid
         */
        XzCborOutputWriter(const int fd) : BaseCborOutputWriter(fd), m_lzma(LZMA_STREAM_INIT) { open(); }

        /**
         * @brief Destroy the XzCborOutputWriter object and close any opened output file or file descriptor
         */
        ~XzCborOutputWriter() override { close(); }

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if writing to output failed
         */
        void write(const char* p, std::size_t size) override;

        private:
        /**
         * @brief Open the output file with given filename or check if given file descriptor is valid
         * @throw CborOutputException if opening of the file or checking of file descriptor fails
         */
        void open() override;

        /**
         * @brief Close the opened output file or file descriptor
         */
        void close() override;

        /**
         * @brief Compress data with LZMA2 and write them to output
         * @param in_size Size of the uncompressed data in bytes
         * @param action What to do with LZMA stream (LZMA_RUN, LZMA_FINISH)
         * @throw CborOutputException if writing to file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         * @return LZMA return code
         */
        lzma_ret write_lzma(std::size_t in_size, lzma_action action);

        std::string m_extension = ".cdns.xz";
        lzma_stream m_lzma;
        std::ofstream m_out;
    };
}