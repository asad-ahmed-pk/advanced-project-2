# generate_plots.py
# Generate plots for the evaluation

import seaborn as sns

import matplotlib.pyplot as plt
import pandas as pd

# files paths and constants
confusion_matrix_file_path = "../fumarole_localization/build/confusion_matrix/test_set_4/classification_confusion_matrix.csv"
detection_metrics_csv_path = "../fumarole_localization/build/detection_metrics/total_metrics.csv"

def plot_confusion_matrix():
    """ Plot the confusion matrix for the classification evaluation """
    df = pd.read_csv(confusion_matrix_file_path, header=None)
    matrix = df.iloc[1:,1:].to_numpy()

    class_labels = ["hole", "open_vent", "heated_area", "hidden_vent"]

    df_fixed = pd.DataFrame(matrix, class_labels, class_labels).astype(int)
    print()
    print(df_fixed)

    plt.figure(figsize = (10,7))
    sns.set(font_scale=1.2)
    sns.heatmap(df_fixed, annot=True, annot_kws={"size": 12}, fmt='d')

    plt.savefig('output/confusion_matrix.png')
    print("\nHeatmap saved")


def plot_detection_precision_plot():
    """ Plot the location error threshold vs the precision for the detector """
    df = pd.read_csv(detection_metrics_csv_path)

    plt.figure(figsize = (10,7))
    ax = sns.lineplot(x="Threshold", y="Precision", data=df)
    ax.set(xlabel='Center Location Error (Pixels)', ylabel='Precision', title='Detector Precision Plot')
    plt.savefig('output/detector_precision_plot.png')

    print('\nDetector precision plot saved')


def run_main():
    plot_detection_precision_plot()
    #plot_confusion_matrix()

if __name__ == "__main__":
    run_main()
