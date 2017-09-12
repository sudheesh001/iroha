/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "model/converters/json_query_factory.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "model/generators/query_generator.hpp"
#include "model/generators/signature_generator.hpp"

using namespace iroha::model::converters;
using namespace iroha::model::generators;

TEST(QuerySerializerTest, DeserializeGetAccountWhenValid) {
  JsonQueryFactory querySerializer;

  auto json_query = R"({
    "signature":{
        "pubkey":"2323232323232323232323232323232323232323232323232323232323232323",
        "signature":"23232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323"
    },
    "created_ts":0,
    "creator_account_id":"123",
    "query_counter":0,
    "query_type":"GetAccount",
    "account_id":"test@test"
  })";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_TRUE(res.has_value());
  ASSERT_EQ("123",res.value()->creator_account_id);
}

TEST(QuerySerializerTest, DeserializeGetAccountWhenInvalid) {
  JsonQueryFactory querySerializer;
  auto json_query = R"({
    "created_ts":0,
    "creator_account_id":"123",
    "query_counter":0,
    "query_type":"GetAccount"
  })";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_FALSE(res.has_value());
}


TEST(QuerySerializerTest, DeserializeGetAccountAssetsWhenValid) {
  JsonQueryFactory querySerializer;
  auto json_query = R"({
    "signature":{
        "pubkey":"2323232323232323232323232323232323232323232323232323232323232323",
        "signature":"23232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323"
    },
    "created_ts":0,
    "creator_account_id":"123",
    "query_counter":0,
    "query_type":"GetAccountAssets",
    "account_id":"test@test",
    "asset_id":"coin#test"
  })";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_TRUE(res.has_value());
  auto casted = std::static_pointer_cast<iroha::model::GetAccountAssets>(res.value());
  ASSERT_EQ("test@test", casted->account_id);
  ASSERT_EQ("coin#test", casted->asset_id);
}


TEST(QuerySerializerTest, DeserializeWhenUnknownType) {
  JsonQueryFactory querySerializer;
  auto json_query = R"({
    "signature":{
        "pubkey":"2323232323232323232323232323232323232323232323232323232323232323",
        "signature":"23232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323"
    },
    "created_ts":0,
    "creator_account_id":"123",
    "query_counter":0,
    "query_type":"GetSomething",
    "account_id":"test@test",
    "asset_id":"coin#test"
  })";
  auto res = querySerializer.deserialize(json_query);
  ASSERT_FALSE(res.has_value());
}

TEST(QuerySerializerTest, SerializeGetAccount){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccount(0, "123", 0, "test");
  val->signature = generateSignature(42);
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  auto ser_val = queryFactory.deserialize(json.value());
  ASSERT_TRUE(ser_val.has_value());
  ASSERT_EQ(val->query_hash, ser_val.value()->query_hash);
  ASSERT_EQ(val->signature.signature, ser_val.value()->signature.signature);
}

TEST(QuerySerializerTest, SerializeGetAccountAssets){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccountAssets(0, "123", 0, "test", "coin");
  val->signature = generateSignature(42);
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  auto ser_val = queryFactory.deserialize(json.value());
  ASSERT_TRUE(ser_val.has_value());
  ASSERT_EQ(val->query_hash, ser_val.value()->query_hash);
  ASSERT_EQ(val->signature.signature, ser_val.value()->signature.signature);

}

TEST(QuerySerializerTest, SerializeGetAccountTransactions){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetAccountTransactions(0, "123", 0, "test");
  val->signature = generateSignature(42);
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  auto ser_val = queryFactory.deserialize(json.value());
  ASSERT_TRUE(ser_val.has_value());
  ASSERT_EQ(val->query_hash, ser_val.value()->query_hash);
  ASSERT_EQ(val->signature.signature, ser_val.value()->signature.signature);
}


TEST(QuerySerializerTest, SerializeGetSignatories){
  JsonQueryFactory queryFactory;
  QueryGenerator queryGenerator;
  auto val = queryGenerator.generateGetSignatories(0, "123", 0, "test");
  val->signature = generateSignature(42);
  auto json = queryFactory.serialize(val);
  ASSERT_TRUE(json.has_value());
  auto ser_val = queryFactory.deserialize(json.value());
  ASSERT_TRUE(ser_val.has_value());
  ASSERT_EQ(val->query_hash, ser_val.value()->query_hash);
  ASSERT_EQ(val->signature.signature, ser_val.value()->signature.signature);
}
