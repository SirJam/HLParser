#pragma once

struct Action
{
  int stateIndex;
  int symbolIndex;
  int type;
  int target;
};

/*
Action types:
1 - Shift to State
2 - Reduce Production
3 - Go to State
4 - Accept
*/