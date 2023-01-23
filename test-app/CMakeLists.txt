cmake_minimum_required (VERSION 3.20)
project (test-app)

set (optee_DIR
  /d1/optee_qemu
)
include_directories (
  ${optee_DIR}/optee_client
  ../mock-ta
)
link_directories (
  ${optee_DIR}/optee_client
)
link_libraries (
  teec
)
add_executable (
  ${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
)
