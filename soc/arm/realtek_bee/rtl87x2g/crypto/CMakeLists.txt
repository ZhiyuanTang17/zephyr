zephyr_include_directories(.)
#The sha256_alt.c file still relies on internal mbedtls header files
#because the SHA-224 implementation uses mbedtls source code instead of a hardware acceleration version.
zephyr_include_directories(${WEST_TOPDIR}/modules/crypto/mbedtls/library)
zephyr_include_directories(${WEST_TOPDIR}/modules/crypto/mbedtls/include)
zephyr_library_sources(sha256_alt.c)
