#pragma once

#include <QWidget>
#include "calculatorengine.h"

class QLineEdit;
class QKeyEvent;

class CalculatorWindow : public QWidget
{
public:
    explicit CalculatorWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void input(const QString &key);
    CalculatorEngine m_engine;
    QLineEdit *m_display;
};
