/**********************************************************
Author: 微信公众号(你才小学生)
WeChat public platform: nicaixiaoxuesheng
Email:  2088201923@qq.com
**********************************************************/

#include "Keyboard.h"
#include <QVBoxLayout>
#include <QDebug>

using namespace AeaQt;

const int normalButtonWidth = 55;
const int normalButtonHeight = 45;
const QString deleteIcon = "/home/strong/android/otherspace/QtKeyboard/Image/Delete_ios7@2x.png";

typedef QList<KeyButton::Mode> Modes;
typedef QList<Modes> ModesList;

const QList<Modes> modeListBar1 = {
    {{Qt::Key_Q, "q"}, {Qt::Key_Q, "Q"}, {Qt::Key_1, "1"}},
    {{Qt::Key_W, "w"}, {Qt::Key_W, "W"}, {Qt::Key_2, "2"}},
    {{Qt::Key_E, "e"}, {Qt::Key_E, "E"}, {Qt::Key_3, "3"}},
    {{Qt::Key_R, "r"}, {Qt::Key_R, "R"}, {Qt::Key_4, "4"}},
    {{Qt::Key_T, "t"}, {Qt::Key_T, "T"}, {Qt::Key_5, "5"}},
    {{Qt::Key_Y, "y"}, {Qt::Key_Y, "Y"}, {Qt::Key_6, "6"}},
    {{Qt::Key_U, "u"}, {Qt::Key_U, "U"}, {Qt::Key_7, "7"}},
    {{Qt::Key_I, "i"}, {Qt::Key_I, "I"}, {Qt::Key_8, "8"}},
    {{Qt::Key_O, "o"}, {Qt::Key_O, "O"}, {Qt::Key_9, "9"}},
    {{Qt::Key_P, "p"}, {Qt::Key_P, "P"}, {Qt::Key_0, "0"}},
};

const QList<Modes> modeListBar2 = {
    {{Qt::Key_A, "a"}, {Qt::Key_A, "A"}, {Qt::Key_unknown, "~"}},
    {{Qt::Key_S, "s"}, {Qt::Key_S, "S"}, {Qt::Key_unknown, "!"}},
    {{Qt::Key_D, "d"}, {Qt::Key_D, "D"}, {Qt::Key_At, "@"}},
    {{Qt::Key_F, "f"}, {Qt::Key_F, "F"}, {Qt::Key_NumberSign, "#"}},
    {{Qt::Key_G, "g"}, {Qt::Key_G, "G"}, {Qt::Key_Percent, "%"}},
    {{Qt::Key_H, "h"}, {Qt::Key_H, "H"}, {Qt::Key_unknown, "'"}},
    {{Qt::Key_J, "j"}, {Qt::Key_J, "J"}, {Qt::Key_unknown, "&", "&&"}},
    {{Qt::Key_K, "k"}, {Qt::Key_K, "K"}, {Qt::Key_unknown, "*"}},
    {{Qt::Key_L, "l"}, {Qt::Key_L, "L"}, {Qt::Key_unknown, "?"}},
};

const QList<Modes> modeListBar3 = {
    {{Qt::Key_CapsLock, "", "切换"}},
    {{Qt::Key_Z, "z"}, {Qt::Key_Z, "Z"}, {Qt::Key_ParenLeft, "("}},
    {{Qt::Key_X, "x"}, {Qt::Key_X, "X"}, {Qt::Key_ParenLeft, ")"}},
    {{Qt::Key_C, "c"}, {Qt::Key_C, "C"}, {Qt::Key_Minus, "-"}},
    {{Qt::Key_V, "v"}, {Qt::Key_V, "V"}, {Qt::Key_unknown, "_"}},
    {{Qt::Key_B, "b"}, {Qt::Key_B, "B"}, {Qt::Key_unknown, ":"}},
    {{Qt::Key_N, "n"}, {Qt::Key_N, "N"}, {Qt::Key_Semicolon, ";"}},
    {{Qt::Key_M, "m"}, {Qt::Key_M, "M"}, {Qt::Key_Slash, "/"}},
    {{Qt::Key_Backspace, "", "退格"}}
};

const QList<Modes> modeListBar4 = {
    {{Qt::Key_Mode_switch, "", "?123"}},
    {{Qt::Key_Context1, "", "中"}, {Qt::Key_Context1, "", "En"}},
    {{Qt::Key_Space,  " ", "空格"}},
    {{Qt::Key_Enter,  "", "回车"}}
};

const QList<ModesList> modesListBar = { modeListBar1, modeListBar2, modeListBar3, modeListBar4 };

Keyboard::Keyboard(QWidget *parent) :
    AbstractKeyboard(parent)
{
    QBoxLayout *layout = new QVBoxLayout(this);

    for (int i = 0; i < modesListBar.count(); i++) {
        ModesList each = modesListBar.at(i);
        QHBoxLayout *HBar = new QHBoxLayout();
        if (i == 1)
            HBar->setContentsMargins(30, 0, 30, 0);

        HBar->setSpacing(5);
        for (int j = 0; j < each.count(); j++) {
            Modes modes = each.at(j);
            KeyButton *button = createButton(modes);
            HBar->addWidget(button);
        }

        layout->addLayout(HBar);
    }

    foreach (KeyButton *button, findChildren<KeyButton *>()) {
        if (button->mode().key == Qt::Key_CapsLock) {
            connect(button, SIGNAL(pressed()), this, SLOT(switchCapsLock()));
        }
        else if (button->mode().key == Qt::Key_Mode_switch) {
            connect(button, SIGNAL(pressed()), this, SLOT(switchSpecialChar()));
        }
        else if (button->mode().key == Qt::Key_Context1) {
            connect(button, SIGNAL(pressed()), this, SLOT(switchEnOrCh()));
        }
    }
}

void Keyboard::update(const QString &text)
{
    qDebug()<<">>>>>>: "<<text;
}

void Keyboard::switchCapsLock()
{
    QList<KeyButton *> buttons = findChildren<KeyButton *>();
    foreach(KeyButton *button, buttons)
        button->switchCapsLock();
}

void Keyboard::switchSpecialChar()
{
    QList<KeyButton *> buttons = findChildren<KeyButton *>();
    foreach(KeyButton *button, buttons)
        button->switchSpecialChar();
}

void Keyboard::switchEnOrCh()
{
    QList<KeyButton *> buttons = findChildren<KeyButton *>();
    foreach(KeyButton *button, buttons) {
        if (button->mode().key == Qt::Key_Context1)
            button->switching();
        button->switchCapsLock();
    }
}

KeyButton *Keyboard::createButton(QList<KeyButton::Mode> modes)
{
    KeyButton *button = new KeyButton(modes, this);
    button->onReponse((AbstractKeyboard*)this, SIGNAL(keyPressed(int, QString)));
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switch (button->mode().key) {
    case Qt::Key_CapsLock:
    case Qt::Key_Backspace: {
        button->setIconSize(QSize(normalButtonWidth, normalButtonHeight));
        button->setMinimumSize(2*normalButtonWidth, normalButtonHeight);
        button->setMaximumSize(2*normalButtonWidth, normalButtonHeight);
        break;
    }
    case Qt::Key_Mode_switch:
    case Qt::Key_Context1: {
        button->setMinimumSize(1.5*normalButtonWidth, normalButtonHeight);
        button->setMaximumSize(1.5*normalButtonWidth, normalButtonHeight);
        break;
    }
    case Qt::Key_Enter: {
        button->setMinimumSize(3*normalButtonWidth, normalButtonHeight);
        button->setMaximumSize(3*normalButtonWidth, normalButtonHeight);
    }
    case Qt::Key_Space: {
        break;
    }
    default: {
        button->setMinimumSize(normalButtonWidth, normalButtonHeight);
    }
    }
    return button;
}
