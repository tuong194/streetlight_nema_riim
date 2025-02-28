# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "bootloader\\bootloader.bin"
  "bootloader\\bootloader.elf"
  "bootloader\\bootloader.map"
  "config\\sdkconfig.cmake"
  "config\\sdkconfig.h"
  "esp-idf\\esptool_py\\flasher_args.json.in"
  "esp-idf\\mbedtls\\x509_crt_bundle"
  "flash_app_args"
  "flash_bootloader_args"
  "flash_project_args"
  "flasher_args.json"
  "index.html.S"
  "index_intro.html.S"
  "index_setkey.html.S"
  "ldgen_libraries"
  "ldgen_libraries.in"
  "project_elf_src_esp32c3.c"
  "streetlight_nema_riim.bin"
  "streetlight_nema_riim.map"
  "x509_crt_bundle.S"
  )
endif()
