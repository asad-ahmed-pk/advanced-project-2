//
// ConfusionMatrix.hpp
// Represents a confusion matrix for multi-class evaluation
//

#ifndef FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP
#define FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP

#include <vector>
#include <string>
#include <tuple>
#include <iostream>

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

        /// Output confusion matrix as a CSV stream
        /// \param os output stream
        /// \param cm Confusion matrix
        /// \return A reference to the output stream
        friend std::ostream& operator<<(std::ostream& os, const ConfusionMatrix& cm);

        /// Append another confusion matrix scores
        /// \param other The other confusion matrix (dimensions must match)
        /// \return Return the reference to this matrix with the appended matrix data
        ConfusionMatrix& operator+=(const ConfusionMatrix& other);

    private:
        int IndexOfClass(const std::string& classLabel) const;

    private:
        std::vector<std::string> m_ClassLabels;
        Eigen::MatrixXi m_Matrix;
    };
}

#endif //FUMAROLE_LOCALIZATION_CONFUSIONMATRIX_HPP
