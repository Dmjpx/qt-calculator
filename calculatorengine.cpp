#include "calculatorengine.h"
#include <cmath>

QString CalculatorEngine::display() const
{
    if (m_error || m_operation.isNull()) return m_display;
    const QString prefix = m_leftText + ' ' + m_operation;
    return m_newEntry ? prefix : prefix + ' ' + m_display;
}

void CalculatorEngine::clear()
{
    m_display = QStringLiteral("0");
    m_left = 0.0;
    m_operation = QChar();
    m_leftText.clear();
    m_newEntry = true;
    m_error = false;
}

void CalculatorEngine::digit(int value)
{
    if (value < 0 || value > 9) return;
    if (m_error) clear();
    if (m_newEntry || m_display == QStringLiteral("0")) {
        m_display = QString::number(value);
        m_newEntry = false;
    } else if (m_display.size() < 16) {
        m_display += QString::number(value);
    }
}

void CalculatorEngine::decimalPoint()
{
    if (m_error) clear();
    if (m_newEntry) {
        m_display = QStringLiteral("0");
        m_newEntry = false;
    }
    if (!m_display.contains('.')) m_display += '.';
}

void CalculatorEngine::showNumber(double value)
{
    if (!std::isfinite(value)) {
        m_display = QStringLiteral("结果超出范围");
        m_error = true;
        m_operation = QChar();
    } else {
        m_display = QString::number(value == 0.0 ? 0.0 : value, 'g', 15);
    }
}

bool CalculatorEngine::apply(double right)
{
    double result = right;
    if (m_operation == '+') result = m_left + right;
    else if (m_operation == '-') result = m_left - right;
    else if (m_operation == '*') result = m_left * right;
    else if (m_operation == '/') {
        if (right == 0.0) {
            m_display = QStringLiteral("除数不能为 0");
            m_error = true;
            m_operation = QChar();
            m_newEntry = true;
            return false;
        }
        result = m_left / right;
    }
    showNumber(result);
    m_left = result;
    return !m_error;
}

void CalculatorEngine::operation(QChar op)
{
    if (m_error || !QStringLiteral("+-*/").contains(op)) return;
    if (!m_operation.isNull() && !m_newEntry) {
        if (!apply(m_display.toDouble())) return;
    } else {
        m_left = m_display.toDouble();
    }
    m_leftText = m_display;
    m_operation = op;
    m_newEntry = true;
}

void CalculatorEngine::equals()
{
    if (m_error || m_operation.isNull()) return;
    apply(m_display.toDouble());
    m_operation = QChar();
    m_newEntry = true;
}

void CalculatorEngine::backspace()
{
    if (m_error) { clear(); return; }
    if (m_newEntry) {
        if (!m_operation.isNull()) {
            m_operation = QChar();
            m_display = m_leftText;
            m_leftText.clear();
        }
        return;
    }
    // 科学计数法结果作为一个整体；重新输入时由 digit() 替换。
    m_display.chop(1);
    if (m_display.isEmpty() || m_display == QStringLiteral("-"))
        m_display = QStringLiteral("0");
}
