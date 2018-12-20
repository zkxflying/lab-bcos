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
/** @file ExecutiveContext.h
 *  @author mingzhenliu
 *  @date 20180921
 */
#pragma once

#include "Common.h"
#include "Precompiled.h"
#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>
#include <libdevcrypto/Common.h>
#include <libethcore/Block.h>
#include <libethcore/ChainOperationParams.h>
#include <libethcore/PrecompiledContract.h>
#include <libexecutive/StateFace.h>
#include <libstorage/MemoryTableFactory.h>
#include <memory>

namespace dev
{
namespace storage
{
class Table;
}

namespace executive
{
class StateFace;
}

namespace blockverifier
{
class ExecutiveContext : public std::enable_shared_from_this<ExecutiveContext>
{
public:
    typedef std::shared_ptr<ExecutiveContext> Ptr;

    ExecutiveContext(){};

    virtual ~ExecutiveContext(){};

    virtual bytes call(Address const& origin, Address address, bytesConstRef param);

    virtual Address registerPrecompiled(Precompiled::Ptr p);

    virtual bool isPrecompiled(Address address) const;

    Precompiled::Ptr getPrecompiled(Address address) const;

    void setAddress2Precompiled(Address address, Precompiled::Ptr precompiled)
    {
        m_address2Precompiled.insert(std::make_pair(address, precompiled));
    }

    BlockInfo blockInfo() { return m_blockInfo; }
    void setBlockInfo(BlockInfo blockInfo) { m_blockInfo = blockInfo; }

    std::shared_ptr<dev::executive::StateFace> getState();
    void setState(std::shared_ptr<dev::executive::StateFace> state);

    std::shared_ptr<dev::storage::Table> getTable(const Address& address);


    virtual bool isOrginPrecompiled(Address const& _a) const;

    virtual std::pair<bool, bytes> executeOrginPrecompiled(
        Address const& _a, bytesConstRef _in) const;

    void setPrecompiledContract(
        std::unordered_map<Address, dev::eth::PrecompiledContract> const& precompiledContract);

    void dbCommit(dev::eth::Block& block);

    void setMemoryTableFactory(std::shared_ptr<dev::storage::MemoryTableFactory> memoryTableFactory)
    {
        m_memoryTableFactory = memoryTableFactory;
    }

    std::shared_ptr<dev::storage::MemoryTableFactory> getMemoryTableFactory()
    {
        return m_memoryTableFactory;
    }


private:
    std::unordered_map<Address, Precompiled::Ptr> m_address2Precompiled;
    int m_addressCount = 0x10000;
    BlockInfo m_blockInfo;
    std::shared_ptr<dev::executive::StateFace> m_stateFace;
    std::unordered_map<Address, dev::eth::PrecompiledContract> m_precompiledContract;
    std::shared_ptr<dev::storage::MemoryTableFactory> m_memoryTableFactory;
};

}  // namespace blockverifier

}  // namespace dev
