#pragma once
#include <vector>

using namespace std;

struct Production
{
  int index;
  int nonTerminalIndex;
  vector<int> handles;
};