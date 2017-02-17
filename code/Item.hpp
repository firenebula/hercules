/****************************************************
 * Author: Phuc Vo
 * Date: 01/30/2017
 * Description:
 *    The Item class is used to create a Item object 
 * for the text adventure game.
 *    Item.hpp is the class specification file.
 *
 * **************************************************/


#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;

class Item
{
   private:
      string name;
      string description;
      string use_text;
	  string talk_text;
      bool movable;
	  int quantity;
	  bool available;
      

   public:
	  Item(string name);
	  Item(string name, string desc, string use, string talk, bool movable, int quantity, bool available);
	  void setDesc(string description);
	  void setUse(string use_text);
	  void setTalk(string talk_text);
	  void setMovable(bool movable);
	  void setQuantity(int quantity);
	  void setAvailable(bool available);

	  string getName();
	  bool isMovable();
	  bool isAvailable();
	  int getQuantity();
      string look();
	  string use();
	  string use(string target);
	  string talk();
      void drop();

	  void printItem();

};
#endif



