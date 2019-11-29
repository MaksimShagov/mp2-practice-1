#include "TList.h"
#include "Converter.h"

void _tlistMain()
{
  TList<int, std::string> list;
  std::string words[7] = {"I", "She", "He", "It", "We", "You", "They"};

  std::cout << "Insert \"" << words[1] << "\" begin: " << std::endl;
  list.InsertBegin(1, &words[1]);
  std::cout << list << std::endl << std::endl;
  
  std::cout << "Insert \"" << words[3] << "\" after:" << std::endl;
  list.InsertAfter(1, 3, &words[3]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Insert \"" << words[2] << "\" before:" << std::endl;
  list.InsertBefore(3, 2, &words[2]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Insert \"" << words[4] << "\" end:" << std::endl;
  list.InsertEnd(4, &words[4]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Insert \"" << words[5] << "\" after last:" << std::endl;
  list.InsertEnd(5, &words[5]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Insert \"" << words[6] << "\" before first:" << std::endl;
  list.InsertBegin(0, &words[6]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Remove element with key 0:" << std::endl;
  list.Remove(0);
  std::cout << list << std::endl << std::endl;

  std::cout << "Insert \"" << words[0] << "\" begin:" << std::endl;
  list.InsertBegin(0, &words[0]);
  std::cout << list << std::endl << std::endl;

  std::cout << "Constructor copy:" << std::endl;
  std::cout << TList<int, std::string>(list) << std::endl << std::endl;

  std::cout << "Constructor with only node with key \"12\" and data \"" << words[6] << "\"" << std::endl;
  std::cout << TList<int, std::string>(&TNode<int, std::string>(12, &words[6])) << std::endl << std::endl;

  try
  {
    std::cout << "Insert \"" << words[0] << "\" after node with key 666:";
    list.InsertAfter(666, 0, &words[0]);
    std::cout << list << std::endl;
  }
  catch (MyException const& e)
  {
    std::cout << std::endl << "  " << e.what() << std::endl;
    std::cout << "  errorLine: " << e.errorLine() << std::endl;
    std::cout << "  errorFile: " << e.errorFile() << std::endl << std::endl;
  }

  try
  {
    std::cout << "Insert \"" << words[0] << "\" before node with key 666:" << std::endl;
    list.InsertBefore(666, 0, &words[0]);
    std::cout << list << std::endl;
  }
  catch (MyException const& e)
  {
    std::cout << "  " << e.what() << std::endl;
    std::cout << "  errorLine: " << e.errorLine() << std::endl;
    std::cout << "  errorFile: " << e.errorFile() << std::endl << std::endl;
  }

  try
  {
    std::cout << "Remove element with key 666:" << std::endl;
    list.Remove(666);
    std::cout << list << std::endl;
  }
  catch (MyException const& e)
  {
    std::cout << "  " << e.what() << std::endl;
    std::cout << "  errorLine: " << e.errorLine() << std::endl;
    std::cout << "  errorFile: " << e.errorFile() << std::endl << std::endl;
  }
}





//--------------------------------- Converter ------------------------------//

void _tStackMain()
{

  std::cout << "Expression can include " << std::endl;
  std::cout << "  * real numbers (example: '2', '71.98')," << std::endl;
  std::cout << "  * operands (example: 'x', 'B', 'asdf', 'amr12')," << std::endl;
  std::cout << "  * mathematical symbols: '+', '-', '*', '/', '(', ')'" << std::endl;
  std::cout << std::endl;
  std::cout << "Please, enter an expression." << std::endl;
  std::cout << "Expression: ";
  std::string buffer;
  std::getline(std::cin, buffer);

  try
  {
    std::cout << "Postfix form: " << Converter::ConvertToPostfixForm(buffer) << std::endl;

    Variables var(Converter::operands);

    std::cout << "Result: ";
    std::cout << Converter::Calculate(Converter::ConvertToPostfixForm(buffer), var);
    std::cout << std::endl << std::endl;
    Converter::Clear();
  }
  catch (MyException const& e)
  {
    std::cout << std::endl << e.what() << std::endl;
    std::cout << "errorLine: " << e.errorLine() << std::endl;
    std::cout << "errorFile: " << e.errorFile() << std::endl;
  }

}

//--------------------------------------- Main ------------------------------------------//

int main()
{
  std::cout << "//~~~~~~~~~~~~~~~~~ Test of class \"TList\" ~~~~~~~~~~~~//" << std::endl << std::endl;
  _tlistMain();

  std::cout << "//~~~~~~~~~~~~~~~~~ Test of poftfix form ~~~~~~~~~~~~//" << std::endl << std::endl;

  int stackBase = 0;
  int isItAll = false;
  enum _baseOfStack
  {
    _array,
    _list
  } baseOfStack;

  do
  {
    std::cout << "Choose: /n    1 - List     0 - Array" << std::endl;
    std::cin >> stackBase;
    baseOfStack = stackBase ? _array : _list;
    std::cin.ignore();
    switch (baseOfStack)
    {
    case _array:
      _tStackMain();
      break;
    case _list:
      _tStackMain();
      break;
    }
    std::cout << "Do you want to exit?" << std::endl;
    std::cout << "1 - Yes       0 - No" << std::endl;
    std::cin >> isItAll;
    std::cin.ignore();
  } while (!isItAll);

  system("pause");
  return 0;
}