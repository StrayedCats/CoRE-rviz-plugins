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

#include "touch_widget.hpp"
#include <sstream>
#include <iostream>

TouchWidget::TouchWidget(QWidget * parent)
: QWidget(parent)
{
  setMinimumSize(100, 100);
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void TouchWidget::setEnabled(bool enable)
{
  grayout_ = !enable;
  update();
}

void TouchWidget::setEnabled(int state)
{
  grayout_ = (state != Qt::Checked);
  update();
}

void TouchWidget::paintEvent(QPaintEvent * event)
{
  (void)event;
  QPoint center = getCenter();
  int pad_size = getPadSize();

  QPainter painter(this);
  if (!grayout_) {
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
  } else {
    painter.setBrush(Qt::lightGray);
    painter.setPen(Qt::darkGray);
  }

  painter.drawRect(QRect(center.x() - pad_size / 2, center.y() - pad_size / 2, pad_size, pad_size));
  painter.drawLine(center.x(), center.y() - pad_size / 2, center.x(), center.y() + pad_size / 2);
  painter.drawLine(center.x() - pad_size / 2, center.y(), center.x() + pad_size / 2, center.y());

  if (!grayout_) {
    QPen arrow;
    arrow.setWidth(pad_size / 20);
    arrow.setColor(Qt::black);
    arrow.setCapStyle(Qt::RoundCap);
    arrow.setJoinStyle(Qt::RoundJoin);
    painter.setPen(arrow);

    QPoint arrows[2];
    arrows[0] = center;
    arrows[1] = getPadPoint(latest_scaled_value_);
    painter.drawPolyline(arrows, 2);
  }
}

void TouchWidget::mouseMoveEvent(QMouseEvent * event)
{
  if (!grayout_) {
    setValue(getScaledPoint(QPoint(event->x(), event->y())));
    update();
  }
}

void TouchWidget::mousePressEvent(QMouseEvent * event)
{
  if (!grayout_) {
    setValue(getScaledPoint(QPoint(event->x(), event->y())));
    update();
  }
}

void TouchWidget::mouseReleaseEvent(QMouseEvent * event)
{
  (void)event;
  setValue(QPointF{});
  update();
}

// private
void TouchWidget::setValue(QPointF point)
{
  latest_scaled_value_ = point;
  Q_EMIT notifyScaledPoint(point);
  std::string stm = std::to_string(point.x()) + "," + std::to_string(point.y());
  QString text = QString::fromStdString(stm.c_str());
  Q_EMIT notifyScaledPoint(text);
}

int TouchWidget::getPadSize(void) const
{
  int w = width();
  int h = height();
  int half_size = ((w > h) ? (h - 1) : (w - 1)) / 2;
  return half_size * 2;
}

QPoint TouchWidget::getCenter(void) const
{
  return QPoint((width() - 1) / 2, (height() - 1) / 2);
}

QPointF TouchWidget::getScaledPoint(const QPoint & point) const
{
  int pad_size = getPadSize();
  QPoint center = getCenter();
  float scaled_x = (float)(point.x() - center.x()) / (pad_size / 2);
  float scaled_y = (float)(point.y() - center.y()) / (pad_size / 2);

  QPointF output;
  output.setX(std::min(std::max(scaled_x, -1.0f), 1.0f));
  output.setY(std::min(std::max(scaled_y, -1.0f), 1.0f));
  return output;
}

QPoint TouchWidget::getPadPoint(const QPointF & point) const
{
  int pad_size = getPadSize();
  QPoint center = getCenter();

  QPoint output;
  output.setX(center.x() + point.x() * (pad_size / 2));
  output.setY(center.y() + point.y() * (pad_size / 2));
  return output;
}
