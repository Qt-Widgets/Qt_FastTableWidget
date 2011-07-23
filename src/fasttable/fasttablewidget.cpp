#include "fasttablewidget.h"

FastTableWidget::FastTableWidget(QWidget *parent) :
    QAbstractScrollArea(parent)
{
    mRowCount=0;
    mColumnCount=0;

    mDefaultHeight=20;
    mDefaultWidth=100;
    mTotalHeight=0;
    mTotalWidth=0;

    mDefaultBackgroundColor.setRgb(255, 255, 255);
    mDefaultForegroundColor.setRgb(0, 0, 0);

    mStartSelection=false;

    horizontalScrollBar()->setSingleStep(100);
    verticalScrollBar()->setSingleStep(100);
}

FastTableWidget::~FastTableWidget()
{
    clearTable();
}

void FastTableWidget::clearTable()
{
    resetBackgroundColors();
    resetForegroundColors();
    resetFonts();

    mRowCount=0;
    mColumnCount=0;

    mTotalHeight=0;
    mTotalWidth=0;

    mRowHeights.clear();
    mColumnWidths.clear();
    mOffsetX.clear();
    mOffsetY.clear();
    mData.clear();
    mBackgroundColors.clear();
    mBackgroundColors.clear();
    mCellFonts.clear();
    mSelectedCells.clear();
    mCurSelection.clear();

    mStartSelection=false;
}

void FastTableWidget::resetBackgroundColors()
{
    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            if (mBackgroundColors.at(i).at(j))
            {
                delete mBackgroundColors.at(i).at(j);
                mBackgroundColors[i][j]=0;
            }
        }
    }
}

void FastTableWidget::resetForegroundColors()
{
    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            if (mForegroundColors.at(i).at(j))
            {
                delete mForegroundColors.at(i).at(j);
                mForegroundColors[i][j]=0;
            }
        }
    }
}

void FastTableWidget::resetFonts()
{
    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            if (mCellFonts.at(i).at(j))
            {
                delete mCellFonts.at(i).at(j);
                mCellFonts[i][j]=0;
            }
        }
    }
}

void FastTableWidget::resetBackgroundColor(const int row, const int column)
{
    if (mBackgroundColors.at(row).at(column))
    {
        delete mBackgroundColors.at(row).at(column);
        mBackgroundColors[row][column]=0;
    }
}

void FastTableWidget::resetForegroundColor(const int row, const int column)
{
    if (mForegroundColors.at(row).at(column))
    {
        delete mForegroundColors.at(row).at(column);
        mForegroundColors[row][column]=0;
    }
}

void FastTableWidget::resetFont(const int row, const int column)
{
    if (mCellFonts.at(row).at(column))
    {
        delete mCellFonts.at(row).at(column);
        mCellFonts[row][column]=0;
    }
}

void FastTableWidget::selectRow(const int row)
{
    for (int i=0; i<mColumnCount; ++i)
    {
        setCellSelected(row, i, true);
    }
}

void FastTableWidget::unselectRow(const int row)
{
    for (int i=0; i<mColumnCount; ++i)
    {
        setCellSelected(row, i, false);
    }
}

void FastTableWidget::selectColumn(const int column)
{
    for (int i=0; i<mRowCount; ++i)
    {
        setCellSelected(i, column, true);
    }
}

void FastTableWidget::unselectColumn(const int column)
{
    for (int i=0; i<mRowCount; ++i)
    {
        setCellSelected(i, column, false);
    }
}

void FastTableWidget::selectAll()
{
    unselectAll();

    QPair<int, int> aCellPos;

    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            aCellPos.first=i;
            aCellPos.second=j;

            mCurSelection.append(aCellPos);

            mSelectedCells[i][j]=true;
        }
    }
}

void FastTableWidget::unselectAll()
{
    for (int i=0; i<mCurSelection.length(); ++i)
    {
        mSelectedCells[mCurSelection.at(i).first][mCurSelection.at(i).second]=false;
    }

    mCurSelection.clear();
}

void FastTableWidget::mousePressEvent(QMouseEvent *event)
{
    QAbstractScrollArea::mousePressEvent(event);
}

void FastTableWidget::resizeEvent(QResizeEvent *event)
{
    updateBarsRanges();

    QAbstractScrollArea::resizeEvent(event);
}

void FastTableWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());

    int offsetX=-horizontalScrollBar()->value();
    int offsetY=-verticalScrollBar()->value();

    for (int i=0; i<mRowCount; ++i)
    {
        for (int j=0; j<mColumnCount; ++j)
        {
            paintCell(painter, offsetX+mOffsetX.at(j), offsetY+mOffsetY.at(i), i, j);
        }
    }
}

void FastTableWidget::paintCell(QPainter &painter, const int x, const int y, const int row, const int column)
{
    painter.drawRect(x, y, mColumnWidths.at(column), mRowHeights.at(row));
    painter.drawText(x+2, y+2, mColumnWidths.at(column)-4, mRowHeights.at(row)-4, 0, mData.at(row).at(column));
}

void FastTableWidget::updateBarsRanges()
{
    QSize areaSize = viewport()->size();

    horizontalScrollBar()->setPageStep(mTotalWidth);
    verticalScrollBar()->setPageStep(mTotalHeight);

    horizontalScrollBar()->setRange(0, mTotalWidth - areaSize.width());
    verticalScrollBar()->setRange(0, mTotalHeight - areaSize.height());
}

int FastTableWidget::rowCount()
{
    return mRowCount;
}

void FastTableWidget::setRowCount(int count)
{
    if (count<0)
    {
        count=0;
    }

    if (mRowCount!=count)
    {
        while (mRowCount<count)
        {
            QStringList aNewRow;
            QList<QColor *> aNewRowColor;
            QList<QFont *> aNewRowFont;
            QList<bool> aNewRowbool;

            mTotalHeight+=mDefaultHeight;

            mData.append(aNewRow);
            mRowHeights.append(mDefaultHeight);
            mOffsetY.append(mRowCount==0? 0 : (mOffsetY.at(mRowCount-1)+mRowHeights.at(mRowCount-1)));
            mBackgroundColors.append(aNewRowColor);
            mForegroundColors.append(aNewRowColor);
            mCellFonts.append(aNewRowFont);
            mSelectedCells.append(aNewRowbool);

            for (int i=0; i<mColumnCount; ++i)
            {
                mData[mRowCount].append("");
                mBackgroundColors[mRowCount].append(0);
                mForegroundColors[mRowCount].append(0);
                mCellFonts[mRowCount].append(0);
                mSelectedCells[mRowCount].append(false);
            }

            mRowCount++;
        }

        while (mRowCount>count)
        {
            mRowCount--;

            mTotalHeight-=mRowHeights.at(mRowCount);

            for (int i=0; i<mColumnCount; ++i)
            {
                if (mBackgroundColors.at(mRowCount).at(i))
                {
                    delete mBackgroundColors.at(mRowCount).at(i);
                }

                if (mForegroundColors.at(mRowCount).at(i))
                {
                    delete mForegroundColors.at(mRowCount).at(i);
                }

                if (mCellFonts.at(mRowCount).at(i))
                {
                    delete mCellFonts.at(mRowCount).at(i);
                }
            }

            mData.removeLast();
            mRowHeights.removeLast();
            mOffsetY.removeLast();
            mBackgroundColors.removeLast();
            mForegroundColors.removeLast();
            mCellFonts.removeLast();

            for (int i=0; i<mColumnCount; ++i)
            {
                if (mSelectedCells.last().at(i))
                {
                    for (int j=0; j<mCurSelection.length(); ++j)
                    {
                        if (mCurSelection.at(j).first==mRowCount && mCurSelection.at(j).second==i)
                        {
                            mCurSelection.removeAt(j);
                            break;
                        }
                    }
                }
            }

            mSelectedCells.removeLast();
        }

        updateBarsRanges();
    }
}

int FastTableWidget::columnCount()
{
    return mColumnCount;
}

void FastTableWidget::setColumnCount(int count)
{
    if (count<0)
    {
        count=0;
    }

    if (mColumnCount!=count)
    {
        if (mColumnCount<count)
        {
            for (int i=mColumnCount; i<count; ++i)
            {
                mColumnWidths.append(mDefaultWidth);
                mTotalWidth+=mDefaultWidth;

                mOffsetX.append(i==0? 0 : (mOffsetX.at(i-1)+mColumnWidths.at(i-1)));
            }

            for (int i=0; i<mData.length(); ++i)
            {
                for (int j=mColumnCount; j<count; ++j)
                {
                    mData[i].append("");
                    mBackgroundColors[i].append(0);
                    mForegroundColors[i].append(0);
                    mCellFonts[i].append(0);
                    mSelectedCells[i].append(false);
                }
            }
        }
        else
        {
            for (int i=mColumnCount-1; i>=count; --i)
            {
                mTotalWidth-=mColumnWidths.at(i);
                mColumnWidths.removeLast();
                mOffsetX.removeLast();
            }

            for (int i=0; i<mData.length(); ++i)
            {
                for (int j=mColumnCount-1; j>=count; --j)
                {
                    if (mBackgroundColors.at(i).at(j))
                    {
                        delete mBackgroundColors.at(i).at(j);
                    }

                    if (mForegroundColors.at(i).at(j))
                    {
                        delete mForegroundColors.at(i).at(j);
                    }

                    if (mCellFonts.at(i).at(j))
                    {
                        delete mCellFonts.at(i).at(j);
                    }

                    mData[i].removeLast();
                    mBackgroundColors[i].removeLast();
                    mForegroundColors[i].removeLast();
                    mCellFonts[i].removeLast();

                    if (mSelectedCells.at(i).last())
                    {
                        for (int k=0; k<mCurSelection.length(); ++k)
                        {
                            if (mCurSelection.at(k).first==i && mCurSelection.at(k).second==j)
                            {
                                mCurSelection.removeAt(k);
                                break;
                            }
                        }
                    }

                    mSelectedCells[i].removeLast();
                }
            }
        }

        mColumnCount=count;

        updateBarsRanges();
    }
}

QColor FastTableWidget::defaultBackgroundColor()
{
    return mDefaultBackgroundColor;
}

void FastTableWidget::setDefaultBackgroundColor(QColor color)
{
    mDefaultBackgroundColor=color;
}

QColor FastTableWidget::defaultForegroundColor()
{
    return mDefaultForegroundColor;
}

void FastTableWidget::setDefaultForegroundColor(QColor color)
{
    mDefaultForegroundColor=color;
}

quint16 FastTableWidget::defaultHeight()
{
    return mDefaultHeight;
}

void FastTableWidget::setDefaultHeight(quint16 height)
{
    mDefaultHeight=height;
}

quint16 FastTableWidget::defaultWidth()
{
    return mDefaultWidth;
}

void FastTableWidget::setDefaultWidth(quint16 width)
{
    mDefaultWidth=width;
}

int FastTableWidget::totalHeight()
{
    return mTotalHeight;
}

int FastTableWidget::totalWidth()
{
    return mTotalWidth;
}

QString FastTableWidget::text(const int row, const int column)
{
    return mData.at(row).at(column);
}

void FastTableWidget::setText(const int row, const int column, const QString text)
{
    mData[row][column]=text;
}

quint16 FastTableWidget::rowHeight(const int row)
{
    return mRowHeights.at(row);
}

void FastTableWidget::setRowHeight(const int row, const quint16 height)
{
    if (mRowHeights.at(row)!=height)
    {
        int aDiff=height-mRowHeights.at(row);

        mTotalHeight+=aDiff;
        mRowHeights[row]=height;

        for (int i=row+1; i<mRowCount; ++i)
        {
            mOffsetY[i]=mOffsetY.at(i)+aDiff;
        }
    }
}

quint16 FastTableWidget::columnWidth(const int column)
{
    return mColumnWidths.at(column);
}

void FastTableWidget::setColumnWidth(const int column, const quint16 width)
{
    if (mColumnWidths.at(column)!=width)
    {
        int aDiff=width-mColumnWidths.at(column);

        mTotalWidth+=aDiff;
        mColumnWidths[column]=width;

        for (int i=column+1; i<mColumnCount; ++i)
        {
            mOffsetX[i]=mOffsetX.at(i)+aDiff;
        }
    }
}

QColor FastTableWidget::backgroundColor(const int row, const int column)
{
    if (mBackgroundColors.at(row).at(column))
    {
        return *mBackgroundColors.at(row).at(column);
    }

    return mDefaultBackgroundColor;
}

void FastTableWidget::setBackgroundColor(const int row, const int column, const QColor color)
{
    if (mBackgroundColors.at(row).at(column))
    {
        *mBackgroundColors[row][column]=color;
    }
    else
    {
        mBackgroundColors[row][column]=new QColor(color);
    }
}

QColor FastTableWidget::foregroundColor(const int row, const int column)
{
    if (mForegroundColors.at(row).at(column))
    {
        return *mForegroundColors.at(row).at(column);
    }

    return mDefaultForegroundColor;
}

void FastTableWidget::setForegroundColor(const int row, const int column, const QColor color)
{
    if (mForegroundColors.at(row).at(column))
    {
        *mForegroundColors[row][column]=color;
    }
    else
    {
        mForegroundColors[row][column]=new QColor(color);
    }
}

QFont FastTableWidget::cellFont(const int row, const int column)
{
    if (mCellFonts.at(row).at(column))
    {
        return *mCellFonts.at(row).at(column);
    }

    return this->font();
}

void FastTableWidget::setCellFont(const int row, const int column, const QFont font)
{
    if (mCellFonts.at(row).at(column))
    {
        *mCellFonts[row][column]=font;
    }
    else
    {
        mCellFonts[row][column]=new QFont(font);
    }
}

bool FastTableWidget::cellSelected(const int row, const int column)
{
    return mSelectedCells.at(row).at(column);
}

void FastTableWidget::setCellSelected(const int row, const int column, const bool selected)
{
    if (mSelectedCells.at(row).at(column)!=selected)
    {
        mSelectedCells[row][column]=selected;

        if (selected)
        {
            mCurSelection.append(QPair<int, int>(row, column));
        }
        else
        {
            for (int i=0; i<mCurSelection.length(); ++i)
            {
                if (mCurSelection.at(i).first==row && mCurSelection.at(i).second==column)
                {
                    mCurSelection.removeAt(i);
                    break;
                }
            }
        }
    }
}