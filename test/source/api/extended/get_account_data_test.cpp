//
// MIT License
//
// Copyright (c) 2017-2018 Thibault Martinez and Simon Ninon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//

#include <gtest/gtest.h>

#include <iota/api/extended.hpp>
#include <iota/api/responses/get_account_data.hpp>
#include <iota/errors/illegal_state.hpp>
#include <iota/models/bundle.hpp>
#include <iota/models/seed.hpp>
#include <test/utils/configuration.hpp>
#include <test/utils/constants.hpp>

TEST(Extended, GetAccountData) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };
  auto res = api.getAccountData(ACCOUNT_2_SEED, 0, 0, true, 0);

  EXPECT_EQ(res.getAddresses(),
            std::vector<IOTA::Models::Address>(
                { ACCOUNT_2_ADDRESS_1_HASH, ACCOUNT_2_ADDRESS_2_HASH, ACCOUNT_2_ADDRESS_3_HASH,
                  ACCOUNT_2_ADDRESS_4_HASH, ACCOUNT_2_ADDRESS_5_HASH, ACCOUNT_2_ADDRESS_6_HASH }));

  EXPECT_EQ(res.getAddresses()[0].getBalance(), ACCOUNT_2_ADDRESS_1_FUND);
  EXPECT_EQ(res.getAddresses()[1].getBalance(), ACCOUNT_2_ADDRESS_2_FUND);
  EXPECT_EQ(res.getAddresses()[2].getBalance(), ACCOUNT_2_ADDRESS_3_FUND);
  EXPECT_EQ(res.getAddresses()[3].getBalance(), ACCOUNT_2_ADDRESS_4_FUND);
  EXPECT_EQ(res.getAddresses()[4].getBalance(), ACCOUNT_2_ADDRESS_5_FUND);
  EXPECT_EQ(res.getAddresses()[5].getBalance(), ACCOUNT_2_ADDRESS_6_FUND);

  std::vector<IOTA::Models::Bundle> expectedBundleRes;

  expectedBundleRes.push_back(
      IOTA::Models::Bundle({ IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_1_TRX_1_TRYTES) }));

  expectedBundleRes.push_back(
      IOTA::Models::Bundle({ IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_2_TRX_1_TRYTES) }));

  expectedBundleRes.push_back(
      IOTA::Models::Bundle({ IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_3_TRX_1_TRYTES) }));

  expectedBundleRes.push_back(
      IOTA::Models::Bundle({ IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_4_TRX_1_TRYTES) }));

  expectedBundleRes.push_back(
      IOTA::Models::Bundle({ IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_5_TRX_1_TRYTES),
                             IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_5_TRX_2_TRYTES),
                             IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_5_TRX_3_TRYTES),
                             IOTA::Models::Transaction(ACCOUNT_2_BUNDLE_5_TRX_4_TRYTES) }));

  ASSERT_EQ(res.getTransfers().size(), expectedBundleRes.size());

  for (size_t i = 0; i < expectedBundleRes.size(); ++i) {
    EXPECT_EQ(res.getTransfers()[i], expectedBundleRes[i]);
  }

  EXPECT_EQ(res.getBalance(), ACCOUNT_2_FUND);
}

TEST(Extended, GetAccountDataStartEnd) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };
  auto res = api.getAccountData(ACCOUNT_2_SEED, 1, 3, true, 0);

  EXPECT_EQ(res.getAddresses(), std::vector<IOTA::Models::Address>(
                                    { ACCOUNT_2_ADDRESS_2_HASH, ACCOUNT_2_ADDRESS_3_HASH }));

  EXPECT_EQ(res.getAddresses()[0].getBalance(), ACCOUNT_2_ADDRESS_2_FUND);
  EXPECT_EQ(res.getAddresses()[1].getBalance(), ACCOUNT_2_ADDRESS_3_FUND);

  EXPECT_FALSE(res.getTransfers().empty());
  EXPECT_EQ(res.getBalance(), ACCOUNT_2_ADDRESS_2_FUND + ACCOUNT_2_ADDRESS_3_FUND);
}

TEST(Extended, GetAccountDataMin) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  EXPECT_THROW(api.getAccountData(ACCOUNT_2_SEED, 0, 0, true, 1000000), IOTA::Errors::IllegalState);
}
