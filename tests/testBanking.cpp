#include <iostream>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct MockAcc : public Account{
    MockAcc(int id, int balance) : Account(id, balance){}
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(GetBalance, int());
    MOCK_METHOD0(id, int());
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

struct MockTrans : public Transaction{
    MOCK_METHOD3(Make, bool(Account& from, Account& to, int sum));
    MOCK_METHOD1(set_fee, void(int fee));
    MOCK_METHOD0(fee, int());
};

TEST(Account, AccMockTest){
    MockAcc Bank(2, 100000);
    EXPECT_CALL(Bank, Lock()).Times(1);
    EXPECT_CALL(Bank, GetBalance()).Times(2);
    EXPECT_CALL(Bank, ChangeBalance(testing::_)).Times(2);
    EXPECT_CALL(Bank, Unlock()).Times(1);
    Bank.Lock();
    Bank.GetBalance();
    Bank.ChangeBalance(100000);
    Bank.GetBalance();
    Bank.Unlock();
    Bank.ChangeBalance(1000);
}

TEST(Account, Lock){
	Account store_bank(105, 150000);
	store_bank.Lock();
	
	ASSERT_NO_THROW(store_bank.ChangeBalance(100000));
	EXPECT_EQ(store_bank.GetBalance(),250000);
	ASSERT_NO_THROW(store_bank.ChangeBalance(-100000));
	EXPECT_EQ(store_bank.GetBalance(),150000);
	
	store_bank.Unlock();

	ASSERT_ANY_THROW(store_bank.ChangeBalance(100000));
	ASSERT_NO_THROW(store_bank.GetBalance());
}

TEST(Account, ChangeBalance){
	Account store_bank(44,1500000);
	Account car_bank(25,12000);
	store_bank.Lock();
	car_bank.Lock();

	EXPECT_EQ(store_bank.GetBalance(),1500000);
	EXPECT_EQ(store_bank.id(),44);

	store_bank.ChangeBalance(6886);

	EXPECT_EQ(store_bank.GetBalance(),1506886);
	EXPECT_EQ(store_bank.id(),44);

	car_bank.ChangeBalance(0);

	ASSERT_NO_THROW(car_bank.ChangeBalance(0));
	EXPECT_EQ(car_bank.GetBalance(),12000);

	store_bank.Unlock();
	car_bank.Unlock();
}

TEST(Transaction, TestMethods){
      Account user_from(231, 67'000);
      Account user_to(456, 35'000);
      MockTransaction test_transaction;
      
      EXPECT_CALL(test_transaction, fee())
	      .Times(3)
              .WillOnce(Return(1))
	      .WillRepeatedly(Return(500));

      EXPECT_CALL(test_transaction, set_fee(_))
	      .Times(1);

      EXPECT_CALL(test_transaction, Make)
	      .Times(AtLeast(1));

      test_transaction.fee();
      test_transaction.set_fee(500);
      test_transaction.fee();

      test_transaction.Make(user_from, user_to, 30'000);

      test_transaction.Make(user_from, user_to, 40'000);

      test_transaction.fee();
}

TEST(Transaction, UsualWork){
      Account bank(111'996, 1'000'000);
      Account user(55'984, 13'000);
      Transaction test_trans; 

      EXPECT_EQ(test_trans.fee(), 1);     
      test_trans.set_fee(1000);
      EXPECT_EQ(test_trans.fee(), 1000);

      EXPECT_TRUE(test_trans.Make(bank, user, 10'000));
      EXPECT_EQ(bank.GetBalance(), 989'000);
      EXPECT_EQ(user.GetBalance(), 23'000);
}
