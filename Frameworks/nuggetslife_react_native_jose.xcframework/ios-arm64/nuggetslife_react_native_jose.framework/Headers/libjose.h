#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum ContentEncryptionAlgorithm {
  A128gcm,
  A192gcm,
  A256gcm,
  A128cbcHs256,
  A192cbcHs384,
  A256cbcHs512,
} ContentEncryptionAlgorithm;

typedef enum KeyEncryptionAlgorithm {
  Dir,
  EcdhEs,
  EcdhEsA128kw,
  EcdhEsA192kw,
  EcdhEsA256kw,
  Rsa1_5,
  RsaOaep,
  RsaOaep256,
  RsaOaep384,
  RsaOaep512,
  Pbes2Hs256A128kw,
  Pbes2Hs384A192kw,
  Pbes2Hs512A256kw,
  A128kw,
  A192kw,
  A256kw,
  A128gcmkw,
  A192gcmkw,
  A256gcmkw,
} KeyEncryptionAlgorithm;

typedef enum NamedCurve {
  P256,
  P384,
  P521,
  Secp256k1,
  Ed25519,
  Ed448,
  X25519,
  X448,
} NamedCurve;

typedef enum SigningAlgorithm {
  Es256,
  Es384,
  Es512,
  Es256k,
  Eddsa,
  Hs256,
  Hs384,
  Hs512,
  Rs256,
  Rs384,
  Rs512,
  Ps256,
  Ps384,
  Ps512,
} SigningAlgorithm;

typedef struct JsonString {
  const char *ptr;
} JsonString;

/**
 * Used for receiving a ByteBuffer from C that was allocated by either C or Rust.
 * If Rust allocated, then the outgoing struct is `ffi_support::ByteBuffer`
 * Caller is responsible for calling free where applicable.
 *
 * C will not notice a difference and can use the same struct
 */
typedef struct ByteArray {
  uintptr_t length;
  const uint8_t *data;
} ByteArray;

void ffi_jose_free_json_string(struct JsonString json_string);

/**
 * Generate JWK as JSON String
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_generate_key_pair_jwk(enum NamedCurve named_curve, struct JsonString *json_string);

/**
 * Generate KeyPair as JSON String
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_generate_key_pair(enum NamedCurve named_curve, struct JsonString *json_string);

/**
 * Encrypt message
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_encrypt(enum ContentEncryptionAlgorithm enc,
                         struct ByteArray key,
                         struct ByteArray iv,
                         struct ByteArray message,
                         struct ByteArray aad,
                         struct JsonString *json_string);

/**
 * Decrypt message
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_decrypt(enum ContentEncryptionAlgorithm enc,
                         struct ByteArray key,
                         struct ByteArray ciphertext,
                         struct ByteArray iv,
                         struct ByteArray tag,
                         struct ByteArray aad,
                         struct JsonString *json_string);

/**
 * General Encrypt JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_general_encrypt_json(enum KeyEncryptionAlgorithm alg,
                                      enum ContentEncryptionAlgorithm enc,
                                      struct ByteArray payload,
                                      struct ByteArray recipients,
                                      struct JsonString *json_string,
                                      bool didcomm);

/**
 * Decrypt JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_decrypt_json(struct ByteArray jwe,
                              struct ByteArray jwk,
                              struct JsonString *json_string);

/**
 * Compact D Sign JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_compact_sign_json(enum SigningAlgorithm alg,
                                   struct ByteArray payload,
                                   struct ByteArray jwk,
                                   struct JsonString *json_string,
                                   bool didcomm);

/**
 * Verify Compact JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_compact_json_verify(struct ByteArray jws,
                                     struct ByteArray jwk,
                                     struct JsonString *json_string);

/**
 * Flattened Sign JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_flattened_sign_json(enum SigningAlgorithm alg,
                                     struct ByteArray payload,
                                     struct ByteArray jwk,
                                     struct JsonString *json_string,
                                     bool didcomm);

/**
 * Verify Flattened / General JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_json_verify(struct ByteArray jws,
                             struct ByteArray jwk,
                             struct JsonString *json_string);

/**
 * General Sign JSON
 *
 * # SAFETY
 * The `json_string.ptr` pointer needs to follow the same safety requirements
 * as Rust's `std::ffi::CStr::from_ptr`
 */
int32_t ffi_jose_general_sign_json(struct ByteArray payload,
                                   struct ByteArray jwks,
                                   struct JsonString *json_string,
                                   bool didcomm);
