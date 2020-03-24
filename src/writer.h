/**
 * Copyright © 2019 CZ.NIC, z. s. p. o.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#define ZLIB_CONST

#include <string>
#include <fstream>
#include <cstdint>
#include <boost/any.hpp>
#include <memory>
#include <type_traits>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

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
        virtual ~BaseCborOutputWriter() = default;

        /** Delete assignment operators */
        virtual BaseCborOutputWriter& operator=(BaseCborOutputWriter& rhs) = delete;
        virtual BaseCborOutputWriter&& operator=(BaseCborOutputWriter&& rhs) = delete;

        /**
         * @brief Write data in buffer to output
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         */
        virtual void write(const char* p, std::size_t size) = 0;

        /**
         * @brief Rotate the output file (currently opened output is closed)
         * @param value Name or other identifier of the new output
         */
        virtual void rotate_output(const boost::any& value) = 0;

        protected:
        /**
         * @brief Open the output with given identifier or check if its valid
         */
        virtual void open() {};

        /**
         * @brief Close the opened output
         */
        virtual void close() {};
    };

    /**
     * @brief Writes given data to output specified by parameter of type T
     *
     * This default implementation does nothing. Use implemented specializations for type T or
     * implement your own specializations.
     */
    template<typename T>
    class Writer : public BaseCborOutputWriter {
        public:
        Writer(const T& value) : BaseCborOutputWriter(), m_value(value) {}
        Writer(Writer& copy) = delete;
        Writer(Writer&& copy) = delete;

        /**
         * @brief Write data in buffer to output
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         */
        void write([[maybe_unused]] const char* p, [[maybe_unused]] std::size_t size) override {}

        /**
         * @brief Rotate the output file (currently opened output is closed)
         * @param value Name or other identifier of the new output
         */
        void rotate_output(const boost::any& value) override {}

        protected:
        T m_value;
    };

    /**
     * @brief Writes given data to output file specified by file's name
     * @tparam std::string Output file's name
     */
    template<>
    class Writer<std::string> : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new Writer<std::string> object for writing data to output file
         * @param filename Name of the output file
         * @param extension Extension for the output file's name
         * @throw CborOutputExtension if opening of the output file fails
         */
        Writer(const std::string& filename, const std::string extension = "")
            : BaseCborOutputWriter(), m_value(filename), m_extension(extension), m_out() { open(); }

        /**
         * @brief Destroy the Writer object and close the current output file
         */
        ~Writer() override { close(); }

        /** Delete copy and move constructors */
        Writer(Writer& copy) = delete;
        Writer(Writer&& copy) = delete;

        /**
         * @brief Write data in buffer to output file
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw std::ios_base::failure if writing to output file fails
         */
        void write(const char* p, std::size_t size) override {
            m_out.write(p, size);
        }

        /**
         * @brief Rotate the output file (currently opened output is closed)
         * @param value Name of the new output file
         * @throw CborOutputException if opening of the output file fails
         */
        void rotate_output(const boost::any& value) override {
            if (value.type() != typeid(std::string))
                return;

            close();
            m_value = boost::any_cast<std::string>(value);
            open();
        }

        protected:
        /**
         * @brief Open the output file with given name
         * @throw CborOutputException if opening of the file fails
         */
        void open() override {
            m_out.open(m_value + m_extension + ".part");
            if (m_out.fail())
                throw CborOutputException("Couldn't open the output file!");
        }

        /**
         * @brief Close the opened output file with given name
         */
        void close() override {
            if (m_out.is_open()) {
                m_out.flush();
                m_out.close();
                if (std::rename((m_value + m_extension + ".part").c_str(), (m_value + m_extension).c_str()))
                    throw CborOutputException("Couldn't rename the output file!");
            }
        }

        std::string m_value;
        std::string m_extension;
        std::ofstream m_out;
    };

    /**
     * @brief Writes data to output specified by given file descriptor
     * @tparam int Output's file descriptor
     */
    template<>
    class Writer<int> : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new Writer<int> object for writing data to output file descriptor
         * @param fd File descriptor for the output
         * @throw CborOutputException if the file descriptor isn't valid
         */
        Writer(const int& fd) : BaseCborOutputWriter(), m_value(fd) { open(); }

        /**
         * @brief Destroy the Writer object and close the current output file descriptor
         */
        ~Writer() override { close(); }

        /** Delete copy and move constructors */
        Writer(Writer& copy) = delete;
        Writer(Writer&& copy) = delete;

        /**
         * @brief Write data in buffer to output file descriptor
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if writing to output file descriptor fails
         */
        void write(const char* p, std::size_t size) override {
            int ret = ::write(m_value, p, size);
            if (ret != size) {
                throw CborOutputException("Given " + std::to_string(size) + " bytes to write, but "
                                        "only " + std::to_string(ret) + " bytes were written!");
            }
        }

        /**
         * @brief Rotate the output file descriptor (currently opened output is closed)
         * @param value File descriptor of the new output
         * @throw CborOutputException if checking of the file descriptor fails
         */
        void rotate_output(const boost::any& value) override {
            if (value.type() != typeid(int))
                return;

            close();
            m_value = boost::any_cast<int>(value);
            open();
        }

        protected:
        /**
         * @brief Check if the given file descriptor is valid
         * @throw CborOutputException if the file descriptor isn't valid
         */
        void open() override {
            struct stat buffer;
            if (fstat(m_value, &buffer) != 0)
                throw CborOutputException("Given file descriptor is invalid!");
        }

        /**
         * @brief Close the opened output file descriptor
         */
        void close() override {
            if (m_value != -1)
                ::close(m_value);
        }

        int m_value;
    };

    /**
     * @brief Writes uncompressed data to output specified by name or other identifier
     */
    class CborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new CborOutputWriter object for writing data to output
         * @param value Name or other identifier of the output
         * @throw CborOutputException if initialization of the output fails
         */
        template<typename T>
        CborOutputWriter(const T& value) : m_writer(nullptr) {
            m_writer = std::make_unique<Writer<T>>(value);
        }

        /** Delete copy and move constructors */
        CborOutputWriter(CborOutputWriter& copy) = delete;
        CborOutputWriter(CborOutputWriter&& copy) = delete;

        /**
         * @brief Write uncompressed data in buffer to output
         * @param p Start of the buffer with data
         * @param size Size of the data in bytes
         * @throw CborOutputException if writing to output file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         */
        void write(const char* p, std::size_t size) override {
            m_writer->write(p, size);
        }

        /**
         * @brief Rotate the output (currently opened output is closed)
         * @param value Name or other identifier of the new output
         * @throw CborOutputException if initialization of the new output fails
         */
        void rotate_output(const boost::any& value) override {
            m_writer->rotate_output(value);
        }

        private:
        std::unique_ptr<BaseCborOutputWriter> m_writer;
    };

    /**
     * @brief Writes data compressed with GZIP to output specified by name or other identifier
     */
    class GzipCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new GzipCborOutputWriter object for writing GZIP compressed data to output
         * @param value Name or other identifier of the output
         * @throw CborOutputException if initialization of the output fails
         */
        template<typename T>
        GzipCborOutputWriter(const T& value) : m_writer(nullptr), m_gzip() {
            if constexpr (std::is_same_v<T, std::string>)
                m_writer = std::make_unique<Writer<T>>(value, ".gz");
            else
                m_writer = std::make_unique<Writer<T>>(value);

            open();
        }

        /**
         * @brief Destroy the GzipCborOutputWriter object and close the current output
         */
        ~GzipCborOutputWriter() override { close(); }

        /** Delete copy and move constructors */
        GzipCborOutputWriter(GzipCborOutputWriter& copy) = delete;
        GzipCborOutputWriter(GzipCborOutputWriter&& copy) = delete;

        /**
         * @brief Compress data in buffer with GZIP and write them to output
         * @param p Start of the buffer with uncompressed data
         * @param size Size of the uncompressed data in bytes
         * @throw CborOutputException if writing to output file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         */
        void write(const char* p, std::size_t size) override;

        /**
         * @brief Rotate the output (currently opened output is closed)
         * @param value Name or other identifier of the new output
         * @throw CborOutputException if initialization of the new output fails
         */
        void rotate_output(const boost::any& value) override {
            close();
            m_writer->rotate_output(value);
            open();
        }

        private:
        /**
         * @brief Open the output with given identifier or check if its valid
         * @throw CborOutputException if initialization of the output fails
         */
        void open() override;

        /**
         * @brief Close the opened output
         */
        void close() override;

        /**
         * @brief Compress data with GZIP and write them to output
         * @param in_size Size of the uncompressed data in bytes
         * @param action What to do with GZIP stream (Z_NO_FLUSH, Z_FINISH)
         * @throw CborOutputException if writing to output file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         * @return ZLIB return code
         */
        int write_gzip(std::size_t in_size, int action);

        std::unique_ptr<BaseCborOutputWriter> m_writer;
        z_stream m_gzip;
    };

    /**
     * @brief Writes data compressed with LZMA2 to output specified by name or other identifier
     */
    class XzCborOutputWriter : public BaseCborOutputWriter {
        public:
        /**
         * @brief Construct a new XzCborOutputWriter object for writing LZMA2 compressed data to output
         * @param value Name or other identifier of the output
         * @throw CborOutputException if initialization of the output fails
         */
        template<typename T>
        XzCborOutputWriter(const T& value) : m_writer(nullptr), m_lzma(LZMA_STREAM_INIT) {
            if constexpr (std::is_same_v<T, std::string>)
                m_writer = std::make_unique<Writer<T>>(value, ".xz");
            else
                m_writer = std::make_unique<Writer<T>>(value);

            open();
        }

        /**
         * @brief Destroy the XzCborOutputWriter object and close the current output
         */
        ~XzCborOutputWriter() override { close(); }

        /** Delete copy and move constructors */
        XzCborOutputWriter(XzCborOutputWriter& copy) = delete;
        XzCborOutputWriter(XzCborOutputWriter&& copy) = delete;

        /**
         * @brief Compress data in buffer with LZMA2 and write them to output
         * @param p Start of the buffer with uncompressed data
         * @param size Size of the uncompressed data in bytes
         * @throw CborOutputException if writing to output file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         */
        void write(const char* p, std::size_t size) override;

        /**
         * @brief Rotate the output (currently opened output is closed)
         * @param value Name or other identifier of the new output
         * @throw CborOutputException if initialization of the new output fails
         */
        void rotate_output(const boost::any& value) override {
            close();
            m_writer->rotate_output(value);
            open();
        }

        private:
        /**
         * @brief Open the output with given identifier or check if its valid
         * @throw CborOutputException if initialization of the output fails
         */
        void open() override;

        /**
         * @brief Close the opened output
         */
        void close() override;

        /**
         * @brief Compress data with LZMA2 and write them to output
         * @param in_size Size of the uncompressed data in bytes
         * @param action What to do with LZMA stream (LZMA_RUN, LZMA_FINISH)
         * @throw CborOutputException if writing to file descriptor fails
         * @throw std::ios_base::failure if writing to output file fails
         * @return lzma_ret LZMA return code
         */
        lzma_ret write_lzma(std::size_t in_size, lzma_action action);

        std::unique_ptr<BaseCborOutputWriter> m_writer;
        lzma_stream m_lzma;
    };
}
