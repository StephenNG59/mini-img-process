#include "editcommand1.h"

EditCommand::EditCommand(QTableWidgetItem* item,QString ori_text,QString new_text,QUndoCommand *parent) :
    QUndoCommand(parent)
{
    this->setText("Edit text");
    m_tableitem=item;
    m_oritext=ori_text;
    m_newtext=new_text;
}

void EditCommand::undo()
{
    m_tableitem->setText(m_oritext);
}

void EditCommand::redo()
{
    m_tableitem->setText(m_newtext);
}

void XlsxTableWidget::SlotItemEdit(QTableWidgetItem *new_item)
{
    if(new_item->text()!=ori_text)
    {
        EditCommand* text_command=new EditCommand(new_item,ori_text,new_item->text(),NULL);
        m_stack->push(text_command);
        is_edit=false;
    }
}
