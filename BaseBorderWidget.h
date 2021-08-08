#ifndef BASEBORDERWIDGET_H
#define BASEBORDERWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>

/// Класс с убранной рамкой по умолчанию и самописным перемещением и
/// растяжением окна.
class BaseBorderWidget : public QWidget
{
  Q_OBJECT
  const int MARGIN_MULTIPLIER = 3;

public:
  /// Конструктор по умолчанию. На вход подается указатель динамически созданный виджет,
  /// (либо виджет создается внутри параметров конструкора), который должен быть помещен внуть рамки!
  BaseBorderWidget(QWidget *window = nullptr);
  ~BaseBorderWidget();
  // Возможные области взаимодействия пользователем при помощи мыши
  enum
  {
    TOP_AREA       = 0, // граница сверху
    BOTTOM_AREA       , // граница снизу
    RIGTH_AREA        , // граница справа
    LEFT_AREA         , // граница слева
    INTERFACE_AREA    , // центральная область пользоватеьлского интерфейса
    LEFT_TOP_AREA     , // перекрестие на закрытие окна сверху
    TOP_EXIT_AREA     ,
    BOTTOM_EXIT_AREA  , // перекрестие на закрытие окна снизу
    RIGHT_BOTTOM_AREA ,
    NONE
  };

private:
//  Ui::BaseBorderWidget *ui;
  QWidget *interFaceWidget;
  int     m_mouseArea;
  bool    isMousePressed;
  QPoint m_previousMousePosition;
  QPointF m_mouseClickedPosition;
  int     checkAreaMouseMoved(QMouseEvent *event);
  void    setMaringsFromLayout();
  //Отступы от рамки
  int leftMargin;
  int topMargin;
  int rightMargin;
  int bottomMargin;
protected:
  // переопределенные методы для обработки события
  void mouseMoveEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event)     override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void moveEvent(QMoveEvent *event) override;

};
#endif // BASEBORDERWIDGET_H
