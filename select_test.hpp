#pragma once
#ifndef __SELECT_TEST_HPP__
#define __SELECT_TEST_HPP__

#include "gtest/gtest.h"
#include <sstream>
#include <iostream>

#include "spreadsheet.cpp"
#include "select.hpp"

TEST(SelectTest, InvalidCol) {
    	try{
		Spreadsheet sheet;
    		sheet.set_column_names({"Animal", "Name","Age","color"});
    		sheet.add_row({"dog","Simba","5","black"});
    		std::stringstream res;
    		sheet.set_selection(new Select_Contains(&sheet,"Hao","Dole"));
    		sheet.print_selection(res);
	
		if(sheet.get_column_by_name("Hao") == -1){
			throw 1;
		}
	}
	catch (int x){
		std::cout << "COLUMN'S NAME DOES NOT EXIST, ERROR NUMBER: " << x << std::endl;
	}
 	
}

TEST(SelectTest, EmptyMatch) {
	Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","color"});
        sheet.add_row({"dog","Simba","5","black"});
        std::stringstream res;
	sheet.set_selection(new Select_Contains(&sheet,"Age",""));
        sheet.print_selection(res);
	EXPECT_EQ(res.str(), "dog Simba 5 black \n");
}

TEST(SelectTest, SubStrMatch) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","color"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Name", "Li"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(), "dog Simba The Lion 5 black \n");
}

TEST(SelectTest, NoMatch) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","color"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
	sheet.add_row({"cat", "Leo", "9", "white"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Name", "Leonardo"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(), "");
}
TEST(SelectTest, CapLow) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","color"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
        sheet.add_row({"cat", "Leo", "9", "white"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Name", "leo"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(), "");
}
TEST(SelectTest, SameColName) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","Name"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
        sheet.add_row({"cat", "Leo", "9", "white"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Name", " Lion"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(),  "dog Simba The Lion 5 black \n");
}
TEST(SelectTest, SameColName2) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","Name"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
        sheet.add_row({"cat", "Leo", "9", "white"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Name", "bla"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(),  "");
}
TEST(SelectTest, EmptyData) {
        Spreadsheet sheet;
        sheet.set_column_names({"Animal", "Name","Age","Name"});
        sheet.add_row({"dog","Simba The Lion","5","black"});
        sheet.add_row({"cat", "Leo", "", "white"});
        std::stringstream res;
        sheet.set_selection(new Select_Contains(&sheet,"Age", "100"));
        sheet.print_selection(res);
        EXPECT_EQ(res.str(),  "");
}


#endif //__SELECT_TEST_HPP__
