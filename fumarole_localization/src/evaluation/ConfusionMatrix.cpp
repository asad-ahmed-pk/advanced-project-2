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
    ConfusionMatrix::ConfusionMatrix(const std::vector<std::string> &classLabels) : m_ClassLabels(classLabels), m_Matrix(classLabels.size(), classLabels.size())
    {
        // initialise matrix
        for (size_t i = 0; i < m_ClassLabels.size(); i++)
        {
            for (size_t j = 0; j < m_ClassLabels.size(); j++) {
                m_Matrix(i, j) = 0;
            }
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

        if (total == 0) {
            total = 1.0;
        }

        return (correct / total);
    }

    // Output stream
    std::ostream& operator<<(std::ostream& os, const ConfusionMatrix& cm)
    {
        // output headers
        os << ",";
        for (size_t i = 0; i < cm.m_ClassLabels.size(); i++)
        {
            os << cm.m_ClassLabels[i];
            if (i < cm.m_ClassLabels.size() - 1) {
                os << ",";
            }
        }
        os << std::endl;

        // output confusion matrix
        for (int i = 0; i < cm.m_ClassLabels.size(); i++)
        {
            os << cm.m_ClassLabels[i] << ",";
            for (int j = 0; j < cm.m_ClassLabels.size(); j++)
            {
                os << cm.m_Matrix(i, j);
                if (j < cm.m_ClassLabels.size() - 1) {
                    os << ",";
                }
            }
            os << std::endl;
        }

        return os;
    }

    // Append other confusion matrix data
    ConfusionMatrix& ConfusionMatrix::operator+=(const Evaluation::ConfusionMatrix &other) {
        this->m_Matrix += other.m_Matrix;
        return *this;
    }
}
