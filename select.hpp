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
    //Select_Column(const Spreadsheet* sheet, const std::string& name)
    //{
        //column = sheet->get_column_by_name(name);
    //}
    
    Select_Column(){ }
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
    virtual int getCol() = 0;
};

//this is Select_Contains class
class Select_Contains: public Select_Column
{
protected:
	std::string r_data;
public:
	Select_Contains(Spreadsheet* sheet, const std::string& col, const std::string& row){
		column = sheet->get_column_by_name(col);
		r_data = row;
	}
	bool select(const std::string& s) const {
		if(s.find(r_data)!= std::string::npos){
			return true;
		}	
		else return false;
	} 
	virtual int getCol(){
		return this->column;
	}		
};

class Select_Not: public Select_Column
{
private:
	Select_Column* t = nullptr;
public:
	~Select_Not(){
		delete t;
	}
	Select_Not(Select_Column* sc){
		t=sc;
	}
	
	bool select(const Spreadsheet* sheet, int row) const {
        	return select(sheet->cell_data(row,t->getCol()));
    	}	
	bool select(const std::string& s) const { 
		if(t->select(s)){
			return false;
		}
		else return true;
	}	
	virtual int getCol(){
		return t->getCol();
	}
};

class Select_And: public Select_Column
{
private:
        Select_Column* lt = nullptr;
	Select_Column* rt = nullptr;
public:
        ~Select_And(){
                delete lt;
		delete rt;
        }
        Select_And(Select_Column* sl, Select_Column* sr){
                lt = sl;
		rt = sr;
        }

       virtual bool select(const Spreadsheet* sheet, int row) const {
		return (lt->select(sheet, row) && rt->select(sheet, row) );
       }
       virtual bool select(const std::string& s) const {
		return true;
	
       }
	virtual int getCol(){
		return -1;
	}
};

class Select_Or: public Select_Column
{
private:
        Select_Column* lt = nullptr;
        Select_Column* rt = nullptr;
public:
        ~Select_Or(){
                delete lt;
                delete rt;
        }
        Select_Or(Select_Column* sl, Select_Column* sr){
                lt = sl;
                rt = sr;
        }

        virtual bool select(const Spreadsheet* sheet, int row) const {
                return( lt->select(sheet, row) || rt->select(sheet, row));
       	}
       	virtual bool select(const std::string& s) const {
                return true;
        }
	virtual int getCol(){
		return -1;
	}
};

#endif //__SELECT_HPP__
