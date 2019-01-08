/**
 * @file    clabsotoolbutton.h
 * @brief   a toolbuttont for clabso
 * @author  chl
 * @date    2017.9.26
 * @version 1.0.1:5
 * @license
 * @note
 */

#ifndef CLABSOTOOLBUTTON_H
#define CLABSOTOOLBUTTON_H

#include <QToolButton>

class  ClabsoToolButton : public QToolButton
{
public:
    explicit ClabsoToolButton(QWidget *parent = 0);
    ~ClabsoToolButton();
    QSize sizeHint() const Q_DECL_OVERRIDE;
};

#endif // CLABSOTOOLBUTTON_H
