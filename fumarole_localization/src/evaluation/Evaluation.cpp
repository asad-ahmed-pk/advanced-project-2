//
// Evaluation.cpp
//

#include "evaluation/Evaluation.hpp"

namespace Evaluation
{
    FumaroleDetectionEvaluation::FumaroleDetectionEvaluation() : ConfusionMatrix(Model::ClassLabels)
    {

    }

    AlgorithmEvaluation::AlgorithmEvaluation() : ConfusionMatrix(Model::ClassLabels)
    {

    }
}

