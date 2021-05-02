#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
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

//this is Select_Contains class


/*class Select_Contains: public Select_Column
{
private:
	//string col;
	std::string r_data;
public:
	Select_Contains(const Spreadsheet* sheet, const std::string col, const std::string row):
		Select_Column::Select_Column(sheet, col),
		r_data(row)
	{
	}
	//virtual bool select(const Spreadsheet* sheet, int row) const{
		//return select(sheet->cell_data(row, column));
	//}
	virtual bool select(const std::string& s) const {
		if(s.find(r_data)!= std::string::npos){
			return true;
		}	
		else return false;
	}		
};

class Select_Not: public Select_Column{
private:
	std::string r_data;
public:
	Select_Not(const Spreadsheet* sheet, const std::string col, const std::string row):
		Select_Column::Select_Column(sheet, col),
		r_data(row)
	{
	}
	virtual bool select(const std::string& s) const {
		if(s.find(r_data) != std::string::npos){
			return false;
		}
		else return true;
        }
};*/






#endif //__SELECT_HPP__
