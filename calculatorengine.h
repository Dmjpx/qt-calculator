#pragma once

#include <QString>

// 顺序运算：例如 2 + 3 * 4 = 20，与普通桌面计算器一致。
class CalculatorEngine
{
public:
    QString display() const;
    void digit(int value);
    void decimalPoint();
    void operation(QChar op);
    void equals();
    void clear();
    void backspace();

private:
    bool apply(double right);
    void showNumber(double value);
    QString m_display = QStringLiteral("0");
    QString m_leftText;
    double m_left = 0.0;
    QChar m_operation;
    bool m_newEntry = true;
    bool m_error = false;
};
