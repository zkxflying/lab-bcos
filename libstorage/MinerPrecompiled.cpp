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
/** @file MinerPrecompiled.cpp
 *  @author ancelmo
 *  @date 20180921
 */
#include "MinerPrecompiled.h"
#include <libdevcore/easylog.h>
#include <libethcore/ABI.h>
#include <libstorage/EntriesPrecompiled.h>
#include <libstorage/TableFactoryPrecompiled.h>
#include <boost/lexical_cast.hpp>
using namespace dev;
using namespace dev::blockverifier;

bytes MinerPrecompiled::call(
    ExecutiveContext::Ptr context, bytesConstRef param, Address const& origin)
{
    STORAGE_LOG(TRACE) << "this: " << this << " call CRUD:" << toHex(param);


    uint32_t func = getParamFunc(param);
    bytesConstRef data = getParamData(param);

    STORAGE_LOG(DEBUG) << "func:" << std::hex << func;

    dev::eth::ContractABI abi;
    bytes out;
    const std::string key("miner");
    switch (func)
    {
    case 0xb0c8f9dc:
    {  // add(string)
        std::string nodeID;
        abi.abiOut(data, nodeID);
        if (nodeID.size() != 128u)
        {
            STORAGE_LOG(DEBUG) << "NodeID length error. " << nodeID;
            break;
        }
        storage::Table::Ptr table = openTable(context, "_sys_miners_");
        if (table.get())
        {
            auto condition = table->newCondition();
            condition->EQ(NODE_KEY_NODEID, nodeID);
            auto entries = table->select(PRI_KEY, condition);
            auto entry = table->newEntry();
            entry->setField(NODE_TYPE, NODE_TYPE_MINER);
            entry->setField("name", PRI_KEY);

            if (entries->size() == 0u)
            {
                entry->setField(NODE_KEY_ENABLENUM,
                    boost::lexical_cast<std::string>(context->blockInfo().number + 1));
                entry->setField(NODE_KEY_NODEID, nodeID);
                table->insert(PRI_KEY, entry, getOptions(origin));
                STORAGE_LOG(DEBUG) << "MinerPrecompiled new miner node, nodeID : " << nodeID;
            }
            else
            {
                table->update(PRI_KEY, entry, condition, getOptions(origin));
                STORAGE_LOG(DEBUG) << "MinerPrecompiled change to miner, nodeID : " << nodeID;
            }
            break;
        }
        STORAGE_LOG(ERROR) << "MinerPrecompiled open _sys_miners_ failed.";

        break;
    }
    case 0x80599e4b:
    {  // remove(string)
        std::string nodeID;
        abi.abiOut(data, nodeID);
        if (nodeID.size() != 128u)
        {
            STORAGE_LOG(DEBUG) << "NodeID length error. " << nodeID;
            break;
        }
        storage::Table::Ptr table = openTable(context, "_sys_miners_");
        if (table.get())
        {
            auto condition = table->newCondition();
            condition->EQ(NODE_KEY_NODEID, nodeID);
            auto entries = table->select(PRI_KEY, condition);
            auto entry = table->newEntry();
            entry->setField(NODE_TYPE, NODE_TYPE_OBSERVER);
            entry->setField("name", PRI_KEY);

            if (entries->size() == 0u)
            {
                STORAGE_LOG(DEBUG)
                    << "MinerPrecompiled remove node not in _sys_miners_, nodeID : " << nodeID;
                entry->setField(NODE_KEY_NODEID, nodeID);
                entry->setField(NODE_KEY_ENABLENUM,
                    boost::lexical_cast<std::string>(context->blockInfo().number + 1));
                table->insert(PRI_KEY, entry, getOptions(origin));
            }
            else
            {
                table->update(PRI_KEY, entry, condition, getOptions(origin));
                STORAGE_LOG(DEBUG) << "MinerPrecompiled remove miner nodeID : " << nodeID;
            }
            break;
        }
        STORAGE_LOG(ERROR) << "MinerPrecompiled open _sys_miners_ failed.";
        break;
    }
    default:
    {
        break;
    }
    }
    return out;
}
