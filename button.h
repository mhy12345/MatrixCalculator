
#ifndef BUTTON_H
#define BUTTON_H
#include <QToolButton>

class Button : public QToolButton
{
    Q_OBJECT
public:
    explicit Button(const QString &text,QWidget* parent=NULL);
    QSize sizeHint()const Q_DECL_OVERRIDE;
};

#endif // BUTTON_H
