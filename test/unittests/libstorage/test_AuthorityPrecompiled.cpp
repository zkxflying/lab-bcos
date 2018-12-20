/**
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
 *
 * @brief: unit test for Authority
 *
 * @file test_AuthorityPrecompiled.cpp
 * @author: caryliao
 * @date 20181212
 */

#include "Common.h"
#include "MemoryStorage.h"
#include <json_spirit/JsonSpiritHeaders.h>
#include <libblockverifier/ExecutiveContextFactory.h>
#include <libdevcrypto/Common.h>
#include <libethcore/ABI.h>
#include <libstorage/AuthorityPrecompiled.h>
#include <libstorage/MemoryTable.h>
#include <libstoragestate/StorageStateFactory.h>
#include <boost/test/unit_test.hpp>

using namespace dev;
using namespace dev::blockverifier;
using namespace dev::storage;
using namespace dev::storagestate;

namespace test_AuthorityPrecompiled
{
struct AuthorityPrecompiledFixture
{
    AuthorityPrecompiledFixture()
    {
        blockInfo.hash = h256(0);
        blockInfo.number = 0;
        context = std::make_shared<ExecutiveContext>();
        ExecutiveContextFactory factory;
        auto storage = std::make_shared<MemoryStorage>();
        auto storageStateFactory = std::make_shared<StorageStateFactory>(h256(0));
        factory.setStateStorage(storage);
        factory.setStateFactory(storageStateFactory);
        factory.initExecutiveContext(blockInfo, h256(0), context);
        authorityPrecompiled = context->getPrecompiled(Address(0x1005));
        memoryTableFactory = context->getMemoryTableFactory();
    }

    ~AuthorityPrecompiledFixture() {}

    ExecutiveContext::Ptr context;
    MemoryTableFactory::Ptr memoryTableFactory;
    Precompiled::Ptr authorityPrecompiled;
    BlockInfo blockInfo;
};

BOOST_FIXTURE_TEST_SUITE(test_AuthorityPrecompiled, AuthorityPrecompiledFixture)

BOOST_AUTO_TEST_CASE(insert)
{
    // first insert
    eth::ContractABI abi;
    std::string tableName = "t_test";
    std::string addr = "0x420f853b49838bd3e9466c85a4cc3428c960dde2";
    bytes in = abi.abiIn("insert(string,string)", tableName, addr);
    bytes out = authorityPrecompiled->call(context, bytesConstRef(&in));
    // query
    auto table = memoryTableFactory->openTable(SYS_ACCESS_TABLE);
    auto entries = table->select(USER_TABLE_PREFIX + tableName, table->newCondition());
    BOOST_TEST(entries->size() == 1u);

    // insert again with same item
    out = authorityPrecompiled->call(context, bytesConstRef(&in));
    // query
    table = memoryTableFactory->openTable(SYS_ACCESS_TABLE);
    entries = table->select(USER_TABLE_PREFIX + tableName, table->newCondition());
    BOOST_TEST(entries->size() == 1u);

    // insert new item with same table name, but diffrent address
    addr = "0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b";
    in = abi.abiIn("insert(string,string)", tableName, addr);
    out = authorityPrecompiled->call(context, bytesConstRef(&in));
    // query
    table = memoryTableFactory->openTable(SYS_ACCESS_TABLE);
    entries = table->select(USER_TABLE_PREFIX + tableName, table->newCondition());
    BOOST_TEST(entries->size() == 2u);
}

BOOST_AUTO_TEST_CASE(remove)
{
    // first insert
    eth::ContractABI abi;
    std::string tableName = "t_test";
    std::string addr = "0x420f853b49838bd3e9466c85a4cc3428c960dde2";
    bytes in = abi.abiIn("insert(string,string)", tableName, addr);
    bytes out = authorityPrecompiled->call(context, bytesConstRef(&in));
    // query
    auto table = memoryTableFactory->openTable(SYS_ACCESS_TABLE);
    auto entries = table->select(USER_TABLE_PREFIX + tableName, table->newCondition());
    BOOST_TEST(entries->size() == 1u);

    // remove
    in = abi.abiIn("remove(string,string)", tableName, addr);
    out = authorityPrecompiled->call(context, bytesConstRef(&in));

    // query
    table = memoryTableFactory->openTable(SYS_ACCESS_TABLE);
    Condition::Ptr condition = table->newCondition();
    condition->EQ(STATUS, "1");
    entries = table->select(USER_TABLE_PREFIX + tableName, condition);
    BOOST_TEST(entries->size() == 0u);

    // remove not exist entry
    tableName = "t_ok";
    in = abi.abiIn("remove(string,string)", tableName, addr);
    out = authorityPrecompiled->call(context, bytesConstRef(&in));
    BOOST_TEST(out[31] == 0);
}

BOOST_AUTO_TEST_CASE(queryByName)
{
    // insert
    eth::ContractABI abi;
    std::string tableName = "t_test";
    std::string addr = "0x420f853b49838bd3e9466c85a4cc3428c960dde2";
    bytes in = abi.abiIn("insert(string,string)", tableName, addr);
    bytes out = authorityPrecompiled->call(context, bytesConstRef(&in));

    // queryByName by a existing key
    in = abi.abiIn("queryByName(string)", tableName);
    out = authorityPrecompiled->call(context, bytesConstRef(&in));
    std::string retStr;
    json_spirit::mValue retJson;
    abi.abiOut(&out, retStr);
    BOOST_TEST(json_spirit::read_string(retStr, retJson) == true);
    BOOST_TEST(retJson.get_array().size() == 1);

    // queryByName by a no existing key
    in = abi.abiIn("queryByName(string)", "test");
    out = authorityPrecompiled->call(context, bytesConstRef(&in));
    abi.abiOut(&out, retStr);
    BOOST_TEST(json_spirit::read_string(retStr, retJson) == true);
    BOOST_TEST(retJson.get_array().size() == 0);
}
BOOST_AUTO_TEST_CASE(toString)
{
    BOOST_TEST(authorityPrecompiled->toString(context) == "Authority");
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace test_AuthorityPrecompiled
