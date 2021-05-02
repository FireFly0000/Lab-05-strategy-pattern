#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <string>
#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
    virtual int getColid() const = 0;
    virtual std::string getR_data() const = 0;

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

    //virtual bool select(const Spreadsheet* sheet, int row) const
    //{
        //return select(sheet->cell_data(row, column));
    //}

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
    //virtual int getColid() const = 0;
    //virtual std::string getR_data() const = 0;	
};

//this is Select_Contains class
class Select_Contains: public Select
{
protected:
	//int row;
	int _col;
	std::string r_data;
public:
	Select_Contains(const Spreadsheet* sheet, const std::string col, const std::string row){
		_col =  sheet->get_column_by_name(col);
		r_data = row;
	}
	
	virtual bool select(const Spreadsheet* sheet, int row) const{
        	return select(sheet->cell_data(row, _col));
    	}

	virtual bool select(const std::string& s) const {
		if(s.find(r_data)!= std::string::npos){
			return true;
		}	
		else return false;
	}
	virtual int getColid() const{
		return this->_col;
	}
	virtual std::string getR_data() const{
		return this->r_data;
	} 
		
};

/*class Select_Not: public Select_Column{
protected:
       int _col;
       std::string r_data;		
public:
	Select_Not(Select_Column* s){
		this->_col = s->getColid();
		this->r_data = s->getR_data();
	
	}
	virtual bool select(const std::string& s) const {
		if(s.find(r_data) == std::string::npos){
			return true;
		}
		else return false;
        }
};*/






#endif //__SELECT_HPP__
