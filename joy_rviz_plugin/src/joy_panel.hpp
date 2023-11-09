// MIT License

// Copyright (c) 2023 Project SRS

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <QtWidgets>
#include <QComboBox>
#include "touch_widget.hpp"

#ifndef Q_MOC_RUN
#include "joy_handler.hpp"
#include <rviz_common/panel.hpp>
#endif

namespace joy_rviz_plugin
{

class JoyPanel : public rviz_common::Panel
{
  Q_OBJECT

public:
  JoyPanel(QWidget * parent = nullptr);
  void onInitialize() override;
  void load(const rviz_common::Config & config) override;
  void save(rviz_common::Config config) const override;

public Q_SLOTS:
  void onCheckChange(int state);
  void onClickA(void);
  void onClickB(void);
  void onTick(void);
  void onTouchChange(QPointF point);

private:
  void updateTopicList(void);

  JoyHandler joy_handler_{};
  QCheckBox * enable_check_;
  QComboBox * topic_combo_;
  TouchWidget * touch_;
  QPushButton * a_button_;
  QPushButton * b_button_;
  QTimer * interval_timer_;
  bool is_active_{false};
  bool a_clicked_{false};
  bool b_clicked_{false};
  QPointF touch_point_{};
};

}  // namespace joy_rviz_plugin
