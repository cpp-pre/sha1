#ifndef CPP_PRE_SHA1_HPP
#define CPP_PRE_SHA1_HPP

#include <array>
#include <boost/uuid/detail/sha1.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/timer/timer.hpp>

namespace pre::sha1 {

  //! \return sha1 Hash of the given string
  inline std::array<uint32_t, 5> hash(const std::string &p_arg);

  //! \return sha1 Hash as string
  inline std::string to_string(std::array<uint32_t, 5> hash);

  /**
   * \param str sha1 as string  
   * \return sha1 as binary representation from the provided hash string
   */
  inline std::array<uint32_t, 5> from_string(const std::string& str);

  inline std::array<uint32_t, 5> hash(const std::string &p_arg) {
    boost::uuids::detail::sha1 sha1;
    sha1.process_bytes(p_arg.data(), p_arg.size());
    uint32_t hash[5] = {0};
    sha1.get_digest(hash);

    return std::array<uint32_t, 5>{hash[0], hash[1], hash[2], hash[3], hash[4]};
  }

  inline std::string to_string(std::array<uint32_t, 5> hash) {
    char buf[41] = {0};

    for (int i = 0; i < 5; i++) {
      std::snprintf(buf + (i << 3), std::size(buf), "%08x", hash[i]);
    }

    return std::string(buf);
  }

  using namespace boost::spirit;
  template <typename Iterator, typename Skipper>
  struct sha1_grammar : qi::grammar<Iterator, std::vector<uint32_t>(), Skipper>
  {
    sha1_grammar() : sha1_grammar::base_type{value}
    {
      using boost::spirit::qi::uint_parser;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::phrase_parse;
      using boost::phoenix::ref;

      value = 
           uint_parser<uint32_t, 16, 8, 8>{} 
        >> uint_parser<uint32_t, 16, 8, 8>{} 
        >> uint_parser<uint32_t, 16, 8, 8>{} 
        >> uint_parser<uint32_t, 16, 8, 8>{}
        >> uint_parser<uint32_t, 16, 8, 8>{}
        ;
    }

    qi::rule<Iterator, std::vector<uint32_t>(), Skipper> value;
  };

  inline thread_local sha1_grammar<std::string::const_iterator, boost::spirit::ascii::space_type> g;
  inline std::array<uint32_t, 5> from_string(const std::string& str) {
    
    std::array<uint32_t, 5> converted;
    {
      std::vector<uint32_t> parsed;

      using boost::spirit::ascii::space;
      auto begin = str.begin();
      bool r = phrase_parse(
          begin,
          str.end(),
          g,
          space, 
          parsed
      );


      if (begin == str.end()) { 
        std::copy_n(parsed.begin(), 5, converted.begin());
      }
  
    }

    return converted;
  }

}

#endif // CPP_PRE_SHA1