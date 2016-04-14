#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <ctime>
#include <queue>
#include <stack>
#include "CbcConfig.h"
#include "CoinPragma.hpp"
#include "defines.h"

// For Branch and bound
#include "OsiSolverInterface.hpp"
#include "CbcModel.hpp"
#include "CbcBranchUser.hpp"
#include "CbcCompareUser.hpp"
#include "CbcCutGenerator.hpp"
#include "CbcHeuristicLocal.hpp"
#include "OsiClpSolverInterface.hpp"

// Cuts

#include "CglGomory.hpp"
#include "CglProbing.hpp"
#include "CglKnapsackCover.hpp"
#include "CglOddHole.hpp"
#include "CglClique.hpp"
#include "CglFlowCover.hpp"
#include "CglMixedIntegerRounding.hpp"

// Heuristics

#include "CbcHeuristic.hpp"

// Methods of building

#include "CoinBuild.hpp"
#include "CoinModel.hpp"

#include "CoinTime.hpp"
