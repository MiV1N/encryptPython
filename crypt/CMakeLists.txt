# ---------------------------------------------------------------------------
# OpenAES License
# ---------------------------------------------------------------------------
# Copyright (c) 2013, Nabil S. Al Ramli, www.nalramli.com
# All rights reserved.
#
# Reunis_crypt_distribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   - Reunis_crypt_distributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   - Reunis_crypt_distributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the unis_crypt_distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ---------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8.0)

project ( "unis_encrypt" )

add_subdirectory(openaes)

include_directories (
		${CMAKE_CURRENT_SOURCE_DIR}/openaes/inc
		${CMAKE_CURRENT_SOURCE_DIR}/inc
	)

set (LIB_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/unis_encrypt.c
	)

set (TEST_SRC
		${CMAKE_CURRENT_SOURCE_DIR}/unis_encrypt.c
		${CMAKE_CURRENT_SOURCE_DIR}/test_unis_encrypt.c
	)

set (SCAN_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/scan_encrypt.c
	${CMAKE_CURRENT_SOURCE_DIR}/unis_encrypt.c
)

add_executable( test_unis_encrypt ${TEST_SRC}  )
add_executable( scan_encrypt ${SCAN_SRC}  )
add_library(unis_encrypt SHARED  ${LIB_SRC})
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
target_link_libraries( test_unis_encrypt oaes_lib )
target_link_libraries( unis_encrypt oaes_lib )
target_link_libraries( scan_encrypt unis_encrypt)

# 设置执行文件和库文件的输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/unis_crypt_dist/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/unis_crypt_dist/lib)
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/unis_crypt_dist/)


# 安装目标文件到指定路径
install(TARGETS test_unis_encrypt unis_encrypt scan_encrypt
  RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# 安装头文件到指定路径
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc
  DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}
  FILES_MATCHING PATTERN "*.h"
)