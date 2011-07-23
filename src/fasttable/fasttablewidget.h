#ifndef FASTTABLEWIDGET_H
#define FASTTABLEWIDGET_H

#include <QAbstractScrollArea>
#include <QTableWidget>
#include <QList>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QPainter>
#include <QScrollBar>

class FastTableWidget : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(int     rowCount               READ rowCount               WRITE setRowCount)
    Q_PROPERTY(int     columnCount            READ columnCount            WRITE setColumnCount)
    Q_PROPERTY(quint16 defaultHeight          READ defaultHeight          WRITE setDefaultHeight)
    Q_PROPERTY(quint16 defaultWidth           READ defaultWidth           WRITE setDefaultWidth)
    Q_PROPERTY(QColor  defaultBackgroundColor READ defaultBackgroundColor WRITE setDefaultBackgroundColor)
    Q_PROPERTY(QColor  defaultForegroundColor READ defaultForegroundColor WRITE setDefaultForegroundColor)

public:
    explicit FastTableWidget(QWidget *parent = 0);
    ~FastTableWidget();

    void clearTable();

    void resetBackgroundColors();
    void resetForegroundColors();
    void resetFonts();

    void resetBackgroundColor(const int row, const int column);
    void resetForegroundColor(const int row, const int column);
    void resetFont(const int row, const int column);

    void selectRow(const int row);
    void unselectRow(const int row);

    void selectColumn(const int column);
    void unselectColumn(const int column);

    void selectAll();
    void unselectAll();

    int rowCount();
    void setRowCount(int count);

    int columnCount();
    void setColumnCount(int count);

    QColor defaultBackgroundColor();
    void setDefaultBackgroundColor(QColor color);

    QColor defaultForegroundColor();
    void setDefaultForegroundColor(QColor color);

    quint16 defaultHeight();
    void setDefaultHeight(quint16 height);

    quint16 defaultWidth();
    void setDefaultWidth(quint16 width);

    int totalHeight();
    int totalWidth();

    QString text(const int row, const int column);
    void setText(const int row, const int column, const QString text);

    quint16 rowHeight(const int row);
    void setRowHeight(const int row, const quint16 height);

    quint16 columnWidth(const int column);
    void setColumnWidth(const int column, const quint16 width);

    QColor backgroundColor(const int row, const int column);
    void setBackgroundColor(const int row, const int column, const QColor color);

    QColor foregroundColor(const int row, const int column);
    void setForegroundColor(const int row, const int column, const QColor color);

    QFont cellFont(const int row, const int column);
    void setCellFont(const int row, const int column, const QFont font);

    bool cellSelected(const int row, const int column);
    void setCellSelected(const int row, const int column, const bool selected);

protected:
    int mRowCount;
    int mColumnCount;

    quint16 mDefaultHeight;
    quint16 mDefaultWidth;
    int mTotalHeight;
    int mTotalWidth;

    QColor mDefaultBackgroundColor;
    QColor mDefaultForegroundColor;

    QList< QStringList > mData;
    QList< quint16 > mRowHeights;
    QList< quint16 > mColumnWidths;
    QList< QList<bool> > mSelectedCells;
    QList< QPair<int, int> > mCurSelection;
    QList< QList<QColor *> > mBackgroundColors;
    QList< QList<QColor *> > mForegroundColors;
    QList< QList<QFont *> > mCellFonts;

    bool mStartSelection;

    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void paintCell(QPainter &painter, const int x, const int y, const int row, const int column);

    void updateBarsRanges();
};

#endif // FASTTABLEWIDGET_H
