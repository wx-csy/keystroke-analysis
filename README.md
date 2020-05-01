# keystroke-analysis

This is an analyzer for the dataset in paper "Observations on Typing from 136 Million Keystrokes" by Dharkal V et al. 

The dataset can be downloaded from http://userinterfaces.aalto.fi/136Mkeystrokes.

## Usage

1. Download the dataset and put all ******_keystrokes.txt files in `files`.
2. Run `make` for preprocessing.
3. Enter `reducer` and run `make` for processing.
4. Move `ops.txt` from `reducer` to parent, then run `postproc.py` for postprocessing.
