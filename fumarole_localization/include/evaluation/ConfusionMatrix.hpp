//
// ConfusionMatrix.hpp
// Represents a confusion matrix for multi-class evaluation
//

#ifndef FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP
#define FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP

#include <vector>
#include <string>
#include <tuple>

#ifdef __APPLE__
    #include <eigen3/Eigen/Eigen>
#else
    #include <Eigen/Eigen>
#endif

namespace Evaluation
{
    class ConfusionMatrix
    {
    public:
        /// Construct the matrix with the given class labels
        /// \param classLabels A list of class labels
        ConfusionMatrix(const std::vector<std::string>& classLabels);

        ~ConfusionMatrix();

        /// Add classification count for a classification output by the classifier
        /// \param predictedClass The name of the class that the classifier predicted
        /// \param actualClass The name of the class that was the ground truth class
        /// \param count The number of classifications made
        void AddClassifications(const std::string& predictedClass, const std::string& actualClass, int count = 1);

        /// Get the current classifications as the count of classifications predicted and the total number of ground truth classifications
        /// \param classLabel The name of the class
        /// \return Returns a tuple where 0 -> predicted cases, 1 -> actual cases for the class
        std::tuple<int, int> GetClassificationScore(const std::string& classLabel) const;

        /// Returns the accuracy of the classifier
        /// \return The accuracy score
        float GetAccuracy() const;

        /// Returns the precision of the classifier for the given class
        /// \param classLabel The name of the class
        /// \return The precision score
        float GetPrecision(const std::string& classLabel) const;

        /// Returns the recall of the classifier for the given class
        /// \param classLabel
        /// \return The recall score
        float GetRecall(const std::string& classLabel) const;

    private:
        int IndexOfClass(const std::string& classLabel) const;

    private:
        std::vector<std::string> m_ClassLabels;
        Eigen::MatrixX2i m_Matrix;
    };
}

#endif //FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP
