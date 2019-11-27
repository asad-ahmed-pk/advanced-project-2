# generate_plots.py
# Generate plots for the evaluation

import seaborn as sns

import matplotlib.pyplot as plt
import pandas as pd

# files paths and constants
confusion_matrix_file_path = "../fumarole_localization/build/confusion_matrix/test_set_4/classification_confusion_matrix.csv"

def plot_confusion_matrix():
    """ Plot the confusion matrix for the classification evaluation """
    df = pd.read_csv(confusion_matrix_file_path, header=None)
    matrix = df.iloc[1:,1:].to_numpy()

    class_labels = ["hole", "open_vent", "heated_area", "hidden_vent"]

    df_fixed = pd.DataFrame(matrix, class_labels, class_labels).astype(int)
    print(df_fixed)

    plt.figure(figsize = (10,7))
    sns.set(font_scale=1.2)
    sns.heatmap(df_fixed, annot=True, annot_kws={"size": 12}, fmt='d')

    plt.savefig('output/confusion_matrix.png')
    print("\nHeatmap saved")

def run_main():
    plot_confusion_matrix()

if __name__ == "__main__":
    run_main()
