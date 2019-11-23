//
// ConfusionMatrix.cpp
// Represents a confusion matrix for multi-class evaluation
//

#include "evaluation/ConfusionMatrix.hpp"

#include <algorithm>

#ifdef __APPLE__
    #include <eigen3/Eigen/Dense>
#else
    #include <Eigen/Dense>
#endif

namespace Evaluation
{
    // Constructor
    ConfusionMatrix::ConfusionMatrix(const std::vector<std::string> &classLabels) : m_ClassLabels(classLabels)
    {
        // initialise matrix
        m_Matrix.resize(m_ClassLabels.size(), m_ClassLabels.size());
        for (size_t i = 0; i < m_ClassLabels.size(); i++) {
            m_Matrix(i, i) = 0;
        }
    }

    // Destructor
    ConfusionMatrix::~ConfusionMatrix() = default;

    void ConfusionMatrix::AddClassifications(const std::string &predictedClass, const std::string &actualClass, int count)
    {
        int x = IndexOfClass(predictedClass);
        int y = IndexOfClass(actualClass);

        m_Matrix(y, x) = m_Matrix(y, x) + count;
    }

    // Get the index of class label
    int ConfusionMatrix::IndexOfClass(const std::string& classLabel) const
    {
        auto iter = std::find(m_ClassLabels.begin(), m_ClassLabels.end(), classLabel);
        if (iter != m_ClassLabels.end()) {
            return static_cast<int>(std::distance(m_ClassLabels.begin(), iter));
        }
        else {
            return -1;
        }
    }

    // Precision
    float ConfusionMatrix::GetPrecision(const std::string& classLabel) const
    {
        int index = IndexOfClass(classLabel);
        if (index < 0) {
            return 0.0;
        }

        float truePositives = m_Matrix(index, index);
        float total = m_Matrix.colwise().sum()(0);

        if (total == 0.0) {
            total = 1.0;
        }

        return (truePositives / total);
    }

    // Recall
    float ConfusionMatrix::GetRecall(const std::string &classLabel) const
    {
        int index = IndexOfClass(classLabel);
        if (index < 0) {
            return 0.0;
        }

        float truePositives = m_Matrix(index, index);

        float total = m_Matrix.rowwise().sum()(0);
        if (total == 0.0) {
            total = 1.0;
        }

        return (truePositives / total);
    }

    // Accuracy
    float ConfusionMatrix::GetAccuracy() const
    {
        float correct = m_Matrix.diagonal().sum();
        float total = m_Matrix.sum();
        return (correct / total);
    }
}
