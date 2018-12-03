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
/**
 * @brief : Encrypt level DB
 * @author: jimmyshi, websterchen
 * @date: 2018-11-26
 */

#pragma once

#include <leveldb/db.h>
#include <leveldb/slice.h>
#include <libdevcore/BasicLevelDB.h>
#include <libdevcore/easylog.h>
#include <libdevcrypto/AES.h>
#include <string>

namespace dev
{
namespace db
{
#define ENCDBLOG(_OBV) LOG(_OBV) << " [ENCDB] "

class KeyCenter
{
public:
    KeyCenter(const std::string _url) : m_url(_url){};
    const std::string getDataKey(const std::string& _cypherDataKey)
    {
        // Fake it
        return "01234567012345670123456701234567";
    };

    const std::string generateCypherDataKey()
    {
        // Fake it
        return std::string("0123456701234567012345670123456") + std::to_string(utcTime() % 10);
    }

private:
    std::string m_url;
};

class EncryptedLevelDBWriteBatch : public LevelDBWriteBatch
{
public:
    EncryptedLevelDBWriteBatch(const std::string& _dataKey) : m_dataKey(_dataKey) {}
    void insertSlice(leveldb::Slice _key, leveldb::Slice _value) override;

private:
    std::string m_dataKey;
};

class EncryptedLevelDB : public BasicLevelDB
{
public:
    EncryptedLevelDB(const leveldb::Options& _options, const std::string& _name,
        const std::string& _keyCenterUrl = "", const std::string& _cypherDataKey = "");
    ~EncryptedLevelDB(){};

    static leveldb::Status Open(const leveldb::Options& _options, const std::string& _name,
        BasicLevelDB** _dbptr, const std::string& _keyCenterUrl = "",
        const std::string& _cypherDataKey = "");  // DB
                                                  // open
    leveldb::Status Get(const leveldb::ReadOptions& _options, const leveldb::Slice& _key,
        std::string* _value) override;
    leveldb::Status Put(const leveldb::WriteOptions& _options, const leveldb::Slice& _key,
        const leveldb::Slice& _value) override;

    std::unique_ptr<LevelDBWriteBatch> createWriteBatch() const override;

    enum class OpenDBStatus
    {
        FirstCreation = 0,
        NoEncrypted,
        CypherKeyError,
        Encrypted
    };

private:
    KeyCenter m_keyCenter;
    std::string m_cypherDataKey;
    std::string m_dataKey;

private:
    std::string getKeyOfDatabase();
    void setCypherDataKey(std::string _cypherDataKey);
    EncryptedLevelDB::OpenDBStatus checkOpenDBStatus();
};
}  // namespace db
}  // namespace dev