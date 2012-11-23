// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_ui_Client_ui_TabBuilder_hpp
#define cf3_ui_Client_ui_TabBuilder_hpp

///////////////////////////////////////////////////////////////////////////

#include <QList>
#include <QMap>
#include <QTabWidget>
#include <QTabBar>
#include <QDragMoveEvent>

#include "common/PropertyList.hpp"
#include "common/UUCount.hpp"

#include "ui/core/CNode.hpp"

#include "ui/graphics/LibGraphics.hpp"

///////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace ui {
namespace graphics {

///////////////////////////////////////////////////////////////////////////

struct TabInfo
{
  QWidget * widget;

  int tab_index;

  bool is_visible;
};

class TabBuilder : public QTabWidget
{
  Q_OBJECT

public:

  static TabBuilder * instance();

  template<typename TYPE>
  TYPE * widget( Handle< core::CNode > node )
  {
    TYPE * widget = nullptr;
    std::string key = node->properties().value_str("uuid"); //node->uri().path();

    if( !m_tabs.contains(key) )
    {
      TabInfo info;

      info.widget = new TYPE(this);
      info.tab_index = addTab(info.widget, node->uri().path().c_str());
      info.is_visible = true;
      m_tabs[key] = info;
    }
    else
    {
      setTabText( m_tabs[key].tab_index, node->uri().path().c_str() );
      m_new_tabs.append( key );
    }

    widget = static_cast<TYPE*>(m_tabs[key].widget);

    cf3_assert ( is_not_null(widget) );
    return widget;
  }

  void show_tab( Handle< core::CNode > node );

  void queue_tab( Handle< core::CNode > node);

  void dragEnterEvent ( QDragEnterEvent * event){
    event->accept();
  }

  void dragLeaveEvent(QDragLeaveEvent * event){
    event->accept();
  }

  /// @brief select the tab that is under the cursor when dragging data
  void dragMoveEvent ( QDragMoveEvent * event )
  {
    QTabBar *tab_bar = tabBar();
    int current_tab;
    if (tab_bar && (current_tab=tab_bar->tabAt(event->pos())) != -1){
      setCurrentIndex(current_tab);
    }
    event->accept();
  }

  void dropEvent ( QDragMoveEvent * event )
  {
    event->ignore();
  }

private slots:

  void tab_clicked(int index);

  void begin_model_reset();

  void end_model_reset();

private: // functions

  TabBuilder(QWidget * parent = 0);

  ~TabBuilder();

private : // data

  QMap<common::UUCount, TabInfo> m_tabs;

  QMap<common::UUCount, int> m_last_tabs;

  QList<common::UUCount> m_new_tabs;

}; // TabManager

///////////////////////////////////////////////////////////////////////////

} // Graphics
} // ui
} // cf3

///////////////////////////////////////////////////////////////////////////

#endif // cf3_ui_Client_ui_TabBuilder_hpp
