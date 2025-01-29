#include <cstdint>
#include <iomanip>
#include <mlc/core/all.h>
#include <numeric>
#include <sstream>

namespace mlc {
namespace {

using mlc::core::TopoVisit;
using mlc::core::VisitFields;

mlc::Str Serialize(Any any);
Any Deserialize(const char *json_str, int64_t json_str_len);
Any JSONLoads(const char *json_str, int64_t json_str_len);
MLC_INLINE Any Deserialize(const char *json_str) { return Deserialize(json_str, -1); }
MLC_INLINE Any Deserialize(const Str &json_str) { return Deserialize(json_str->data(), json_str->size()); }
MLC_INLINE Any JSONLoads(const char *json_str) { return JSONLoads(json_str, -1); }
MLC_INLINE Any JSONLoads(const Str &json_str) { return JSONLoads(json_str->data(), json_str->size()); }

inline mlc::Str Serialize(Any any) {
  using mlc::base::TypeTraits;
  std::vector<const char *> type_keys;
  auto get_json_type_index = [type_key2index = std::unordered_map<const char *, int32_t>(),
                              &type_keys](const char *type_key) mutable -> int32_t {
    if (auto it = type_key2index.find(type_key); it != type_key2index.end()) {
      return it->second;
    }
    int32_t type_index = static_cast<int32_t>(type_key2index.size());
    type_key2index[type_key] = type_index;
    type_keys.push_back(type_key);
    return type_index;
  };
  using TObj2Idx = std::unordered_map<Object *, int32_t>;
  using TJsonTypeIndex = decltype(get_json_type_index);
  struct Emitter {
    MLC_INLINE void operator()(MLCTypeField *, const Any *any) { EmitAny(any); }
    // clang-format off
    MLC_INLINE void operator()(MLCTypeField *, ObjectRef *obj) { if (Object *v = obj->get()) EmitObject(v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<ObjectRef> *opt) { if (Object *v = opt->get()) EmitObject(v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<bool> *opt) { if (const bool *v = opt->get()) EmitBool(*v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<int64_t> *opt) { if (const int64_t *v = opt->get()) EmitInt(*v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<double> *opt) { if (const double *v = opt->get())  EmitFloat(*v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<DLDevice> *opt) { if (const DLDevice *v = opt->get()) EmitDevice(*v); else EmitNil(); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<DLDataType> *opt) { if (const DLDataType *v = opt->get()) EmitDType(*v); else EmitNil(); }
    // clang-format on
    MLC_INLINE void operator()(MLCTypeField *, bool *v) { EmitBool(*v); }
    MLC_INLINE void operator()(MLCTypeField *, int8_t *v) { EmitInt(static_cast<int64_t>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, int16_t *v) { EmitInt(static_cast<int64_t>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, int32_t *v) { EmitInt(static_cast<int64_t>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, int64_t *v) { EmitInt(static_cast<int64_t>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, float *v) { EmitFloat(static_cast<double>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, double *v) { EmitFloat(static_cast<double>(*v)); }
    MLC_INLINE void operator()(MLCTypeField *, DLDataType *v) { EmitDType(*v); }
    MLC_INLINE void operator()(MLCTypeField *, DLDevice *v) { EmitDevice(*v); }
    MLC_INLINE void operator()(MLCTypeField *, Optional<void *> *) {
      MLC_THROW(TypeError) << "Unserializable type: void *";
    }
    MLC_INLINE void operator()(MLCTypeField *, void **) { MLC_THROW(TypeError) << "Unserializable type: void *"; }
    MLC_INLINE void operator()(MLCTypeField *, const char **) {
      MLC_THROW(TypeError) << "Unserializable type: const char *";
    }
    inline void EmitNil() { (*os) << ", null"; }
    inline void EmitBool(bool v) { (*os) << ", " << (v ? "true" : "false"); }
    inline void EmitFloat(double v) { (*os) << ", " << std::fixed << std::setprecision(19) << v; }
    inline void EmitInt(int64_t v) {
      int32_t type_int = (*get_json_type_index)(TypeTraits<int64_t>::type_str);
      (*os) << ", [" << type_int << ", " << v << "]";
    }
    inline void EmitDevice(DLDevice v) {
      int32_t type_device = (*get_json_type_index)(TypeTraits<DLDevice>::type_str);
      (*os) << ", [" << type_device << ", " << ::mlc::base::TypeTraits<DLDevice>::__str__(v) << "]";
    }
    inline void EmitDType(DLDataType v) {
      int32_t type_dtype = (*get_json_type_index)(TypeTraits<DLDataType>::type_str);
      (*os) << ", [" << type_dtype << ", " << ::mlc::base::TypeTraits<DLDataType>::__str__(v) << "]";
    }
    inline void EmitAny(const Any *any) {
      int32_t type_index = any->type_index;
      if (type_index == kMLCNone) {
        EmitNil();
      } else if (type_index == kMLCBool) {
        EmitBool(any->operator bool());
      } else if (type_index == kMLCInt) {
        EmitInt(any->operator int64_t());
      } else if (type_index == kMLCFloat) {
        EmitFloat(any->operator double());
      } else if (type_index == kMLCDevice) {
        EmitDevice(any->operator DLDevice());
      } else if (type_index == kMLCDataType) {
        EmitDType(any->operator DLDataType());
      } else if (type_index >= kMLCStaticObjectBegin) {
        EmitObject(any->operator Object *());
      } else {
        MLC_THROW(TypeError) << "Cannot serialize type: " << Lib::GetTypeKey(type_index);
      }
    }
    inline void EmitObject(Object *obj) {
      if (!obj) {
        MLC_THROW(InternalError) << "This should never happen: null object pointer during EmitObject";
      }
      int32_t obj_idx = obj2index->at(obj);
      if (obj_idx == -1) {
        MLC_THROW(InternalError) << "This should never happen: topological ordering violated";
      }
      (*os) << ", " << obj_idx;
    }
    std::ostringstream *os;
    TJsonTypeIndex *get_json_type_index;
    const TObj2Idx *obj2index;
  };

  std::unordered_map<Object *, int32_t> topo_indices;
  std::ostringstream os;
  auto on_visit = [&topo_indices, get_json_type_index = &get_json_type_index, os = &os,
                   is_first_object = true](Object *object, MLCTypeInfo *type_info) mutable -> void {
    int32_t &topo_index = topo_indices[object];
    if (topo_index == 0) {
      topo_index = static_cast<int32_t>(topo_indices.size()) - 1;
    } else {
      MLC_THROW(InternalError) << "This should never happen: object already visited";
    }
    Emitter emitter{os, get_json_type_index, &topo_indices};
    if (is_first_object) {
      is_first_object = false;
    } else {
      os->put(',');
    }
    if (StrObj *str = object->TryCast<StrObj>()) {
      str->PrintEscape(*os);
      return;
    }
    (*os) << '[' << (*get_json_type_index)(type_info->type_key);
    if (UListObj *list = object->TryCast<UListObj>()) {
      for (Any &any : *list) {
        emitter(nullptr, &any);
      }
    } else if (UDictObj *dict = object->TryCast<UDictObj>()) {
      for (auto &kv : *dict) {
        emitter(nullptr, &kv.first);
        emitter(nullptr, &kv.second);
      }
    } else if (object->IsInstance<FuncObj>() || object->IsInstance<ErrorObj>()) {
      MLC_THROW(TypeError) << "Unserializable type: " << object->GetTypeKey();
    } else {
      VisitFields(object, type_info, emitter);
    }
    os->put(']');
  };
  os << "{\"values\": [";
  if (any.type_index >= kMLCStaticObjectBegin) { // Topological the objects according to dependency
    TopoVisit(any.operator Object *(), nullptr, on_visit);
  } else if (any.type_index == kMLCNone) {
    os << "null";
  } else if (any.type_index == kMLCBool) {
    bool v = any.operator bool();
    os << (v ? "true" : "false");
  } else if (any.type_index == kMLCInt) {
    int32_t type_int = get_json_type_index(TypeTraits<int64_t>::type_str);
    int64_t v = any;
    os << "[" << type_int << ", " << v << "]";
  } else if (any.type_index == kMLCFloat) {
    double v = any;
    os << v;
  } else if (any.type_index == kMLCDevice) {
    int32_t type_device = get_json_type_index(TypeTraits<DLDevice>::type_str);
    DLDevice v = any;
    os << "[" << type_device << ", \"" << TypeTraits<DLDevice>::__str__(v) << "\"]";
  } else if (any.type_index == kMLCDataType) {
    int32_t type_dtype = get_json_type_index(TypeTraits<DLDataType>::type_str);
    DLDataType v = any;
    os << "[" << type_dtype << ", \"" << TypeTraits<DLDataType>::__str__(v) << "\"]";
  } else {
    MLC_THROW(TypeError) << "Cannot serialize type: " << Lib::GetTypeKey(any.type_index);
  }
  os << "], \"type_keys\": [";
  for (size_t i = 0; i < type_keys.size(); ++i) {
    if (i > 0) {
      os << ", ";
    }
    os << '"' << type_keys[i] << '\"';
  }
  os << "]}";
  return os.str();
}

inline Any Deserialize(const char *json_str, int64_t json_str_len) {
  // Step 0. Parse JSON string
  UDict json_obj = JSONLoads(json_str, json_str_len);
  // Step 1. type_key => constructors
  UList type_keys = json_obj->at("type_keys");
  std::vector<FuncObj *> constructors;
  constructors.reserve(type_keys.size());
  for (Str type_key : type_keys) {
    int32_t type_index = Lib::GetTypeIndex(type_key->data());
    FuncObj *func = Lib::_init(type_index);
    constructors.push_back(func);
  }
  auto invoke_init = [&constructors](UList args) {
    int32_t json_type_index = args[0];
    Any ret;
    ::mlc::base::FuncCall(constructors.at(json_type_index), static_cast<int32_t>(args.size()) - 1, args->data() + 1,
                          &ret);
    return ret;
  };
  // Step 2. Translate JSON object to objects
  UList values = json_obj->at("values");
  for (int64_t i = 0; i < values->size(); ++i) {
    Any obj = values[i];
    if (obj.type_index == kMLCList) {
      UList list = obj.operator UList();
      int32_t json_type_index = list[0];
      for (int64_t j = 1; j < list.size(); ++j) {
        Any arg = list[j];
        if (arg.type_index == kMLCInt) {
          int64_t k = arg;
          if (k < i) {
            list[j] = values[k];
          } else {
            MLC_THROW(ValueError) << "Invalid reference when parsing type `" << type_keys[json_type_index]
                                  << "`: referring #" << k << " at #" << i << ". v = " << obj;
          }
        } else if (arg.type_index == kMLCList) {
          list[j] = invoke_init(arg.operator UList());
        } else if (arg.type_index == kMLCStr || arg.type_index == kMLCBool || arg.type_index == kMLCFloat ||
                   arg.type_index == kMLCNone) {
          // Do nothing
        } else {
          MLC_THROW(ValueError) << "Unexpected value: " << arg;
        }
      }
      values[i] = invoke_init(list);
    } else if (obj.type_index == kMLCInt) {
      int32_t k = obj;
      values[i] = values[k];
    } else if (obj.type_index == kMLCStr) {
      // Do nothing
      // TODO: how about kMLCBool, kMLCFloat, kMLCNone?
    } else {
      MLC_THROW(ValueError) << "Unexpected value: " << obj;
    }
  }
  return values->back();
}

inline Any JSONLoads(const char *json_str, int64_t json_str_len) {
  struct JSONParser {
    Any Parse() {
      SkipWhitespace();
      Any result = ParseValue();
      SkipWhitespace();
      if (i != json_str_len) {
        MLC_THROW(ValueError) << "JSON parsing failure at position " << i
                              << ": Extra data after valid JSON. JSON string: " << json_str;
      }
      return result;
    }

    void ExpectChar(char c) {
      if (json_str[i] == c) {
        ++i;
      } else {
        MLC_THROW(ValueError) << "JSON parsing failure at position " << i << ": Expected '" << c << "' but got '"
                              << json_str[i] << "'. JSON string: " << json_str;
      }
    }

    char PeekChar() { return i < json_str_len ? json_str[i] : '\0'; }

    void SkipWhitespace() {
      while (i < json_str_len && std::isspace(json_str[i])) {
        ++i;
      }
    }

    void ExpectString(const char *expected, int64_t len) {
      if (i + len <= json_str_len && std::strncmp(json_str + i, expected, len) == 0) {
        i = i + len;
      } else {
        MLC_THROW(ValueError) << "JSON parsing failure at position " << i << ": Expected '" << expected
                              << ". JSON string: " << json_str;
      }
    }

    Any ParseNull() {
      ExpectString("null", 4);
      return Any(nullptr);
    }

    Any ParseBoolean() {
      if (PeekChar() == 't') {
        ExpectString("true", 4);
        return Any(true);
      } else {
        ExpectString("false", 5);
        return Any(false);
      }
    }

    Any ParseNumber() {
      int64_t start = i;
      // Identify the end of the numeric sequence
      while (i < json_str_len) {
        char c = json_str[i];
        if (c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-' || std::isdigit(c)) {
          ++i;
        } else {
          break;
        }
      }
      std::string num_str(json_str + start, i - start);
      std::size_t pos = 0;
      try {
        // Attempt to parse as integer
        int64_t int_value = std::stoll(num_str, &pos);
        if (pos == num_str.size()) {
          i = start + static_cast<int64_t>(pos); // Update the main index
          return Any(int_value);
        }
      } catch (const std::invalid_argument &) { // Not an integer, proceed to parse as double
      } catch (const std::out_of_range &) {     // Integer out of range, proceed to parse as double
      }
      try {
        // Attempt to parse as double
        double double_value = std::stod(num_str, &pos);
        if (pos == num_str.size()) {
          i = start + pos; // Update the main index
          return Any(double_value);
        }
      } catch (const std::invalid_argument &) {
      } catch (const std::out_of_range &) {
      }
      MLC_THROW(ValueError) << "JSON parsing failure at position " << i
                            << ": Invalid number format. JSON string: " << json_str;
      MLC_UNREACHABLE();
    }

    Any ParseStr() {
      ExpectChar('"');
      std::ostringstream oss;
      while (true) {
        if (i >= json_str_len) {
          MLC_THROW(ValueError) << "JSON parsing failure at position " << i
                                << ": Unterminated string. JSON string: " << json_str;
        }
        char c = json_str[i++];
        if (c == '"') {
          // End of string
          return Any(Str(oss.str()));
        } else if (c == '\\') {
          // Handle escape sequences
          if (i >= json_str_len) {
            MLC_THROW(ValueError) << "JSON parsing failure at position " << i
                                  << ": Incomplete escape sequence. JSON string: " << json_str;
          }
          char next = json_str[i++];
          switch (next) {
          case 'n':
            oss << '\n';
            break;
          case 't':
            oss << '\t';
            break;
          case 'r':
            oss << '\r';
            break;
          case '\\':
            oss << '\\';
            break;
          case '"':
            oss << '\"';
            break;
          case 'x': {
            if (i + 1 < json_str_len && std::isxdigit(json_str[i]) && std::isxdigit(json_str[i + 1])) {
              int32_t value = std::stoi(std::string(json_str + i, 2), nullptr, 16);
              oss << static_cast<char>(value);
              i += 2;
            } else {
              MLC_THROW(ValueError) << "Invalid hexadecimal escape sequence at position " << i - 2
                                    << " in string: " << json_str;
            }
            break;
          }
          case 'u': {
            if (i + 3 < json_str_len && std::isxdigit(json_str[i]) && std::isxdigit(json_str[i + 1]) &&
                std::isxdigit(json_str[i + 2]) && std::isxdigit(json_str[i + 3])) {
              int32_t codepoint = std::stoi(std::string(json_str + i, 4), nullptr, 16);
              if (codepoint <= 0x7F) {
                // 1-byte UTF-8
                oss << static_cast<char>(codepoint);
              } else if (codepoint <= 0x7FF) {
                // 2-byte UTF-8
                oss << static_cast<char>(0xC0 | (codepoint >> 6));
                oss << static_cast<char>(0x80 | (codepoint & 0x3F));
              } else {
                // 3-byte UTF-8
                oss << static_cast<char>(0xE0 | (codepoint >> 12));
                oss << static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                oss << static_cast<char>(0x80 | (codepoint & 0x3F));
              }
              i += 4;
            } else {
              MLC_THROW(ValueError) << "Invalid Unicode escape sequence at position " << i - 2
                                    << " in string: " << json_str;
            }
            break;
          }
          default:
            // Unrecognized escape sequence, interpret literally
            oss << next;
            break;
          }
        } else {
          // Regular character, copy as-is
          oss << c;
        }
      }
    }

    UList ParseArray() {
      UList arr;
      ExpectChar('[');
      SkipWhitespace();
      if (PeekChar() == ']') {
        ExpectChar(']');
        return arr;
      }
      while (true) {
        SkipWhitespace();
        arr.push_back(ParseValue());
        SkipWhitespace();
        if (PeekChar() == ']') {
          ExpectChar(']');
          return arr;
        }
        ExpectChar(',');
      }
    }

    Any ParseObject() {
      UDict obj;
      ExpectChar('{');
      SkipWhitespace();
      if (PeekChar() == '}') {
        ExpectChar('}');
        return Any(obj);
      }
      while (true) {
        SkipWhitespace();
        Any key = ParseStr();
        SkipWhitespace();
        ExpectChar(':');
        SkipWhitespace();
        Any value = ParseValue();
        obj[key] = value;
        SkipWhitespace();
        if (PeekChar() == '}') {
          ExpectChar('}');
          return Any(obj);
        }
        ExpectChar(',');
      }
    }

    Any ParseValue() {
      SkipWhitespace();
      char c = PeekChar();
      if (c == '"') {
        return ParseStr();
      } else if (c == '{') {
        return ParseObject();
      } else if (c == '[') {
        return ParseArray();
      } else if (c == 'n') {
        return ParseNull();
      } else if (c == 't' || c == 'f') {
        return ParseBoolean();
      } else if (std::isdigit(c) || c == '-') {
        return ParseNumber();
      } else {
        MLC_THROW(ValueError) << "JSON parsing failure at position " << i << ": Unexpected character: " << c
                              << ". JSON string: " << json_str;
      }
      MLC_UNREACHABLE();
    }
    int64_t i;
    int64_t json_str_len;
    const char *json_str;
  };
  if (json_str_len < 0) {
    json_str_len = static_cast<int64_t>(std::strlen(json_str));
  }
  return JSONParser{0, json_str_len, json_str}.Parse();
}

// --------------------------------------------------------------------------
// 1) Global constants for Base64 encoding/decoding, endianness and byte-swap
// --------------------------------------------------------------------------
static const uint64_t kMLCTensorMagic = 0xDD5E40F096B4A13F;
static const char kBase64EncTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const std::array<uint8_t, 256> kBase64DecTable = []() {
  std::array<uint8_t, 256> ret;
  // Initialize all entries to 0xFF (invalid)
  for (int i = 0; i < 256; ++i) {
    ret[i] = 0xFF;
  }
  // Fill valid mappings: 'A'-'Z', 'a'-'z', '0'-'9', '+' and '/'
  for (int i = 0; i < 64; ++i) {
    uint8_t c = static_cast<uint8_t>(kBase64EncTable[i]);
    ret[c] = static_cast<uint8_t>(i);
  }
  return ret;
}();
static const bool kIsLittleEndian = []() {
  const uint16_t val = 0x0001;
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&val);
  return *bytes == 0x01;
}();

void ByteSwapInPlace(uint8_t *data, size_t elem_size, size_t count) {
  // Byte-swap in place for elements each having size elem_size.
  // e.g., to swap 64-bit, pass elem_size=8, to swap 32-bit, pass elem_size=4, etc.
  for (size_t i = 0; i < count; ++i) {
    uint8_t *start = data + i * elem_size;
    // Reverse the elem_size bytes
    for (size_t j = 0; j < elem_size / 2; ++j) {
      std::swap(start[j], start[elem_size - 1 - j]);
    }
  }
}

inline void DefaultDLTensorDeleter(DLManagedTensor *arg) {
  // TODO: delete this
  if (!arg)
    return;
  // free the data
  free(arg->dl_tensor.data);
  // free the shape
  free(arg->dl_tensor.shape);
  // free the DLManagedTensor itself
  delete arg;
}

// ----------------------------------------------------------------------
// 2) Base64 encoding/decoding
// ----------------------------------------------------------------------

void Base64Encode(const uint8_t *data, size_t len, std::ostream *os) {
  constexpr int BITS_PER_CHAR = 6;
  // out.reserve(((len + 2) / 3) * 4);
  for (size_t i = 0; i < len; i += 3) {
    // Collect up to 3 bytes into a 24-bit chunk
    uint32_t chunk = 0;
    int bytes_in_chunk = 0;
    for (int j = 0; j < 3; ++j) {
      if (i + j < len) {
        chunk <<= 8;
        chunk |= data[i + j];
        bytes_in_chunk++;
      } else {
        chunk <<= 8; // pad with zero
      }
    }
    // chunk now has up to 24 bits of actual data, left-aligned
    // We emit 4 Base64 chars, but some might become '=' padding
    // Based on how many raw bytes we actually had.
    for (int k = 0; k < 4; ++k) {
      // For each 6-bit group: (from left to right in chunk)
      int shift = 18 - (k * BITS_PER_CHAR);
      uint32_t index = (chunk >> shift) & 0x3F;
      if (k <= bytes_in_chunk) {
        // For 3 raw bytes, k goes 0..3 => all real
        // For 2 raw bytes, k goes 0..2 => 3rd is '='
        // For 1 raw byte, k goes 0..1 => 2nd/3rd are '='
        os->put(kBase64EncTable[index]);
      } else {
        os->put('=');
      }
    }
  }
}

std::vector<uint8_t> Base64Decode(const std::string &input) {
  size_t in_len = input.size();
  if (in_len % 4 != 0) {
    MLC_THROW(ValueError) << "Base64Decode: Input length not multiple of 4.";
  }
  // Maximum possible output size: (in_len / 4) * 3 and final bytes may be padding
  std::vector<uint8_t> out;
  out.reserve((in_len / 4) * 3);
  for (size_t i = 0; i < in_len; i += 4) {
    // Each block of 4 chars -> up to 3 bytes
    uint32_t accum = 0;
    int valid_chars = 0;
    // Read 4 base64 characters
    for (int j = 0; j < 4; ++j) {
      uint8_t c = static_cast<uint8_t>(input[i + j]);
      if (c != '=') {
        // '=' indicates padding, do not shift in any bits
        // we still do the loop, but no accumulation
        if (uint8_t v = kBase64DecTable[c]; v != 0xFF) {
          accum = (accum << 6) | v;
          valid_chars++;
        }
        MLC_THROW(ValueError) << "Base64Decode: Invalid character in input.";
        MLC_UNREACHABLE();
      }
    }
    int total_bits = valid_chars * 6;
    accum <<= (24 - total_bits);
    int total_bytes = total_bits / 8;
    for (int b = 0; b < total_bytes; ++b) {
      uint8_t byte_val = static_cast<uint8_t>((accum >> (16 - 8 * b)) & 0xFF);
      out.push_back(byte_val);
    }
  }
  return out;
}

// ----------------------------------------------------------------------
// 3) SaveDLPack / LoadDLPack
// ----------------------------------------------------------------------

struct Converter {
  struct B1 {
    uint8_t v0;
  };
  struct B2 {
    uint8_t v0;
    uint8_t v1;
  };
  struct B4 {
    uint8_t v0;
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
  };
  struct B8 {
    uint8_t v0;
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
    uint8_t v4;
    uint8_t v5;
    uint8_t v6;
    uint8_t v7;
  };
  template <typename T> static void AppendByte1(T val, std::vector<uint8_t> *buffer) {
    union {
      T v;
      B1 b;
    } v;
    static_assert(sizeof(decltype(v)) == 1, "`T` must be 1 byte wide.");
    v.v = val;
    buffer->push_back(v.b.v0);
  }
  template <typename T> static void AppendByte2(T val, std::vector<uint8_t> *buffer) {
    union {
      T v;
      B2 b;
    } v;
    static_assert(sizeof(decltype(v)) == 2, "`T` must be 2 bytes wide.");
    v.v = val;
    if (kIsLittleEndian) {
      buffer->push_back(v.b.v0);
      buffer->push_back(v.b.v1);
    } else {
      buffer->push_back(v.b.v1);
      buffer->push_back(v.b.v0);
    }
  }
  template <typename T> static void AppendByte4(T val, std::vector<uint8_t> *buffer) {
    union {
      T v;
      B4 b;
    } v;
    static_assert(sizeof(decltype(v)) == 4, "`T` must be 4 bytes wide.");
    v.v = val;
    if (kIsLittleEndian) {
      buffer->push_back(v.b.v0);
      buffer->push_back(v.b.v1);
      buffer->push_back(v.b.v2);
      buffer->push_back(v.b.v3);
    } else {
      buffer->push_back(v.b.v3);
      buffer->push_back(v.b.v2);
      buffer->push_back(v.b.v1);
      buffer->push_back(v.b.v0);
    }
  }
  template <typename T> static void AppendByte8(T val, std::vector<uint8_t> *buffer) {
    union {
      T v;
      B8 b;
    } v;
    static_assert(sizeof(decltype(v)) == 8, "`T` must be 8 bytes wide.");
    v.v = val;
    if (kIsLittleEndian) {
      buffer->push_back(v.b.v0);
      buffer->push_back(v.b.v1);
      buffer->push_back(v.b.v2);
      buffer->push_back(v.b.v3);
      buffer->push_back(v.b.v4);
      buffer->push_back(v.b.v5);
      buffer->push_back(v.b.v6);
      buffer->push_back(v.b.v7);
    } else {
      buffer->push_back(v.b.v7);
      buffer->push_back(v.b.v6);
      buffer->push_back(v.b.v5);
      buffer->push_back(v.b.v4);
      buffer->push_back(v.b.v3);
      buffer->push_back(v.b.v2);
      buffer->push_back(v.b.v1);
      buffer->push_back(v.b.v0);
    }
  }
};

std::vector<uint8_t> DLTensorToBytes(const DLTensor *src) {
  if (src->device.device_type != kDLCPU || src->strides != nullptr) {
    MLC_THROW(ValueError) << "SaveDLPack: Only CPU tensor without strides is supported.";
  }
  int32_t ndim = src->ndim;
  int64_t numel = std::accumulate(src->shape, src->shape + ndim, static_cast<int64_t>(1), std::multiplies<int64_t>());
  int64_t elem_size = ((static_cast<int32_t>(src->dtype.bits) + 7) / 8) * static_cast<int32_t>(src->dtype.lanes);
  // Layout: all in little-endian
  //   8 bytes: header
  //   4 bytes: ndim
  //   4 bytes: dtype
  //   8 bytes * ndim: shape
  //   raw data
  std::vector<uint8_t> buffer;
  buffer.reserve(8 + 4 + 4 + 8 * ndim + numel * elem_size);
  Converter::AppendByte8(static_cast<uint64_t>(kMLCTensorMagic), &buffer);
  Converter::AppendByte4(ndim, &buffer);
  Converter::AppendByte4(src->dtype, &buffer);
  for (int i = 0; i < ndim; ++i) {
    Converter::AppendByte8(src->shape[i], &buffer);
  }
  const uint8_t *data_ptr = static_cast<const uint8_t *>(src->data);
  buffer.insert(buffer.end(), data_ptr, data_ptr + numel * elem_size);
  if (!kIsLittleEndian && elem_size > 1) { // we need to swap each element
    ByteSwapInPlace(buffer.data() + 16 + 8 * ndim, size_t(elem_size), size_t(numel));
  }
  return buffer;
}

/*!
 * \brief Load a CPU, stride-free DLPack (DLManagedTensor) from a base64-encoded
 *        little-endian binary payload in the given std::istream.
 *
 * Steps:
 *   1) We read the entire stream into a string.
 *   2) Base64-decode into a raw buffer.
 *   3) Byte-swap multi-byte fields if we are on a big-endian machine.
 *   4) Parse fields, allocate a brand-new DLManagedTensor, copy shape and data.
 *   5) Return it. Caller can handle or store the deleter as desired.
 */
inline DLManagedTensor *LoadDLPack(std::istream *is) {
  // // Read entire base64 text
  // std::string encoded((std::istreambuf_iterator<char>(*is)), std::istreambuf_iterator<char>());
  // if (encoded.empty()) {
  //   throw std::runtime_error("LoadDLPack: Empty input stream.");
  // }
  // // Decode from base64
  // std::vector<uint8_t> buffer = Base64Decode(encoded);

  // We'll define a reader that picks bytes from 'buffer'
  size_t offset = 0;
  auto read_le = [&](auto &out_val) {
    using T = decltype(out_val);
    if (offset + sizeof(T) > buffer.size()) {
      throw std::runtime_error("LoadDLPack: Unexpected EOF in buffer.");
    }
    std::memcpy(&out_val, &buffer[offset], sizeof(T));
    offset += sizeof(T);
    // if we are big-endian, reverse the bytes
    if (!kIsLittleEndian) {
      uint8_t *ptr = reinterpret_cast<uint8_t *>(&out_val);
      std::reverse(ptr, ptr + sizeof(T));
    }
  };

  // 1) read header (uint64_t), reserved (uint64_t)
  uint64_t header = 0, reserved = 0;
  read_le(header);
  read_le(reserved);
  if (header != kMLCTensorMagic) {
    throw std::runtime_error("LoadDLPack: Magic number mismatch.");
  }
  // 2) read dev_type(4), dev_id(4), ndim(4)
  int32_t dev_type = 0, dev_id = 0, ndim = 0;
  read_le(dev_type);
  read_le(dev_id);
  read_le(ndim);

  if (dev_type != kDLCPU || dev_id != 0) {
    throw std::runtime_error("LoadDLPack: Only CPU device_type=1, device_id=0 is supported.");
  }
  if (ndim <= 0) {
    throw std::runtime_error("LoadDLPack: Invalid ndim <= 0.");
  }

  // 3) read dtype_code(4), dtype_bits(4), dtype_lanes(4)
  int32_t dtype_code = 0, dtype_bits = 0, dtype_lanes = 0;
  read_le(dtype_code);
  read_le(dtype_bits);
  read_le(dtype_lanes);

  // 4) read shape (ndim * 8 bytes each)
  std::vector<int64_t> shape_array(ndim);
  for (int i = 0; i < ndim; ++i) {
    int64_t dimval = 0;
    read_le(dimval);
    shape_array[i] = dimval;
  }

  // 5) read data_byte_size (8 bytes)
  int64_t data_byte_size = 0;
  read_le(data_byte_size);
  if (data_byte_size < 0) {
    throw std::runtime_error("LoadDLPack: data_byte_size < 0? Corrupt file.");
  }
  if (offset + static_cast<size_t>(data_byte_size) > buffer.size()) {
    throw std::runtime_error("LoadDLPack: Not enough bytes for raw data.");
  }

  // 6) read the raw data
  const uint8_t *raw_data_start = &buffer[offset];
  offset += data_byte_size;

  // We also might need to swap each element if we are big-endian and dtype_bits>8
  // We'll do that in a newly allocated array.
  uint8_t *data_ptr = reinterpret_cast<uint8_t *>(malloc(data_byte_size));
  if (!data_ptr) {
    throw std::runtime_error("LoadDLPack: failed to allocate data buffer.");
  }
  std::memcpy(data_ptr, raw_data_start, data_byte_size);

  // If big-endian and type_size>1, swap in place
  int64_t type_size = (dtype_bits / 8) * dtype_lanes;
  int64_t total_elems = 1;
  for (auto dimval : shape_array) {
    total_elems *= dimval;
  }
  if (!kIsLittleEndian && type_size > 1) {
    ByteSwapInPlace(data_ptr, size_t(type_size), size_t(total_elems));
  }
  // Now create a new DLManagedTensor
  DLManagedTensor *ret = new DLManagedTensor();
  memset(ret, 0, sizeof(*ret)); // zero fill
  // Fill ret->dl_tensor
  ret->dl_tensor.device.device_type = static_cast<DLDeviceType>(kDLCPU);
  ret->dl_tensor.device.device_id = 0;
  ret->dl_tensor.ndim = ndim;
  ret->dl_tensor.dtype.code = static_cast<uint8_t>(dtype_code);
  ret->dl_tensor.dtype.bits = static_cast<uint8_t>(dtype_bits);
  ret->dl_tensor.dtype.lanes = static_cast<uint16_t>(dtype_lanes);
  ret->dl_tensor.shape = reinterpret_cast<int64_t *>(malloc(sizeof(int64_t) * ndim));
  if (!ret->dl_tensor.shape) {
    free(data_ptr);
    delete ret;
    throw std::runtime_error("LoadDLPack: failed to allocate shape array.");
  }
  for (int i = 0; i < ndim; ++i) {
    ret->dl_tensor.shape[i] = shape_array[i];
  }
  ret->dl_tensor.strides = nullptr; // stride-free
  ret->dl_tensor.byte_offset = 0;
  ret->dl_tensor.data = data_ptr;
  // Provide a default deleter
  ret->manager_ctx = nullptr; // optional
  ret->deleter = &DefaultDLTensorDeleter;
  return ret;
}

MLC_REGISTER_FUNC("mlc.core.JSONLoads").set_body([](AnyView json_str) {
  if (json_str.type_index == kMLCRawStr) {
    return ::mlc::core::JSONLoads(json_str.operator const char *());
  } else {
    ::mlc::Str str = json_str;
    return ::mlc::core::JSONLoads(str);
  }
});
MLC_REGISTER_FUNC("mlc.core.JSONSerialize").set_body(::mlc::core::Serialize);
MLC_REGISTER_FUNC("mlc.core.JSONDeserialize").set_body([](AnyView json_str) {
  if (json_str.type_index == kMLCRawStr) {
    return ::mlc::core::Deserialize(json_str.operator const char *());
  } else {
    return ::mlc::core::Deserialize(json_str.operator ::mlc::Str());
  }
});
} // namespace
} // namespace mlc

namespace mlc {

Any JSONLoads(AnyView json_str) {
  if (json_str.type_index == kMLCRawStr) {
    return JSONLoads(json_str.operator const char *());
  } else {
    return JSONLoads(json_str.operator Str());
  }
}

Any JSONDeserialize(AnyView json_str) {
  if (json_str.type_index == kMLCRawStr) {
    return Deserialize(json_str.operator const char *());
  } else {
    return Deserialize(json_str.operator ::mlc::Str());
  }
}

Str JSONSerialize(AnyView source) { return Serialize(source); }

} // namespace mlc
