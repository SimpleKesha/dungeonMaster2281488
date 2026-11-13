#ifndef BITSET_H
#define BITSET_H

#include <QString>
#include <bitset>
#include <string>

class BitSet {
private:
    static const size_t MAX_SIZE = 100;
    std::bitset<MAX_SIZE> bits;
    
public:
    BitSet();
    
    // Основные операции
    void add(int element);
    void remove(int element);
    bool contains(int element) const;
    
    // Операции со множествами
    BitSet unite(const BitSet& other) const;
    BitSet intersect(const BitSet& other) const;
    BitSet difference(const BitSet& other) const;
    
    // Утилиты
    size_t size() const;
    QString toString() const;
    bool isEmpty() const;
    
    // Получить элементы как список
    QString getElementsList() const;
};

#endif // BITSET_H
