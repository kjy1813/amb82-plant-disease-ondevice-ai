# Training

`train_model.py` trains the CNN used in the final demo and saves it as `model.h5`.

## Environment

```bash
conda create -n amb82 python=3.10
conda activate amb82
pip install -r requirements.txt   # tensorflow==2.14.1
```

TensorFlow 2.14.1 was chosen because it was the highest version accepted by the AmebaPro2
converter. Newer versions produced `.h5` files the converter rejected.

## Dataset layout

The script uses `flow_from_directory`, so each class is a sub-folder of `dataset/`.
Folder names sort alphabetically and become the output indices, which is why they carry a numeric prefix:

```
dataset/
├── 0_cabbage_good/   # healthy cabbage (~300 images)
└── 1_cabbage_bad/    # rotten / diseased cabbage (~300 images)
```

Set `NUM_CLASSES` to the number of folders. The submitted script is left at `3`, the value used for
the tomato side experiment (healthy / mosaic virus / yellow leaf curl); use `2` for the cabbage model.

The images were gathered from web image search for coursework only and are **not redistributed** here.

## Settings

| Item | Value |
|---|---|
| Input | 224 x 224 x 3 (RGB) |
| Batch size | 16 |
| Epochs | 30 |
| Normalization | 1/255 |
| Validation split | 10 % |
| Train augmentation | horizontal/vertical flip, shear 0.2, zoom 0.2, width/height shift 0.1 |
| Validation | normalization only |
| Loss / optimizer | categorical cross-entropy / Adam |

## Output

`model.h5` (weights, no optimizer state). It is about 25 MB and is git-ignored.
Convert it to `.nb` as described in [`../model/model_conversion.md`](../model/model_conversion.md).
