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

#include "button_panel.hpp"

#include <rviz_common/config.hpp>
#include <rviz_common/display_context.hpp>

#include <QPainter>
#include <QMouseEvent>
#include <QSizePolicy>

namespace button_rviz_plugin
{
EmptyButton::EmptyButton(QWidget * parent)
: rviz_common::Panel(parent)
{
  QVBoxLayout * layout = new QVBoxLayout;

  QHBoxLayout * layout_1st = new QHBoxLayout;
  enable_check_ = new QCheckBox("Enable");
  layout_1st->addWidget(enable_check_);
  topic_combo_ = new QComboBox();
  topic_combo_->setEditable(true);
  layout_1st->addWidget(topic_combo_);
  // layout->addLayout(layout_1st);

  // QHBoxLayout * layout_3rd = new QHBoxLayout;
  a_button_ = new QPushButton("click");
  layout_1st->addWidget(a_button_);
  layout->addLayout(layout_1st);

  setLayout(layout);

  interval_timer_ = new QTimer(this);

  connect(interval_timer_, &QTimer::timeout, this, &EmptyButton::onTick);
  connect(enable_check_, &QCheckBox::stateChanged, this, &EmptyButton::onCheckChange);
  connect(a_button_, &QPushButton::clicked, this, &EmptyButton::onClickA);

  interval_timer_->start(100);
}

void EmptyButton::onInitialize()
{
  button_handler_.setRosNodePtr(
    this->getDisplayContext()->getRosNodeAbstraction().lock()->get_raw_node());
  updateTopicList();
}

void EmptyButton::onCheckChange(int state)
{
  if (state == Qt::Checked) {
    std::string topic_name = topic_combo_->currentText().toStdString();
    bool ret = button_handler_.initializePublisher(topic_name);
    if (!ret) {
      return;
    }
    is_active_ = true;
  } else {
    button_handler_.finalizePublisher();
    is_active_ = false;
    updateTopicList();
  }
}

void EmptyButton::onClickA()
{
  a_clicked_ = true;
}

void EmptyButton::onTick()
{
  if (is_active_) {
    button_handler_.publishButton(a_clicked_);
    a_clicked_ = false;
  }
}

void EmptyButton::save(rviz_common::Config config) const
{
  rviz_common::Panel::save(config);
  config.mapSetValue("BaseTopic", topic_combo_->currentText());
  config.mapSetValue("Checked", enable_check_->isChecked());
}

void EmptyButton::load(const rviz_common::Config & config)
{
  rviz_common::Panel::load(config);
  QString tmp_text;
  bool tmp_bool;
  if (config.mapGetString("BaseTopic", &tmp_text)) {
    topic_combo_->setCurrentText(tmp_text);
  }
  if (config.mapGetBool("Checked", &tmp_bool)) {
    enable_check_->setChecked(tmp_bool);
  }
}

void EmptyButton::updateTopicList(void)
{
  std::string previous_topic_name = topic_combo_->currentText().toStdString();
  auto topic_list = button_handler_.getEmptyTopicList();
  topic_combo_->clear();
  int same_topic_index = -1;
  for (auto t : topic_list) {
    topic_combo_->addItem(t.c_str());
    if (t == previous_topic_name) {
      same_topic_index = topic_combo_->count() - 1;
    }
  }

  if (previous_topic_name != "") {
    if (same_topic_index < 0) {
      topic_combo_->addItem(previous_topic_name.c_str());
      same_topic_index = topic_combo_->count() - 1;
    }
    topic_combo_->setCurrentIndex(same_topic_index);
  }
}

} // namespace button_rviz_plugin

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(button_rviz_plugin::EmptyButton, rviz_common::Panel)
