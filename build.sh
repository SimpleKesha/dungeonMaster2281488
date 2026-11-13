#!/bin/bash
# Скрипт для подготовки проекта BitSet Qt на Linux

echo "=== Подготовка проекта BitSet на Linux ==="
echo ""

# Проверка наличия Qt
if ! command -v qmake &> /dev/null; then
    echo "ОШИБКА: qmake не найден!"
    echo "Установите Qt4 Dev:"
    echo "  Ubuntu/Debian: sudo apt-get install qt4-qmake libqt4-dev"
    echo "  Fedora: sudo yum install qt-devel"
    exit 1
fi

echo "✓ qmake найден: $(qmake -v | head -1)"
echo ""

# Компиляция с qmake
echo "Компилирование проекта..."
qmake bitset.pro
make clean
make -j4

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Компиляция успешна!"
    echo ""
    echo "Запуск приложения..."
    ./bitset
else
    echo "✗ Ошибка при компиляции"
    exit 1
fi
