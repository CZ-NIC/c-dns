/*
 * Copyright 2016-2019 Internet Corporation for Assigned Names and Numbers.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*
 * Developed by Sinodun IT (www.sinodun.com)
 * Modified by CZ.NIC (www.nic.cz)
 */

#pragma once

#include <deque>
#include <unordered_map>
#include <stdexcept>

#include "hash.h"

namespace CDNS{

    /**
     * @class KeyRef
     * @brief A class to let a reference act as a map key.
     */
    template<typename T>
    class KeyRef
    {
    public:
        /**
         * @brief Constructor.
         * 
         * @param key a reference to the key item.
         */
        explicit KeyRef(const T& key) : key_(key) { }

        /**
         * @brief Equality operator.
         * 
         * @param rhs the key reference to compare to.
         * @returns `true` if the two referenced items are equal.
         */
        bool operator==(const KeyRef<T>& rhs) const
        {
            return ( key_ == rhs.key_ );
        }

        /**
         * @brief Inequality operator.
         * 
         * @param rhs the key reference to compare to.
         * @returns `false` if the two referenced items are equal.
         */
        bool operator!=(const KeyRef<T>& rhs) const
        {
            return !( *this == rhs );
        }

        /**
         * @brief Calculate a hash value for the referenced item.
         * 
         * @returns hash value.
         */
        friend std::size_t hash_value(const KeyRef<T>& k)
        {
            CDNS::hash<T> hash_func;
            return hash_func(k.key_);
        }

    private:
        /**
         * @brief reference to the item
         */
        const T& key_;
    };

    /**
     * @brief Representation of one block table's table
     */
    template<typename T, typename K = T>
    class BlockTable {
    public:
        /**
         * @brief Default constructor.
         */
        explicit BlockTable() {}

        /**
         * @brief Find if a key value is in the list
         * 
         * @param key the key value to search for.
         * @param index the index of the item, if found.
         * @returns `true` if the item is found.
         */
        bool find(const K& key, index_t& index)
        {
            auto find = indexes_.find(KeyRef<K>(key));
            if ( find != indexes_.end() )
            {
                index = find->second;
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
         * @brief Add a new value to the list
         * 
         * Add a new value to the list and update the map to reference
         * the location of the value in the vector
         * 
         * @param val the value to add.
         * @returns index reference to the value.
         */
        index_t add_value(const T& val)
        {
            items_.push_back(val);
            return record_last_key();
        }

        /**
         * @brief Add a new value to the list
         * 
         * Add a new value to the list and update the map to reference
         * the location of the value in the vector
         * 
         * @param val the value to add.
         * @returns index reference to the value.
         */
        index_t add_value(T&& val)
        {
            items_.push_back(val);
            return record_last_key();
        }

        /**
         * @brief Add a new value to the list.
         * 
         * If the value is present in the list already, return existing
         * value index. Otherwise add the value to the list.
         * 
         * @param val the value to add.
         * @returns index reference to the value.
         */
        index_t add(const T& val)
        {
            const K& key = val.key();
            index_t res;
            if ( !find(key, res) )
                res = add_value(val);
            return res;
        }

        /**
         * @brief Clear the list contents.
         */
        void clear()
        {
            items_.clear();
            indexes_.clear();
        }

        /**
         * @brief Get the indexed item.
         * 
         * @param pos the index.
         * @throw std::runtime_error if given index is out of range
         */
        const T& operator[](index_t pos) const
        {
            if ( pos < items_.size() )
                return items_[pos];
            
            throw std::runtime_error("Block index out of range");
        }

        /**
         * @brief Get the number of items stored.
         */
        typename std::deque<T>::size_type size() const
        {
            return items_.size();
        }

        /**
         * @brief Iterator begin
         * 
         * @returns iterator.
         */
        typename std::deque<T>::iterator begin()
        {
            return items_.begin();
        }

        /**
         * @brief Iterator end
         * 
         * @returns iterator.
         */
        typename std::deque<T>::iterator end()
        {
            return items_.end();
        }

    private:
        /**
         * @brief Record the key to the latest item in the vector.
         * 
         * @returns index reference to the value.
         */
        index_t record_last_key()
        {
            index_t res = items_.size();
            res -= 1;
            indexes_[KeyRef<K>(items_.back().key())] = res;
            return res;
        }

        std::deque<T> items_;
        std::unordered_map<KeyRef<K>, index_t, CDNS::hash<KeyRef<K>>> indexes_;
    };
}