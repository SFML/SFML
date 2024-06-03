# prevent clang-tidy from running on our dependencies by removing the entries from compile_commands.json
file(READ "${PROJECT_BINARY_DIR}/compile_commands.json" COMPILE_COMMANDS_JSON_CONTENTS)
string(REGEX REPLACE "(,\n)?{[^{]*\"output\": \"[^}]*_deps[^}]*}" "" COMPILE_COMMANDS_JSON_CONTENTS "${COMPILE_COMMANDS_JSON_CONTENTS}")
file(WRITE "${PROJECT_BINARY_DIR}/compile_commands.json" "${COMPILE_COMMANDS_JSON_CONTENTS}")
