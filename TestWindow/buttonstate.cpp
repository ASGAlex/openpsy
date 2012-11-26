#include "buttonstate.h"

ButtonState::ButtonState()
{
}

ButtonState::ButtonState(QPushButton *srcButton)
{
  saveButton(srcButton);
}

void ButtonState::saveButton(QPushButton *srcButton)
{
  text = srcButton->text();
  checkable = srcButton->isCheckable();
  checked = srcButton->isChecked();
  enabled = srcButton->isEnabled();
  visible = srcButton->isVisible();
}

bool ButtonState::restoreButton(QPushButton *dstButton)
{
  if(dstButton)
  {
    dstButton->setText(text);
    dstButton->setEnabled(enabled);
    dstButton->setVisible(visible);
    dstButton->setCheckable(checkable);
    dstButton->setChecked(checked);

    return true;
  }

  return false;
}
