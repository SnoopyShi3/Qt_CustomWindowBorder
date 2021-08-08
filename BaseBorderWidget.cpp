#include "BaseBorderWidget.h"
#include <QPainter>
#include <QGridLayout>

BaseBorderWidget::BaseBorderWidget(QWidget *interface):
    QWidget(nullptr), interFaceWidget(interface)
{
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setMouseTracking(true);
  QGridLayout* layout = new QGridLayout(this);
  if(!interFaceWidget)
    interFaceWidget = new QWidget(this);
  layout->addWidget(interFaceWidget);
  this->setContentsMargins(1, 1, 1, 1);
  interFaceWidget->setMouseTracking(true);
  m_mouseArea = NONE;
  m_previousMousePosition = QPoint(0, 0);
  setMaringsFromLayout();
  isMousePressed = false;
  repaint();
}
BaseBorderWidget::~BaseBorderWidget()
{
}
/// Функция проверки области, где происходит движение мыши.
int BaseBorderWidget::checkAreaMouseMoved(QMouseEvent *event)
{
  qreal width  = this->width();
  qreal height = this->height();
  // Области рамки, отвечающие за растяжение и перемещение
  QRect topRect(this->x(), this->y(), width - MARGIN_MULTIPLIER * rightMargin, topMargin);
  QRect leftRect(this->x(), this->y(), leftMargin, height - MARGIN_MULTIPLIER * bottomMargin);
  QRect rigthRect(this->x() + width - rightMargin, this->y() + topMargin * MARGIN_MULTIPLIER,
                  rightMargin, height - MARGIN_MULTIPLIER * bottomMargin);
  QRect bottomRect(this->x() + MARGIN_MULTIPLIER * leftMargin,
                   this->y() + height - bottomMargin, width - rightMargin, bottomMargin);
  QRect interFaceRect(this->x() + leftMargin, this->y() + topMargin,
                      width - rightMargin - leftMargin, height - bottomMargin - topMargin);

  QRect topExitRect(this->x() + width - rightMargin * MARGIN_MULTIPLIER, this->y(),
                    rightMargin * MARGIN_MULTIPLIER, topMargin);

  QRect rigthExitRect(this->x() + width - rightMargin, this->y(),
                      rightMargin, topMargin * MARGIN_MULTIPLIER);

  QRect leftExitRect(this->x(), this->y() + height - bottomMargin * MARGIN_MULTIPLIER,
                     leftMargin, bottomMargin * MARGIN_MULTIPLIER);

  QRect bottomExitRect(this->x(), this->y() + height - bottomMargin,
                       leftMargin * MARGIN_MULTIPLIER, bottomMargin) ;
  // Проверка в какую из областей попал курсор мыши
  if(topRect.contains(m_previousMousePosition) || leftRect.contains(m_previousMousePosition))
    return LEFT_TOP_AREA;
  else if(bottomRect.contains(m_previousMousePosition) || rigthRect.contains(m_previousMousePosition))
    return RIGHT_BOTTOM_AREA;
  else if(rigthExitRect.contains(m_previousMousePosition) || topExitRect.contains(m_previousMousePosition))
    return TOP_EXIT_AREA;
  else if(leftExitRect.contains(m_previousMousePosition) ||
          bottomExitRect.contains(m_previousMousePosition))
    return BOTTOM_EXIT_AREA;
  else if(interFaceRect.contains(m_previousMousePosition))
    return INTERFACE_AREA;
  return NONE;
}
/// Фнукция считывает параметры из layout'a виджета и ставит нужные значения отступов
void BaseBorderWidget::setMaringsFromLayout()
{
  auto layout = this->layout();
  if(layout)
    layout->getContentsMargins(&leftMargin, &topMargin, &rightMargin, &bottomMargin);
  else
  {
    qDebug() << "Error! Layout isn't exist!";
    leftMargin = rightMargin = topMargin = bottomMargin = 8;
    return;
  }
}

void BaseBorderWidget::mouseMoveEvent(QMouseEvent *event)
{
  m_previousMousePosition.setX(this->x() + event->x());
  m_previousMousePosition.setY(this->y() + event->y());
  if(!isMousePressed)
    m_mouseArea = checkAreaMouseMoved(event);
  auto mousePosition = event->screenPos();
  if(m_mouseArea == LEFT_TOP_AREA && isMousePressed)
  {
    auto dx = m_mouseClickedPosition.x() - mousePosition.x();/* + event->x() */;
    auto dy = m_mouseClickedPosition.y() - mousePosition.y()/* + event->y() */;
    setGeometry(this->x() - dx, this->y() - dy , width(), height());
    m_mouseClickedPosition = event->screenPos();
  }
  else if(m_mouseArea == RIGHT_BOTTOM_AREA && isMousePressed)
  {
    auto dx = m_mouseClickedPosition.x() - mousePosition.x();/* + event->x() */;
    auto dy = m_mouseClickedPosition.y() - mousePosition.y()/* + event->y() */;
    setGeometry(this->x(), this->y(), width() - dx, height() - dy);
    m_mouseClickedPosition = event->screenPos();
  }
  repaint();
  return QWidget::mouseMoveEvent(event);
}

/// TODO Оптимизнуть фукнцию
void BaseBorderWidget::paintEvent(QPaintEvent *event)
{
  QColor defaultFrameColor(215, 215, 215);  // Светло-серый цвет рамки
  QColor selectedFrameColor(166, 202, 240); // Светло-синий цвет рамки

  QPainter painter(this);
  QRect TopRect(0, 0, width() - MARGIN_MULTIPLIER * rightMargin, topMargin);
  QRect LeftRect(0, 0, leftMargin, height() -MARGIN_MULTIPLIER * bottomMargin);
  QRect RightRect(width() - rightMargin, 0 + topMargin * MARGIN_MULTIPLIER,
                  rightMargin, height() - MARGIN_MULTIPLIER * bottomMargin);
  QRect BottomRect(0 + rightMargin * MARGIN_MULTIPLIER, height() - bottomMargin,
                   width() - MARGIN_MULTIPLIER * rightMargin, bottomMargin);
  // Отрисовка рамки слева и наверху
  painter.setPen(Qt::transparent);
  painter.setBrush(QColor(defaultFrameColor));
  if(m_mouseArea == LEFT_TOP_AREA)
  {
    painter.setBrush(QColor(selectedFrameColor));;
  }
  painter.drawRect(TopRect);
  painter.drawRect(LeftRect);
  painter.setBrush(QColor(defaultFrameColor));
  if(m_mouseArea == RIGHT_BOTTOM_AREA)
  {
    painter.setBrush(QColor(selectedFrameColor));
  }
  painter.drawRect(BottomRect);
  painter.drawRect(RightRect);

  if(m_mouseArea == TOP_EXIT_AREA || m_mouseArea == BOTTOM_EXIT_AREA)
  {
    painter.setBrush(QColor(128, 0, 0));
    painter.drawRect(width() - MARGIN_MULTIPLIER * rightMargin, 0, MARGIN_MULTIPLIER * rightMargin, topMargin);
    painter.drawRect(width() - rightMargin, 0, rightMargin, MARGIN_MULTIPLIER * topMargin);
    painter.drawRect(0, height() - MARGIN_MULTIPLIER * bottomMargin, leftMargin, bottomMargin * MARGIN_MULTIPLIER);
    painter.drawRect(0, height() - bottomMargin, leftMargin * MARGIN_MULTIPLIER, bottomMargin);
  }
  // Линии для придания объёма рамке
  painter.setPen(QPen(Qt::darkGray, 2));
  painter.drawLine(QLine(QPoint(leftMargin, topMargin),
                         QPoint(width() - rightMargin, topMargin)));
  painter.drawLine(QLine(QPoint(leftMargin, topMargin),
                         QPoint(leftMargin, height() - bottomMargin)));

  painter.setPen(QPen(Qt::white, 2));
  painter.drawLine(QLine(QPoint(leftMargin, height() - bottomMargin),
                         QPoint(width() - rightMargin, height() - bottomMargin)));
  painter.drawLine(QLine(QPoint(width() - rightMargin, topMargin),
                         QPoint(width() - rightMargin, height() - bottomMargin)));
  return QWidget::paintEvent(event);
}

void BaseBorderWidget::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
  {
    isMousePressed = true;
    m_mouseClickedPosition = event->screenPos();
  }
  return QWidget::mousePressEvent(event);
}

void BaseBorderWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
    isMousePressed = false;
  return QWidget::mouseReleaseEvent(event);
}

void BaseBorderWidget::leaveEvent(QEvent *event)
{
  m_mouseArea = NONE;
  repaint();
  return QWidget::leaveEvent(event);
}

void BaseBorderWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton &&
          (m_mouseArea == TOP_EXIT_AREA || m_mouseArea == BOTTOM_EXIT_AREA))
  {
    interFaceWidget->close();
    close();
  }
}

void BaseBorderWidget::moveEvent(QMoveEvent *event)
{
  //Вызываем ивент предка для запоминания координат
  this->children().at(1)->event(event);
}
