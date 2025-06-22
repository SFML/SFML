//
//  unicodelib_encodings.h
//
//  Copyright (c) 2020 Yuji Hirose. All rights reserved.
//  MIT License
//

#pragma once

#include <cstdlib>
#include <string>

#if !(defined(__cplusplus) && __cplusplus >= 201703L) && !(defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#error "Requires complete C++17 support"
#endif

/*

  namespace utf8 {

    size_t codepoint_length(char32_t cp);
    size_t codepoint_length(const char *s8, size_t l);
    size_t codepoint_count(const char *s8, size_t l);

    size_t encode_codepoint(char32_t cp, std::string &out);
    void encode(const char32_t *s32, size_t l, std::string &out);

    size_t decode_codepoint(const char *s8, size_t l, char32_t &out);
    void decode(const char *s8, size_t l, std::u32string &out);

  }  // namespace utf8

  namespace utf16 {

    size_t codepoint_length(char32_t cp);
    size_t codepoint_length(const char16_t *s16, size_t l);
    size_t codepoint_count(const char16_t *s16, size_t l);

    size_t encode_codepoint(char32_t cp, std::u16string &out);
    void encode(const char32_t *s32, size_t l, std::u16string &out);

    size_t decode_codepoint(const char16_t *s16, size_t l, char32_t &out);
    void decode(const char16_t *s16, size_t l, std::u32string &out);

  }  // namespace utf16

  std::string to_utf8(const char16_t *s16, size_t l);
  std::u16string to_utf16(const char *s8, size_t l);

  std::wstring to_wstring(const char *s8, size_t l);
  std::wstring to_wstring(const char16_t *s16, size_t l);
  std::wstring to_wstring(const char32_t *s32, size_t l);

  std::string to_utf8(const wchar_t *sw, size_t l);
  std::u16string to_utf16(const wchar_t *sw, size_t l);
  std::u32string to_utf32(const wchar_t *sw, size_t l);

*/

namespace unicode {

//-----------------------------------------------------------------------------
// UTF8 encoding
//-----------------------------------------------------------------------------

namespace utf8 {

inline size_t codepoint_length(char32_t cp) {
  if (cp < 0x0080) {
    return 1;
  } else if (cp < 0x0800) {
    return 2;
  } else if (cp < 0xD800) {
    return 3;
  } else if (cp < 0xe000) {
    // D800 - DFFF is invalid...
    return 0;
  } else if (cp < 0x10000) {
    return 3;
  } else if (cp < 0x110000) {
    return 4;
  }
  return 0;
}

inline size_t codepoint_length(const char *s8, size_t l) {
  if (l) {
    uint8_t b = s8[0];
    if ((b & 0x80) == 0) {
      return 1;
    } else if ((b & 0xE0) == 0xC0) {
      return 2;
    } else if ((b & 0xF0) == 0xE0) {
      return 3;
    } else if ((b & 0xF8) == 0xF0) {
      return 4;
    }
  }
  return 0;
}

inline size_t codepoint_count(const char *s8, size_t l) {
  size_t count = 0;
  for (size_t i = 0; i < l; i += codepoint_length(s8 + i, l - i)) {
    count++;
  }
  return count;
}

inline size_t encode_codepoint(char32_t cp, char *buff) {
  if (cp < 0x0080) {
    buff[0] = static_cast<uint8_t>(cp & 0x7F);
    return 1;
  } else if (cp < 0x0800) {
    buff[0] = static_cast<uint8_t>(0xC0 | ((cp >> 6) & 0x1F));
    buff[1] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    return 2;
  } else if (cp < 0xD800) {
    buff[0] = static_cast<uint8_t>(0xE0 | ((cp >> 12) & 0xF));
    buff[1] = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3F));
    buff[2] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    return 3;
  } else if (cp < 0xE000) {
    // D800 - DFFF is invalid...
    return 0;
  } else if (cp < 0x10000) {
    buff[0] = static_cast<uint8_t>(0xE0 | ((cp >> 12) & 0xF));
    buff[1] = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3F));
    buff[2] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    return 3;
  } else if (cp < 0x110000) {
    buff[0] = static_cast<uint8_t>(0xF0 | ((cp >> 18) & 0x7));
    buff[1] = static_cast<uint8_t>(0x80 | ((cp >> 12) & 0x3F));
    buff[2] = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3F));
    buff[3] = static_cast<uint8_t>(0x80 | (cp & 0x3F));
    return 4;
  }
  return 0;
}

inline size_t encode_codepoint(char32_t cp, std::string &out) {
  char buff[4];
  auto l = encode_codepoint(cp, buff);
  out.append(buff, l);
  return l;
}

inline void encode(const char32_t *s32, size_t l, std::string &out) {
  for (size_t i = 0; i < l; i++) {
    encode_codepoint(s32[i], out);
  }
}

inline bool decode_codepoint(const char *s8, size_t l, size_t &bytes,
                             char32_t &cp) {
  if (l) {
    uint8_t b = s8[0];
    if ((b & 0x80) == 0) {
      bytes = 1;
      cp = b;
      return true;
    } else if ((b & 0xE0) == 0xC0) {
      if (l >= 2) {
        bytes = 2;
        cp = ((static_cast<char32_t>(s8[0] & 0x1F)) << 6) |
             (static_cast<char32_t>(s8[1] & 0x3F));
        return true;
      }
    } else if ((b & 0xF0) == 0xE0) {
      if (l >= 3) {
        bytes = 3;
        cp = ((static_cast<char32_t>(s8[0] & 0x0F)) << 12) |
             ((static_cast<char32_t>(s8[1] & 0x3F)) << 6) |
             (static_cast<char32_t>(s8[2] & 0x3F));
        return true;
      }
    } else if ((b & 0xF8) == 0xF0) {
      if (l >= 4) {
        bytes = 4;
        cp = ((static_cast<char32_t>(s8[0] & 0x07)) << 18) |
             ((static_cast<char32_t>(s8[1] & 0x3F)) << 12) |
             ((static_cast<char32_t>(s8[2] & 0x3F)) << 6) |
             (static_cast<char32_t>(s8[3] & 0x3F));
        return true;
      }
    }
  }
  return false;
}

inline size_t decode_codepoint(const char *s8, size_t l, char32_t &out) {
  size_t bytes;
  if (decode_codepoint(s8, l, bytes, out)) {
    return bytes;
  }
  return 0;
}

template <typename T>
inline void for_each(const char *s8, size_t l, T callback) {
  size_t id = 0;
  size_t i = 0;
  while (i < l) {
    auto beg = i++;
    while (i < l && (s8[i] & 0xc0) == 0x80) {
      i++;
    }
    callback(s8, l, beg, i, id++);
  }
}

inline void decode(const char *s8, size_t l, std::u32string &out) {
  for_each(
      s8, l,
      [&](const char *s, size_t /*l*/, size_t beg, size_t end, size_t /*i*/) {
        size_t bytes;
        char32_t cp;
        decode_codepoint(&s[beg], (end - beg), bytes, cp);
        out += cp;
      });
}

}  // namespace utf8

//-----------------------------------------------------------------------------
// UTF16 encoding
//-----------------------------------------------------------------------------

namespace utf16 {

inline bool is_surrogate_pair(const char16_t *s16, size_t l) {
  if (l > 0) {
    auto first = s16[0];
    if (0xD800 <= first && first < 0xDC00) {
      auto second = s16[1];
      if (0xDC00 <= second && second < 0xE000) {
        return true;
      }
    }
  }
  return false;
}

inline size_t codepoint_length(char32_t cp) { return cp <= 0xFFFF ? 1 : 2; }

inline size_t codepoint_length(const char16_t *s16, size_t l) {
  if (l > 0) {
    if (is_surrogate_pair(s16, l)) {
      return 2;
    }
    return 1;
  }
  return 0;
}

inline size_t codepoint_count(const char16_t *s16, size_t l) {
  size_t count = 0;
  for (size_t i = 0; i < l; i += codepoint_length(s16 + i, l - i)) {
    count++;
  }
  return count;
}

inline size_t encode_codepoint(char32_t cp, char16_t *buff) {
  if (cp < 0xD800) {
    buff[0] = static_cast<char16_t>(cp);
    return 1;
  } else if (cp < 0xE000) {
    // D800 - DFFF is invalid...
    return 0;
  } else if (cp < 0x10000) {
    buff[0] = static_cast<char16_t>(cp);
    return 1;
  } else if (cp < 0x110000) {
    // high surrogate
    buff[0] = static_cast<char16_t>(0xD800 + (((cp - 0x10000) >> 10) & 0x3FF));
    // low surrogate
    buff[1] = static_cast<char16_t>(0xDC00 + ((cp - 0x10000) & 0x3FF));
    return 2;
  }
  return 0;
}

inline size_t encode_codepoint(char32_t cp, std::u16string &out) {
  char16_t buff[2];
  auto l = encode_codepoint(cp, buff);
  out.append(buff, l);
  return l;
}

inline void encode(const char32_t *s32, size_t l, std::u16string &out) {
  for (size_t i = 0; i < l; i++) {
    encode_codepoint(s32[i], out);
  }
}

inline bool decode_codepoint(const char16_t *s16, size_t l, size_t &length,
                             char32_t &cp) {
  if (l) {
    // Surrogate
    auto first = s16[0];
    if (0xD800 <= first && first < 0xDC00) {
      if (l >= 2) {
        auto second = s16[1];
        if (0xDC00 <= second && second < 0xE000) {
          cp = (((first - 0xD800) << 10) | (second - 0xDC00)) + 0x10000;
          length = 2;
          return true;
        }
      }
    }

    // Non surrogate
    else {
      cp = first;
      length = 1;
      return true;
    }
  }

  return false;
}

inline size_t decode_codepoint(const char16_t *s16, size_t l, char32_t &out) {
  size_t length;
  if (decode_codepoint(s16, l, length, out)) {
    return length;
  }
  return 0;
}

template <typename T>
inline void for_each(const char16_t *s16, size_t l, T callback) {
  size_t id = 0;
  size_t i = 0;
  while (i < l) {
    auto beg = i++;
    if (is_surrogate_pair(&s16[beg], l - beg)) {
      i++;
    }
    callback(s16, l, beg, i, id++);
  }
}

inline void decode(const char16_t *s16, size_t l, std::u32string &out) {
  for_each(s16, l,
           [&](const char16_t *s, size_t /*l*/, size_t beg, size_t end,
               size_t /*i*/) {
             size_t length;
             char32_t cp;
             decode_codepoint(&s[beg], (end - beg), length, cp);
             out += cp;
           });
}

}  // namespace utf16

//-----------------------------------------------------------------------------
// Inline Wrapper functions
//-----------------------------------------------------------------------------

namespace utf8 {

inline size_t codepoint_length(std::string_view s8) {
  return codepoint_length(s8.data(), s8.length());
}

inline size_t codepoint_count(std::string_view s8) {
  return codepoint_count(s8.data(), s8.length());
}

inline std::string encode_codepoint(char32_t cp) {
  std::string out;
  encode_codepoint(cp, out);
  return out;
}

inline void encode(std::u32string_view s32, std::string &out) {
  encode(s32.data(), s32.length(), out);
}

inline std::string encode(const char32_t *s32, size_t l) {
  std::string out;
  encode(s32, l, out);
  return out;
}

inline std::string encode(std::u32string_view s32) {
  return encode(s32.data(), s32.length());
}

inline size_t decode_codepoint(std::string_view s8, char32_t &cp) {
  return decode_codepoint(s8.data(), s8.length(), cp);
}

inline char32_t decode_codepoint(const char *s8, size_t l) {
  char32_t out = 0;
  decode_codepoint(s8, l, out);
  return out;
}

inline char32_t decode_codepoint(std::string_view s8) {
  return decode_codepoint(s8.data(), s8.length());
}

inline void decode(std::string_view s8, std::u32string &out) {
  decode(s8.data(), s8.length(), out);
}

inline std::u32string decode(const char *s8, size_t l) {
  std::u32string out;
  decode(s8, l, out);
  return out;
}

inline std::u32string decode(std::string_view s8) {
  return decode(s8.data(), s8.length());
}

}  // namespace utf8

namespace utf16 {

inline size_t codepoint_length(std::u16string_view s16) {
  return codepoint_length(s16.data(), s16.length());
}

inline size_t codepoint_count(std::u16string_view s16) {
  return codepoint_count(s16.data(), s16.length());
}

inline std::u16string encode_codepoint(char32_t cp) {
  std::u16string out;
  encode_codepoint(cp, out);
  return out;
}

inline void encode(std::u32string_view s32, std::u16string &out) {
  encode(s32.data(), s32.length(), out);
}

inline std::u16string encode(const char32_t *s32, size_t l) {
  std::u16string out;
  encode(s32, l, out);
  return out;
}

inline std::u16string encode(std::u32string_view s32) {
  return encode(s32.data(), s32.length());
}

inline size_t decode_codepoint(std::u16string_view s16, char32_t &cp) {
  return decode_codepoint(s16.data(), s16.length(), cp);
}

inline char32_t decode_codepoint(const char16_t *s16, size_t l) {
  char32_t out = 0;
  decode_codepoint(s16, l, out);
  return out;
}

inline char32_t decode_codepoint(std::u16string_view s16) {
  return decode_codepoint(s16.data(), s16.length());
}

inline void decode(std::u16string_view s16, std::u32string &out) {
  decode(s16.data(), s16.length(), out);
}

inline std::u32string decode(const char16_t *s16, size_t l) {
  std::u32string out;
  decode(s16, l, out);
  return out;
}

inline std::u32string decode(std::u16string_view s16) {
  return decode(s16.data(), s16.length());
}

}  // namespace utf16

//-----------------------------------------------------------------------------
// utf8/utf16 conversion
//-----------------------------------------------------------------------------

inline std::string to_utf8(const char16_t *s16, size_t l) {
  return utf8::encode(utf16::decode(s16, l));
}

inline std::string to_utf8(std::u16string_view s16) {
  return to_utf8(s16.data(), s16.length());
}

inline std::u16string to_utf16(const char *s8, size_t l) {
  return utf16::encode(utf8::decode(s8, l));
}

inline std::u16string to_utf16(std::string_view s8) {
  return to_utf16(s8.data(), s8.length());
}

//-----------------------------------------------------------------------------
// std::wstring conversion
//-----------------------------------------------------------------------------

namespace detail {

inline std::wstring to_wstring_core(const char *s8, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    auto s16 = utf16::encode(utf8::decode(s8, l));
    return std::wstring(s16.begin(), s16.end());
  } else if constexpr (sizeof(wchar_t) == 4) {
    auto s32 = utf8::decode(s8, l);
    return std::wstring(s32.begin(), s32.end());
  }
}

inline std::wstring to_wstring_core(const char16_t *s16, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    return std::wstring(s16, s16 + l);
  } else if constexpr (sizeof(wchar_t) == 4) {
    auto s32 = utf16::decode(s16, l);
    return std::wstring(s32.begin(), s32.end());
  }
}

inline std::wstring to_wstring_core(const char32_t *s32, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    auto s16 = utf16::encode(s32, l);
    return std::wstring(s16.begin(), s16.end());
  } else if constexpr (sizeof(wchar_t) == 4) {
    return std::wstring(s32, s32 + l);
  }
}

inline std::string to_utf8_core(const wchar_t *sw, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    std::u16string buf(sw, sw + l);
    return utf8::encode(utf16::decode(buf.data(), l));
  } else if constexpr (sizeof(wchar_t) == 4) {
    std::u32string buf(sw, sw + l);
    return utf8::encode(buf.data(), l);
  }
}

inline std::u16string to_utf16_core(const wchar_t *sw, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    return std::u16string(sw, sw + l);
  } else if constexpr (sizeof(wchar_t) == 4) {
    std::u32string buf(sw, sw + l);
    return utf16::encode(buf.data(), l);
  }
}

inline std::u32string to_utf32_core(const wchar_t *sw, size_t l) {
  if constexpr (sizeof(wchar_t) == 2) {
    std::u16string buf(sw, sw + l);
    return utf16::decode(buf.data(), l);
  } else if constexpr (sizeof(wchar_t) == 4) {
    return std::u32string(sw, sw + l);
  }
}

}  // namespace detail

inline std::wstring to_wstring(const char *s8, size_t l) {
  return detail::to_wstring_core(s8, l);
}

inline std::wstring to_wstring(std::string_view s8) {
  return to_wstring(s8.data(), s8.length());
}

inline std::wstring to_wstring(const char16_t *s16, size_t l) {
  return detail::to_wstring_core(s16, l);
}

inline std::wstring to_wstring(std::u16string_view s16) {
  return to_wstring(s16.data(), s16.length());
}

inline std::wstring to_wstring(const char32_t *s32, size_t l) {
  return detail::to_wstring_core(s32, l);
}

inline std::wstring to_wstring(std::u32string_view s32) {
  return to_wstring(s32.data(), s32.length());
}

inline std::string to_utf8(const wchar_t *sw, size_t l) {
  return detail::to_utf8_core(sw, l);
}

inline std::string to_utf8(std::wstring_view sw) {
  return to_utf8(sw.data(), sw.length());
}

inline std::u16string to_utf16(const wchar_t *sw, size_t l) {
  return detail::to_utf16_core(sw, l);
}

inline std::u16string to_utf16(std::wstring_view sw) {
  return to_utf16(sw.data(), sw.length());
}

inline std::u32string to_utf32(const wchar_t *sw, size_t l) {
  return detail::to_utf32_core(sw, l);
}

inline std::u32string to_utf32(std::wstring_view sw) {
  return to_utf32(sw.data(), sw.length());
}

}  // namespace unicode

// vim: et ts=2 sw=2 cin cino=\:0 ff=unix
