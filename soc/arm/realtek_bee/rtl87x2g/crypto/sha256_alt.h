#if !defined(CONFIG_MBEDTLS_CFG_FILE)
#include "mbedtls/mbedtls-config.h"
#else
#include CONFIG_MBEDTLS_CFG_FILE
#endif

#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#include <stddef.h>
#include <stdint.h>

/** SHA-256 input data was malformed. */
#define MBEDTLS_ERR_SHA256_BAD_INPUT_DATA                 -0x0074

#if defined(MBEDTLS_SHA256_ALT)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          The SHA-256 context structure.
 *
 *                 The structure is used both for SHA-256 and for SHA-224
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha256_starts().
 */
typedef struct mbedtls_sha256_context
{
    uint32_t MBEDTLS_PRIVATE(total)[2];          /*!< The number of Bytes processed.  */
    uint32_t MBEDTLS_PRIVATE(state)[8];          /*!< The intermediate digest state.  */
    unsigned char MBEDTLS_PRIVATE(buffer)[64];   /*!< The data block being processed. */
    int MBEDTLS_PRIVATE(is224);                  /*!< Determines which function to use:
                                     0: Use SHA-256, or 1: Use SHA-224. */
}
mbedtls_sha256_context;

#ifdef __cplusplus
}
#endif
#endif /* MBEDTLS_SHA256_ALT */
