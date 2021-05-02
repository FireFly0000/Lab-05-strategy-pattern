#ifndef __SELECT_HPP__
#define __SELECT_HPP__
#include <iostream>
#include <string>
#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
    virtual std::string getColumn() = 0;
    virtual std::string getString() = 0;
    virtual Spreadsheet* getSpreadsheet() = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column : public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

// WORK IN PROGRESS!!
class Select_Contains : public Select
{
protected:
    std::string column;
    int columnindex;
    std::string _userstring;
    Spreadsheet* _sheet;

public:
    bool select(const std::string& s) const
    {
        if (s.find(_userstring) != std::string::npos) { // if _userstring is a substring of s, then select returns true
            return true;
        }
        else {
            return false;
        }
    }

    bool select(const Spreadsheet* sheet, int row) const {
        return select(sheet->cell_data(row, this->columnindex));
    }

    Select_Contains(Spreadsheet* sheet, std::string column, std::string userstring)
    {
        //sheet->set_selection(nullptr); // clear selections, probably not needed here.
        this->_userstring = userstring;
        this->column = column;
        this->columnindex = sheet->get_column_by_name(column);
        this->_sheet = sheet;
        for (int i = 0; i < sheet->getnumRows(); ++i) // start at 1st row after column names, end once final roww has been reached. Not sure if gets correct size.
        {
            select(sheet->cell_data(i, this->columnindex)); // query each row to see if it should be selected
            // ??? do more stuff ???
        }

    }

    std::string getString() {
        return this->_userstring;
    }
    std::string getColumn() {
        return this->column;
    }
    Spreadsheet* getSpreadsheet() {
        return this->_sheet;
    }

};

class Select_Not : public Select
{
protected:
    int columnindex;
    std::string _column;
    std::string _userstring;
    Spreadsheet* _sheet;
    Select* _selectptr;

public:
    bool select(const std::string& s) const
    {
        if (s.find(_userstring) == std::string::npos) { // if _userstring is not a substring of s, then select returns true
            return true;
        }
        else {
            return false;
        }
    }

    bool select(const Spreadsheet* sheet, int row) const {
        return select(sheet->cell_data(row, this->columnindex));
    }

    //void Select_Notp(Spreadsheet* sheet, std::string column, std::string userstring)
    //{
    //    //sheet->set_selection(nullptr); // clear selections, probably not needed here.
    //    this->_userstring = userstring;
    //    this->columnindex = sheet->get_column_by_name(column);
    //    for (int i = 0; i < sheet->getnumRows(); ++i) // start at 1st row after column names, end once final roww has been reached. Not sure if gets correct size.
    //    {
    //        select(sheet->cell_data(i, this->columnindex)); // query each row to see if it should be selected
    //    }
    //}

    Select_Not(Select* selectptr) {
        
        this->_selectptr = selectptr;
        this->_userstring = selectptr->getString();
        this->_sheet = selectptr->getSpreadsheet();
        this->_column = selectptr->getColumn();
        this->columnindex = _sheet->get_column_by_name(selectptr->getColumn());
        
        for (int i = 0; i < _sheet->getnumRows(); ++i) {
            select(_sheet->cell_data(i, this->columnindex));
        }
    }
    std::string getString() {
        return this->_userstring;
    }
    std::string getColumn() {
        return this->_column;
    }
    Spreadsheet* getSpreadsheet() {
        return this->_sheet;
    }
};

class Select_And : public Select
{
protected:
    int columnindex;
    std::string _column;
    std::string _userstring;
    Spreadsheet* _sheet;
    Select* _selectptrleft;
    Select* _selectptrright;

public:
    bool select(Select* selectptrleft, Select* selectptrright, int row) const
    {
        if ((selectptrleft->select(selectptrleft->getSpreadsheet(), row) == true) && 
            ((selectptrright->select(selectptrright->getSpreadsheet(), row) == true))) { // if both pointers return true, return true.
            return true;
        }
        else {
            return false;
        }
    }
    
    
    bool select(const Spreadsheet* sheet, int row) const {

        return select(this->_selectptrleft, this->_selectptrright, row);
    }

    Select_And(Select* selectptrleft, Select* selectptrright) {

        this->_selectptrleft = selectptrleft;
        this->_selectptrright = selectptrright;
        this->_userstring = selectptrleft->getString();
        this->_sheet = selectptrleft->getSpreadsheet();
        this->_column = selectptrleft->getColumn();
        this->columnindex = _sheet->get_column_by_name(selectptrleft->getColumn());

        for (int i = 0; i < _sheet->getnumRows(); ++i) 
        {
            select(selectptrleft, selectptrright, i);
        }
    }
    std::string getString() {
        return this->_userstring;
    }
    std::string getColumn() {
        return this->_column;
    }
    Spreadsheet* getSpreadsheet() {
        return this->_sheet;
    }
};

#endif //__SELECT_HPP__