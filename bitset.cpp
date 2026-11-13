#include "bitset.h"

BitSet::BitSet() : bits(0) {}

void BitSet::add(int element) {
    if (element >= 0 && element < (int)MAX_SIZE) {
        bits[element] = 1;
    }
}

void BitSet::remove(int element) {
    if (element >= 0 && element < (int)MAX_SIZE) {
        bits[element] = 0;
    }
}

bool BitSet::contains(int element) const {
    if (element >= 0 && element < (int)MAX_SIZE) {
        return bits[element];
    }
    return false;
}

BitSet BitSet::unite(const BitSet& other) const {
    BitSet result;
    result.bits = bits | other.bits;
    return result;
}

BitSet BitSet::intersect(const BitSet& other) const {
    BitSet result;
    result.bits = bits & other.bits;
    return result;
}

BitSet BitSet::difference(const BitSet& other) const {
    BitSet result;
    result.bits = bits & ~other.bits;
    return result;
}

size_t BitSet::size() const {
    return bits.count();
}

QString BitSet::toString() const {
    QString result = "{ ";
    bool first = true;
    for (size_t i = 0; i < MAX_SIZE; ++i) {
        if (bits[i]) {
            if (!first) result += ", ";
            result += QString::number(i);
            first = false;
        }
    }
    result += " }";
    return result;
}

bool BitSet::isEmpty() const {
    return bits.count() == 0;
}

QString BitSet::getElementsList() const {
    QString result;
    for (size_t i = 0; i < MAX_SIZE; ++i) {
        if (bits[i]) {
            result += QString::number(i) + "\n";
        }
    }
    return result.isEmpty() ? "Пусто" : result;
}
