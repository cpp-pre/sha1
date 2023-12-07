# cpp::pre::sha1 - Computes and parse SHA1
Simple sha1 computation and parsing library


```cpp

  /**
   * \param p_arg buffer to hash
   * \return sha1 Hash of the given string
   */
  inline std::array<uint32_t, 5> hash(const std::string &p_arg);

  //! \return sha1 Hash as string
  inline std::string to_string(std::array<uint32_t, 5> hash);

  /**
   * \param str sha1 as string  
   * \return sha1 as binary representation from the provided hash string
   */
  inline std::array<uint32_t, 5> from_string(const std::string& str);

```