#include "calculatorwindow.h"
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

CalculatorWindow::CalculatorWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(QStringLiteral("计算器"));
    resize(540, 590);
    setMinimumSize(440, 480);
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet(QStringLiteral(
        "QWidget { background: #f0f0f0; font-family: 'Microsoft YaHei UI'; }"
        "QLineEdit { background: white; border: 2px solid #2995ed;"
        " padding: 12px; color: #202020; font-size: 27px; }"
        "QPushButton { background: #e1e1e1; border: 1px solid #adadad;"
        " border-radius: 2px; color: #202020; font-size: 22px; }"
        "QPushButton:hover { background: #e5f1fb; border-color: #0078d7; }"
        "QPushButton:pressed { background: #cce4f7; border-color: #005a9e; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(34, 26, 34, 48);
    layout->setSpacing(30);
    m_display = new QLineEdit(QStringLiteral("0"), this);
    m_display->setObjectName(QStringLiteral("display"));
    m_display->setReadOnly(true);
    m_display->setAlignment(Qt::AlignRight);
    m_display->setMinimumHeight(90);
    m_display->setFocusPolicy(Qt::NoFocus);
    m_display->setAccessibleName(QStringLiteral("算式与计算结果"));
    layout->addWidget(m_display);

    auto *grid = new QGridLayout;
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(25);
    auto add = [this, grid](const QString &text, int row, int column,
                            int rowSpan = 1, const QString &accessible = QString()) {
        auto *button = new QPushButton(text, this);
        button->setMinimumSize(58, 48);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setFocusPolicy(Qt::NoFocus);
        button->setAccessibleName(accessible.isEmpty() ? text : accessible);
        grid->addWidget(button, row, column, rowSpan, 1);
        connect(button, &QPushButton::clicked, this, [this, text] { input(text); });
    };
    add("1", 0, 0); add("2", 0, 1); add("3", 0, 2);
    add("4", 1, 0); add("5", 1, 1); add("6", 1, 2);
    add("7", 2, 0); add("8", 2, 1); add("9", 2, 2);
    add(".", 3, 0); add("0", 3, 1); add("*", 3, 2);
    add("+", 0, 3, 2); add("-", 2, 3); add("/", 3, 3);
    add("CE", 0, 4, 1, QStringLiteral("清零"));
    add("<-", 1, 4, 1, QStringLiteral("退格删除"));
    add("=", 2, 4, 2, QStringLiteral("等于"));
    for (int i = 0; i < 5; ++i) grid->setColumnStretch(i, 1);
    for (int i = 0; i < 4; ++i) grid->setRowStretch(i, 1);
    layout->addLayout(grid, 1);
    setFocus();
}

void CalculatorWindow::input(const QString &key)
{
    if (key.size() == 1 && key.at(0).isDigit()) m_engine.digit(key.toInt());
    else if (key == ".") m_engine.decimalPoint();
    else if (key == "CE") m_engine.clear();
    else if (key == "<-") m_engine.backspace();
    else if (key == "=") m_engine.equals();
    else if (key.size() == 1) m_engine.operation(key.at(0));
    m_display->setText(m_engine.display());
}

void CalculatorWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        input("=");
    else if (event->key() == Qt::Key_Backspace) input("<-");
    else if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Delete)
        input("CE");
    else if (event->text().size() == 1 &&
             QStringLiteral("0123456789.+-*/=").contains(event->text()))
        input(event->text());
    else { QWidget::keyPressEvent(event); return; }
    event->accept();
}
