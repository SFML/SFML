# Fix returning a pointer to stack memory by forcing libssh to make a copy of it instead
file(READ "${LIBSSH2_DIR}/src/mbedtls.c" LIBSSH2_MBEDTLS_CONTENTS)
string(REPLACE "return _libssh2_error(session, LIBSSH2_ERROR_FILE, buf);" "return _libssh2_error_flags(session, LIBSSH2_ERROR_FILE, buf, LIBSSH2_ERR_FLAG_DUP);" LIBSSH2_MBEDTLS_CONTENTS "${LIBSSH2_MBEDTLS_CONTENTS}")
file(WRITE "${LIBSSH2_DIR}/src/mbedtls.c" "${LIBSSH2_MBEDTLS_CONTENTS}")

# Replace FindMbedTLS.cmake with our own
file(READ "${MODULES_DIR}/FindMbedTLS.cmake" FINDMBEDTLS_CONTENTS)
file(WRITE "${LIBSSH2_DIR}/cmake/FindMbedTLS.cmake" "${FINDMBEDTLS_CONTENTS}")

# Make libssh2-config.cmake search for MbedTLS using CONFIG NO_DEFAULT_PATH
file(READ "${LIBSSH2_DIR}/cmake/libssh2-config.in.cmake" LIBSSH2_CONFIG_CONTENTS)
string(REPLACE "find_dependency(MbedTLS)" "find_dependency(MbedTLS CONFIG NO_DEFAULT_PATH)" LIBSSH2_CONFIG_CONTENTS "${LIBSSH2_CONFIG_CONTENTS}")
file(WRITE "${LIBSSH2_DIR}/cmake/libssh2-config.in.cmake" "${LIBSSH2_CONFIG_CONTENTS}")
