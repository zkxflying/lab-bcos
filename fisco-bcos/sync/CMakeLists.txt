#------------------------------------------------------------------------------
# Link libraries into main.cpp to generate executable binrary fisco-bcos
# ------------------------------------------------------------------------------
# This file is part of FISCO-BCOS.
#
# FISCO-BCOS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FISCO-BCOS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
#
# (c) 2016-2018 fisco-dev contributors.
#------------------------------------------------------------------------------
if(TESTS)

aux_source_directory(. SRC_LIST)

file(GLOB HEADERS "*.h")

add_executable(mini-sync ${SRC_LIST} ${HEADERS})

target_include_directories(mini-sync PRIVATE ${BOOST_INCLUDE_DIR})

target_link_libraries(mini-sync devcore)
target_link_libraries(mini-sync devcrypto)
target_link_libraries(mini-sync ethcore)
target_link_libraries(mini-sync p2p)
target_link_libraries(mini-sync blockverifier)
target_link_libraries(mini-sync sync)
target_link_libraries(mini-sync txpool)
target_link_libraries(mini-sync blockchain)
target_link_libraries(mini-sync consensus)
target_link_libraries(mini-sync ledger)
target_link_libraries(mini-sync initializer)

endif()
