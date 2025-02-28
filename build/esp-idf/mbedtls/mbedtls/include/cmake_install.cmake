# Install script for directory: C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/streetlight_nema_riim")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/My_Doc/ESP_IDF/tool_v5.3.2/tools/riscv32-esp-elf/esp-13.2.0_20240530/riscv32-esp-elf/bin/riscv32-esp-elf-objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_dependencies.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "C:/My_Doc/ESP_IDF/v5.3.2/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

