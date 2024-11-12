#ifndef PREDEFINES_H
#define PREDEFINES_H

/*
    This file is for defining constants or predefined values across all the files and classes.

    And the namespace is for avoiding variable name conflicts and clearer definitions.

*/

#include<iostream>
#include <QString>

namespace StyleSheet{

    const std::string ORDO_listWidgetStyleSheet =
        "QListWidget {"
        "   font-size: 15px;"
        //"   font-weight: 200;"
        "}"
        "QListWidget::item {"
        "   background-color: #FFFFF0;"
        "   color: #0d172a;"
        "   border: 0px solid black;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   margin: 3px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #06A77D;"
        "   color: white;"
        "   border: 1px solid black;"
        "}";

    const std::string ORDO_buttonStyle =
        "QPushButton {"
        "background-color: #fff;"
        "border: 0 solid #e2e8f0;"
        "border-radius: 5px;"
        "color: #0d172a;"
        "font-family: 'Basier circle',-apple-system,system-ui,'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans',sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol','Noto Color Emoji';"
        "font-size: 15px;"
        "font-weight: 600;"
        "line-height: 1;"
        "padding: 5px 5px;"
        "text-align: center;"
        "text-decoration:none #0d172a solid;"
        "}"

        "QPushButton:hover {"
        "background-color: #221D23;"
        "color: #fff;"
        "}";

    const std::string ORDO_elementsStyle =
        "border-radius:15px; "
        "background-color: #FFFFF0;"
        "color: #0d172a;"
        "border: 0px solid black;"
        "font-size: 15px;"
        "font-weight: 600;"
        "line-height: 1;";

    const std::string ORDO_islandStyle =
        "border-radius:20px; "
        "background-color:#ffebc2";

    const std::string ORDO_LabelStyleBordered =
        "border:2px solid black;"
        "border-radius: 10px;"
        "font-size: 15px;"
        "font-weight: 600;"
        "line-height: 1;"
        "padding: 2px 2px;"
        "color:black";

    // const std::string mainBG=
    //     "background:"
}

namespace styles{

    const QString buttonNormal=
        "QWidget{"
        "font-size:14px;"
        "color: white;"
        "border-radius:5px"
        "}"
        "QPushButton{"
        "background:black;"
        "}"
        "QPushButton:hover{"
        "background:#1E1E1E;"
        "}";

    const QString buttonVisited=
        "QWidget{"
        "font-size:14px;"
        "color: white;"
        "border-radius:5px"
        "}"
        "QPushButton{"
        "background:#1E1E1E;"
        "}"
        "QPushButton:hover{"
        "background:#1E1E1E;"
        "}";

    const QString buttonBrowseVisited=
        "QWidget{"
        "font-size:14px;"
        "color: white;"
        "border-radius:5px"
        "}"
        "QPushButton{"
        "background:#1E1E1E;"
        "border-bottom-left-radius:5px;"
        "border-bottom-right-radius:0px;"
        "border-top-left-radius:5px;"
        "border-top-right-radius:0px;"
        "text-align:right;"
        "}"
        "QPushButton:hover{"
        "background:#1E1E1E;"
        "}";

    const QString buttonBrowseNormal=
        "QWidget{"
        "font-size:14px;"
        "color: white;"
        "border-radius:5px"
        "}"
        "QPushButton{"
        "background:black;"
        "text-align:right;"
       "border-bottom-left-radius:5px;"
       "border-bottom-right-radius:0px;"
       "border-top-left-radius:5px;"
       "border-top-right-radius:0px;"
        "}"
        "QPushButton:hover{"
        "background:#1E1E1E;"
        "}"
        ;
}

#endif // PREDEFINES_H

