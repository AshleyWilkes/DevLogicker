#pragma once
#include <string>
/*
 * Input interface to be used for initialization of Logicker objects.
 *
 * Input represents a tree structure of string-labeled values. Clients 
 * ask for values in the structure using the get<T>(name) method.
 *
 * If the requested name does not exist in the structure, 
 * std::invalid_argument is thrown. If the name exists, the associated
 * value is converted to requested type T and returned. If the
 * conversion fails, the behavior is undefined.
 *
 * For the following value types the conversion shall never fail
 * (provided input object was initialized with appropriate data):
 *
 * - input (allows to traverse down the tree)
 * - bool
 * - int
 * - std::string
 * - boost::dynamic_bitset (see Note below)
 * - std::vector<T>, where T is any supported type (including vector itself)
 * 
 * Input requires implementation to support at least the following
 * value ranges:
 * - int: 16-bit signed values [-32768, 32767]
 * - string and vector: size of 65536
 * - recursive vectors of depth 4 (i.e. e.g. vector<vector<vector<vector<int>>>>)
 *
 * Note!: The above allows for vector<bool>, which is not the preferred way of
 * working with such a data structure; bitset is. The size of the vector is 
 * not known at compilation time, so boost::dynamic_bitset has to be
 * used. In order not to force users to use bitset instead of vector<bool>,
 * the vector<bool> option is also made available.
 *
 * When bitset is specified as type T, we expect vector<bool> to be present and
 * convert is to bitset. Also if bitset is specified as T in vector<T> type,
 * we treat is as vector<bool>, so e.g. vector<bitset> is treated as vector<vector<bool>>
 * before being converted to vector<bitset>. The same principle applies with
 * recursive vector<vector<bitset>> and deeper recursive vectors.
 */

namespace logicker::core::input {
  class input {
    public:
      template<typename T>
      const T get( const std::string& name ) const;

      virtual ~input() = 0;
  };
}
