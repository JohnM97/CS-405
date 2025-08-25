#include "pch.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>

// Global environment setup
class Environment : public ::testing::Environment {
public:
    ~Environment() override {}

    void SetUp() override {
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    void TearDown() override {}
};

// Collection test fixture
class CollectionTest : public ::testing::Test {
protected:
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override {
        collection = std::make_unique<std::vector<int>>();
    }

    void TearDown() override {
        collection->clear();
        collection.reset(nullptr);
    }

    void add_entries(int count) {
        assert(count > 0);
        for (int i = 0; i < count; ++i) {
            collection->push_back(rand() % 100);
        }
    }
};

// === Base Tests ===

TEST_F(CollectionTest, CollectionSmartPointerIsNotNull) {
    ASSERT_TRUE(collection);
    ASSERT_NE(collection.get(), nullptr);
}

TEST_F(CollectionTest, IsEmptyOnCreate) {
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

// === Intentional Failure Test (Commented Out) ===
// TEST_F(CollectionTest, AlwaysFail) {
//     FAIL();
// }

// === Required Tests ===

TEST_F(CollectionTest, CanAddToEmptyVector) {
    ASSERT_TRUE(collection->empty());
    add_entries(1);
    EXPECT_EQ(collection->size(), 1);
}

TEST_F(CollectionTest, CanAddFiveValuesToVector) {
    add_entries(5);
    EXPECT_EQ(collection->size(), 5);
}

TEST_F(CollectionTest, MaxSizeAlwaysGreaterOrEqualToSize) {
    for (int entries : {0, 1, 5, 10}) {
        collection->clear();
        if (entries > 0) add_entries(entries);
        EXPECT_GE(collection->max_size(), collection->size());
    }
}

TEST_F(CollectionTest, CapacityGreaterOrEqualToSize) {
    for (int entries : {0, 1, 5, 10}) {
        collection->clear();
        if (entries > 0) add_entries(entries);
        EXPECT_GE(collection->capacity(), collection->size());
    }
}

TEST_F(CollectionTest, ResizeIncreasesSize) {
    collection->resize(5);
    EXPECT_EQ(collection->size(), 5);
}

TEST_F(CollectionTest, ResizeDecreasesSize) {
    add_entries(10);
    collection->resize(3);
    EXPECT_EQ(collection->size(), 3);
}

TEST_F(CollectionTest, ResizeToZeroEmptiesCollection) {
    add_entries(4);
    collection->resize(0);
    EXPECT_TRUE(collection->empty());
}

TEST_F(CollectionTest, ClearErasesAllElements) {
    add_entries(7);
    collection->clear();
    EXPECT_TRUE(collection->empty());
    EXPECT_EQ(collection->size(), 0);
}

TEST_F(CollectionTest, EraseAllElementsUsingIterator) {
    add_entries(5);
    collection->erase(collection->begin(), collection->end());
    EXPECT_TRUE(collection->empty());
}

// === Reserve Behavior ===

TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize) {
    size_t originalCapacity = collection->capacity();
    collection->reserve(originalCapacity + 10);
    EXPECT_GE(collection->capacity(), originalCapacity + 10);
    EXPECT_EQ(collection->size(), 0);
}

// === Negative Test: Out of Range Access ===

TEST_F(CollectionTest, AccessOutOfRangeThrowsException) {
    add_entries(2);
    EXPECT_THROW(collection->at(10), std::out_of_range);
}

// === Custom Positive Test ===

TEST_F(CollectionTest, SumOfAllElementsCorrect) {
    collection->push_back(5);
    collection->push_back(10);
    collection->push_back(15);
    int sum = std::accumulate(collection->begin(), collection->end(), 0);
    EXPECT_EQ(sum, 30);
}

// === Custom Negative Test ===

TEST_F(CollectionTest, NegativeIndexCastTriggersException) {
    EXPECT_ANY_THROW({
        collection->resize(static_cast<size_t>(-1));
        });
}

