// Copyright 2024 StrayedCats.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <QComboBox>
#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>

#include <QtWidgets>

// Q_MOC_RUN is defined when this file is processed by moc
#ifndef Q_MOC_RUN
#include "button_rviz_plugin/button_handler.hpp"
#include <rviz_common/panel.hpp>

#include <rviz_common/config.hpp>
#include <rviz_common/display_context.hpp>
#endif

namespace button_rviz_plugin
{

class EmptyButton : public rviz_common::Panel
{
  Q_OBJECT

public:
  EmptyButton(QWidget * parent = nullptr);
  void onInitialize() override;
  void load(const rviz_common::Config & config) override;
  void save(rviz_common::Config config) const override;

public Q_SLOTS:
  void onCheckChange(int state);
  void onClickA(void);
  void onTick(void);

private:
  void updateTopicList(void);

  ButtonHandler button_handler_{};
  QCheckBox * enable_check_;
  QComboBox * topic_combo_;
  QPushButton * a_button_;
  QTimer * interval_timer_;
  bool is_active_{false};
  bool a_clicked_{false};
};

}  // namespace button_rviz_plugin
