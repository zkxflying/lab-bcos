/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 */
/** @file MinerPrecompiled.h
 *  @author ancelmo
 *  @date 20180921
 */
#pragma once
#include "libblockverifier/ExecutiveContext.h"
#include "libstorage/CRUDPrecompiled.h"
namespace dev
{
namespace blockverifier
{
#if 0
contract Miner {
    function add(string) public constant returns();
    function remove(string) public returns();
}
{
    "b0c8f9dc": "add(string)",
    "80599e4b": "remove(string)"
}
#endif

/// \brief Sign of the miner is valid or not
const char* const NODE_TYPE = "type";
const char* const NODE_TYPE_MINER = "miner";
const char* const NODE_TYPE_OBSERVER = "observer";
const char* const NODE_KEY_NODEID = "node_id";
const char* const NODE_KEY_ENABLENUM = "enable_num";
const char* const PRI_KEY = "node";

class MinerPrecompiled : public CRUDPrecompiled
{
public:
    typedef std::shared_ptr<MinerPrecompiled> Ptr;

    virtual ~MinerPrecompiled(){};

    virtual bytes call(
        ExecutiveContext::Ptr context, bytesConstRef param, Address const& origin = Address());
};

}  // namespace blockverifier

}  // namespace dev
