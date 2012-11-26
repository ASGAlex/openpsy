#ifndef BUTTONSTATE_H
#define BUTTONSTATE_H

#include <QPushButton>

class ButtonState
{
public:
    ButtonState();
    ButtonState(QPushButton* srcButton);

    void saveButton(QPushButton* srcButton);
    bool restoreButton(QPushButton* dstButton);

private:
    QString text;
    bool checkable;
    bool checked;
    bool enabled;
    bool visible;
};

#endif // BUTTONSTATE_H
