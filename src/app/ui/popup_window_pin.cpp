// Aseprite
// Copyright (C) 2001-2016  David Capello
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/ui/popup_window_pin.h"

#include "app/modules/gfx.h"
#include "app/modules/gui.h"
#include "app/ui/skin/button_icon_impl.h"
#include "app/ui/skin/skin_theme.h"
#include "base/bind.h"
#include "gfx/border.h"
#include "gfx/size.h"
#include "ui/ui.h"

#include <vector>

namespace app {

using namespace app::skin;
using namespace ui;

PopupWindowPin::PopupWindowPin(const std::string& text, ClickBehavior clickBehavior)
  : PopupWindow(text, clickBehavior)
  , m_pin("")
{
  SkinTheme* theme = SkinTheme::instance();

  m_pin.Click.connect(&PopupWindowPin::onPinClick, this);
  m_pin.setIconInterface(
    new ButtonIconImpl(theme->parts.unpinned(),
                       theme->parts.pinned(),
                       theme->parts.unpinned(),
                       CENTER | MIDDLE));
}

void PopupWindowPin::showPin(bool state)
{
  m_pin.setVisible(state);
}

void PopupWindowPin::setPinned(bool pinned)
{
  m_pin.setSelected(pinned);

  Event ev(this);
  onPinClick(ev);
}

void PopupWindowPin::onPinClick(Event& ev)
{
  if (m_pin.isSelected()) {
    makeFloating();
  }
  else {
    gfx::Rect rc = bounds();
    rc.enlarge(8);
    setHotRegion(gfx::Region(rc));
    makeFixed();
  }
}

bool PopupWindowPin::onProcessMessage(Message* msg)
{
  switch (msg->type()) {

    case kOpenMessage: {
      if (!isPinned())
        makeFixed();
      break;
    }

  }

  return PopupWindow::onProcessMessage(msg);
}

void PopupWindowPin::onWindowMovement()
{
  PopupWindow::onWindowMovement();

  // If the window isn't pinned and we move it, we can automatically
  // pin it.
  if (!m_pin.isSelected())
    setPinned(true);
}

} // namespace app
